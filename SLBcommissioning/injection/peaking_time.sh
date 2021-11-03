#!/bin/bash
#Expect files in this format:
# /path/run_XXXXX_SLB_2.root (or SLB_3)
# run example:
#> source analysis.sh XXXXX convert
# where:
#          XXXXX is the run number
#          convert=0 if no conversiom =1 if yes

#source analysis.sh 21010 1

#Run_ILC_test_cosmic_02192020_18h30min_Ascii"
data_folder="/mnt/win2/Run_data/"
run="20200226_dac1.15V_chn0to3_hold80"
repo_top_level=$(dirname $(dirname $(dirname $(realpath $BASH_SOURCE))))
cd ${repo_top_level}/converter_SLB


for irun in 30 60 90 120
do
    #irun=120
    folder=${data_folder}${run}"_FSpeakTime"${irun}"ns_Ascii/"
    output="${repo_top_level}/converter_SLB/convertedfiles/"${run}"_Ascii/"
    mkdir ${output}
    root -l -q ConvertDirectorySL.cc\(\"${folder}\",false,\"${output}\"\)
    hadd ${output}/FSpeakTime${irun}ns.root ${output}/conv*.root
    rm ${output}/conv*.root
done

cd -
root -l PeakTime4Channels.cc+

#hadd ${output}/${run}.root ${output}/*.root 

#root -l SingleSlabAnalysis.cc\(\"${output}/${run}\",\"${run}\",1\) &
