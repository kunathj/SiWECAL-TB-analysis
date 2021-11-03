#!/bin/bash

date=$1
round=$2
it=$3
nslabs=$4

if [[ $1 == "" ]]
 then
 echo "You forgot the 1st Argument: date --> 05182020 for the 5th May 2020"
 (exit 33)
fi

if [[ $2 == "" ]]
 then
 echo "You forgot the 2nd Argument: Round (masking, scurves, cosmic)"
 (exit 33)
fi

if [[ $3 == "" ]]
 then
 echo "You forgot the 3rd Argument: iteration"
 (exit 33)
fi


repo_top_level=$(dirname $(dirname $(dirname $(realpath $BASH_SOURCE))))
if [ ! -d "${repo_top_level}/SLBcommissioning/${date}" ]; then
    mkdir ${repo_top_level}/SLBcommissioning/${date}  
fi
if [ ! -d "${repo_top_level}/SLBcommissioning/masking/histos" ]; then
    mkdir ${repo_top_level}/SLBcommissioning/masking/histos
fi

data_path="/mnt/win1/"
# data_path="/mnt/win1/"


if [[ $round == "masking" ]]
then
    for i in 0 1 2
    do
	run="Run_ILC_"${date}"_"${round}"_it"${it}"_"${i}"_Ascii"
	mkdir ${repo_top_level}/converter_SLB/convertedfiles/${run}
	output=${repo_top_level}"/converter_SLB/convertedfiles/"${run}"/"
	
	cd ${repo_top_level}/converter_SLB
	data_folder=${data_path}"/Run_Data/"${run}"/"
	root -l -q ConvertDirectorySL.cc\(\"${data_folder}\",false,\"${output}\"\) 
	hadd ${output}/../Run_ILC_${date}_${round}_it${it}_${i}.root ${output}/*.root 
	cd -
    done

    cp ${data_path}/Run_Data/Run_ILC_${date}_${round}_it${it}_2_Ascii/Run_Settings.txt  ${repo_top_level}/SLBcommissioning/${date}/Run_Settings_it${it}.txt
    root -l -q SimpleNoiseChecks.cc\(\"${date}\",\"${round}\",${it}\)
    it2=$((it+1))
    cp ${repo_top_level}/SLBcommissioning/${date}/Run_Settings_comm_it${it2}.txt ${data_path}/Setup/.
fi



if [[ $round == "scurves" ]]
then
    echo "DID YOU SET UP CORRECTLY THE SLBOARD-ADDress mapping in scurves.C ???"
    echo "DID you properly setup the address of the sk2 and sk2a versions in ../scurves/fithistos.C function fithistos ?? (it uses the slab idx, not slab address"
    cp ${data_path}/Histos/RateVsThresholdScan_${date}_SLBoard.txt ../${date}/.
    cp ${data_path}/Run_Data/Run_ILC_${date}_masking_it${it}_Ascii/Run_Settings.txt ${repo_top_level}/SLBcommissioning/${date}/Run_Settings_it${it}.txt

    root -l -q scurves.C\(\"${date}\",${it}\)
    it2=$((it+1))
    cp ${repo_top_level}/SLBcommissioning/${date}/Run_Settings_comm_it${it2}.txt ${data_path}/Setup/.
    cp histos/scurves_${date}.root ../${date}/.
fi




if [[ $round == "cosmic" ]]
then

    run="Run_ILC_"${date}"_"${round}"_it"${it}"_Ascii"
    mkdir ${repo_top_level}"/converter_SLB/convertedfiles/"${run}
    output=${repo_top_level}"/converter_SLB/convertedfiles/"${run}"/"

    cd ${repo_top_level}/converter_SLB
    data_folder=${data_path}"/Run_Data/"${run}"/"
    root -l -q ConvertDirectorySL.cc\(\"${data_folder}\",false,\"${output}\"\)
    hadd ${output}/../Run_ILC_${date}_${round}_it${it}.root ${output}/*.root
    cd -

    cp ${data_path}/Run_Data/Run_ILC_${date}_${round}_it${it}_Ascii/Run_Settings.txt  ${repo_top_level}/SLBcommissioning/${date}/Run_Settings_it${it}.txt
    root -l -q SimpleNoiseChecks.cc\(\"${date}\",\"${round}\",${it}\)
    it2=$((it+1))
    cp ${repo_top_level}/SLBcommissioning/${date}/Run_Settings_comm_it${it2}.txt ${data_path}/Setup/.
fi
