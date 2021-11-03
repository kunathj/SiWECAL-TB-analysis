#!/usr/bin/env python
from __future__ import print_function
import os
import numpy as np


class EcalNumbers:
    def __init__(self, slabs=None, cob_slabs=None):
        self.n_chips = 16
        self.n_scas = 15
        self.n_channels = 64
        if slabs is None:
            self.slabs = []
        else:
            self.slabs = sorted(set(slabs))
        if cob_slabs is None:
            self.cob_slabs = []
        else:
            self.cob_slabs = sorted(set(cob_slabs))
        self.n_slabs = len(self.slabs)

        w_conf_1 = np.full(self.n_slabs, 2.1)
        w_conf_1[-3:] = 4.2
        self.w_config_options = {  # abs thickness of Tungsten/W plates.
            1: w_conf_1,
        }

        self.bcid_skip_noisy_acquisition_start = 50
        self.bcid_merge_delta = 3
        self.bcid_too_many_hits = 8000

        self.pedestal_min_average = 200
        self.pedestal_min_scas = 3
        self.pedestal_min_value = 10
        self.mip_cutoff = 0.5
        self.validate_ecal_numbers(self)


    @classmethod
    def validate_ecal_numbers(cls, n):
        assert type(n.n_chips) == int
        assert type(n.n_scas) == int
        assert type(n.n_channels) == int
        assert type(n.n_slabs) == int
        assert len(n.slabs) == n.n_slabs
        assert all((type(i_slab) == int for i_slab in n.slabs))
        assert all((i_cob in n.slabs for i_cob in n.cob_slabs))

        assert all(type(w_conf) == np.ndarray for w_conf in n.w_config_options.values())
        assert all(len(w_conf) == n.n_slabs for w_conf in n.w_config_options.values())

        assert type(n.bcid_skip_noisy_acquisition_start) == int
        assert n.bcid_merge_delta >= 0
        assert type(n.bcid_too_many_hits) == int

        assert type(n.pedestal_min_average) == int
        assert type(n.pedestal_min_scas) == int
        assert type(n.pedestal_min_value) == int
        assert type(n.mip_cutoff) == float and n.mip_cutoff <= 1


class EventBuildingException(Exception):
    pass

dummy_config = dict(
    mapping_file="mapping/fev10_chip_channel_x_y_mapping.txt",
    mapping_file_cob="mapping/fev11_cob_chip_channel_x_y_mapping.txt",
    pedestals_file="pedestals/pedestal_PROTO15_dummy.txt",
    mip_calibration_file="mip_calib/MIP_PROTO15_dummy.txt",
    masked_file="masked/masked_PROTO15_dummy.txt",
)


def position_per_slab(slab):
    return 15 * slab  # in mm.


class EcalHit:
    def __init__(self, slab, chip, chan, sca, hg, lg, gain_hit_high, ecal_config):
        self.slab = slab
        self.chip = chip
        self.chan = chan
        self.sca = sca
        self.hg = hg
        self.lg = lg
        self._ecal_config = ecal_config
        self._idx_slab = self._ecal_config._N.slabs.index(self.slab)

        self.isMasked = int(self._ecal_config.masked_map[self._idx_slab][self.chip][self.chan])
        self.isCommissioned = 1 if self.isMasked == 0 else 0
        self._gain_hit_high = gain_hit_high

        self._set_positions()
        self._pedestal_subtraction()
        self._mip_calibration()


    @property
    def isHit(self):
        return 1 if self._gain_hit_high > 0 else 0

    def _set_positions(self):
        self.z = position_per_slab(self.slab)  # Here we use the actual slab position, not the index.
        slab_channel_map = self._ecal_config.get_channel_map(self.slab)
        (self.x,self.y) = slab_channel_map[(self.chip, self.chan)]
        # TODO: Is this really doing the right thing to the positioning? Check with new cosmic runs.
        # Invert the mapping for the 5 first slabs, to agree with the last 4.
        # if slab < 5: currently equivalent to:
        # if "_dif_" in self._ecal_config._N.slab_map[self._idx_slab]:
        #    self.x = -self.x
        #    self.y = -self.y


    def _pedestal_subtraction(self):
        pedestals_per_sca = self._ecal_config.pedestal_map[self._idx_slab][self.chip][self.chan]

        sca_has_valid_pedestal = pedestals_per_sca[self.sca] > self._ecal_config._N.pedestal_min_value
        if sca_has_valid_pedestal:
            self.hg -= pedestals_per_sca[self.sca]
        else:
            is_good_pedestal = pedestals_per_sca > self._ecal_config._N.pedestal_min_average
            if sum(is_good_pedestal) > 0:
                pedestal_average = np.mean(pedestals_per_sca[is_good_pedestal])
            else:
                pedestal_average = 0
            if sum(is_good_pedestal) < self._ecal_config._N.pedestal_min_scas:
                self.isCommissioned = 0
            self.hg -= pedestal_average


    def _mip_calibration(self):
        mip_value = self._ecal_config.mip_map[self._idx_slab][self.chip][self.chan]
        if mip_value > self._ecal_config._N.mip_cutoff:
            self.energy = self.hg / mip_value
        else:
            self.energy = 0
            self.isCommissioned = 0


