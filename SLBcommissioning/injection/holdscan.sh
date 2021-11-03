#!/bin/bash

data_folder="/mnt/win2/Run_Data/"
run="08312021_dac1.2V_small"
repo_top_level=$(dirname $(dirname $(dirname $(realpath $BASH_SOURCE))))


for chn in "_chn0to7" "_chn8to15" "_chn16to23" "_chn24to31" "_chn32to39" "_chn40to47" "_chn48to55" "_chn56to63"
do
    cd ${repo_top_level}/converter_SLB
    for irun in {20..160..20}
    do
	#irun=80
	folder=${data_folder}${run}${chn}"_hold"${irun}"_Ascii/"

	output="${repo_top_level}/converter_SLB/convertedfiles/"${run}${chn}"_Ascii/"
	mkdir ${output}
	root -l -q ConvertDirectorySL.cc\(\"${folder}\",false,\"${output}\"\)
	hadd ${output}/holdscan_hold${irun}.root ${output}/conv*.root
	rm ${output}/conv*.root
    done
    cd -
done

mkdir "${repo_top_level}/converter_SLB/convertedfiles/"${run}

for irun in {20..160..20}
do
    output="${repo_top_level}/converter_SLB/convertedfiles/"${run}*"_Ascii/"
    hadd ${repo_top_level}/converter_SLB/convertedfiles/${run}/holdscan_hold${irun}.root ${output}/holdscan_hold${irun}.root 
done

root -l -q HoldscanGraphs.cc\(\"${repo_top_level}/converter_SLB/convertedfiles/\",\"08312021_dac1.2V_small\",15,0\)
root -l -q HoldscanGraphs.cc\(\"${repo_top_level}/converter_SLB/convertedfiles/\",\"08312021_dac1.2V_small\",15,1\)
root -l -q HoldscanGraphs.cc\(\"${repo_top_level}/converter_SLB/convertedfiles/\",\"08312021_dac1.2V_small\",15,2\)
root -l -q HoldscanGraphs.cc\(\"${repo_top_level}/converter_SLB/convertedfiles/\",\"08312021_dac1.2V_small\",15,15\) #all sca's