class EcalConfig:

    def __init__(
        self,
        mapping_file=dummy_config["mapping_file"],
        mapping_file_cob=dummy_config["mapping_file_cob"],
        pedestals_file=dummy_config["pedestals_file"],
        mip_calibration_file=dummy_config["mip_calibration_file"],
        masked_file=dummy_config["masked_file"],
        commissioning_folder=None,
        numbers=None,
        error_on_missing_config=True,
        verbose=False,
    ):
        self._verbose = verbose
        self._error_on_missing_config = error_on_missing_config
        if commissioning_folder:
            self._commissioning_folder = commissioning_folder
        else:
            # Resolves to the root folder of this repo
            # Equivalent to ../ if called from within the eventbuilding folder.
            self._commissioning_folder = os.path.dirname(os.path.dirname((__file__)))
        if numbers:
            EcalNumbers.validate_ecal_numbers(numbers)  # Catch problems early on.
            self._N = numbers
        else:
            self._N = EcalNumbers()

        self._channel_map = self._read_mapping(mapping_file)
        self._channel_map_cob = self._read_mapping(mapping_file_cob)
        self.pedestal_map = self._read_pedestals(pedestals_file)
        self.mip_map = self._read_mip_values(mip_calibration_file)
        self.masked_map = self._read_masked(masked_file)


    def get_channel_map(self, slab):
        if slab in self._N.cob_slabs:
            return self._channel_map
        else:
            return self._channel_map_cob


    def _get_lines(self, file_name):
        if not os.path.isabs(file_name):
            file_name = os.path.join(self._commissioning_folder, file_name)
        if not os.path.exists(file_name):
            raise EventBuildingException("File does not exist: %s" %file_name)

        try:
            lines = open(file_name).readlines()
        except FileNotFoundError:
            txt = "%s does not exist" %file_name
            if self._error_on_missing_config:
                raise EventBuildingException(txt)
        return lines


    def _read_mapping(self, file_name):
        channel_map = dict()
        lines = self._get_lines(file_name)

        fields = lines[0].split()
        i_chip = fields.index("chip")
        i_channel = fields.index("channel")
        i_x = fields.index("x")
        i_y = fields.index("y")

        for line in lines[1:]:
            v = line.split()
            pos = (float(v[i_x]), float(v[i_y]))
            channel_map[(int(v[i_chip]), int(v[i_channel]))] = pos
        return channel_map


    def _read_pedestals(self, file_name):
        ped_map = np.zeros((
            self._N.n_slabs,
            self._N.n_chips,
            self._N.n_channels,
            self._N.n_scas,
        ))
        print("Reading pedestals from %s." %file_name)
        lines = self._get_lines(file_name)

        assert lines[0].startswith("#pedestal results")
        assert lines[1].startswith("#") and not lines[2].startswith("#")
        fields = lines[1][1:].split()
        i_slab = fields.index("layer")
        i_chip = fields.index("chip")
        i_channel = fields.index("channel")
        i_ped0 = fields.index("ped0")


        for line in lines[2:]:
            v = line.split()
            for i_sca in range(ped_map.shape[-1]):
                n_entries_per_sca = 3  # ped, eped, widthped
                ped_val = float(v[2 + i_sca * n_entries_per_sca])
                idx_slab = self._N.slabs.index(int(v[i_slab]))
                ped_map[idx_slab][int(v[i_chip])][int(v[i_channel])][i_sca] = ped_val
        if self._verbose:
            print("pedestal_map", ped_map)
        return ped_map


    def _read_mip_values(self, file_name):
        mip_map = np.ones((
            self._N.n_slabs,
            self._N.n_chips,
            self._N.n_channels,
        ))
        print("Reading MIP values from %s." %file_name)
        lines = self._get_lines(file_name)

        assert lines[0].startswith("#mip results")
        assert lines[1].startswith("#") and not lines[2].startswith("#")
        fields = lines[1][1:].split()
        i_slab = fields.index("layer")
        i_chip = fields.index("chip")
        i_channel = fields.index("channel")
        i_mpv = fields.index("mpv")

        for line in lines[2:]:
            v = line.split()
            mip_val = float(v[i_mpv])
            idx_slab = self._N.slabs.index(int(v[i_slab]))
            mip_map[idx_slab][int(v[i_chip])][int(v[i_channel])] = mip_val
        if self._verbose:
            print("mip_map", mip_map)
        return mip_map


    def _read_masked(self, file_name):
        masked_map = np.zeros((
            self._N.n_slabs,
            self._N.n_chips,
            self._N.n_channels,
        ))
        print("Reading masked channels from %s." %file_name)
        lines = self._get_lines(file_name)

        start_tag = "#masked_chns_list "
        assert lines[0].startswith(start_tag)
        assert not lines[1].startswith("#")
        fields = lines[0][len(start_tag):].split()
        assert fields[0] == "layer"
        assert fields[1] == "chip"
        assert fields[2] == "chns"

        for line in lines[1:]:
            v = line.split()
            assert len(v) == self._N.n_channels + 2
            idx_slab = self._N.slabs.index(int(v[0]))
            chip = int(v[1])
            for channel, mask_val in enumerate(v[2:]):
                masked_map[idx_slab][chip][channel] = mask_val
        if self._verbose:
            print("masked_map", masked_map)
        return masked_map


if __name__ == "__main__":
    EcalConfig(verbose=True)
