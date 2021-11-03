#!/bin/bash
#Expect files in this format:
# /path/run_XXXXX_SLB_2.root (or SLB_3)
# run example:
#> source analysis.sh XXXXX convert
# where:
#          XXXXX is the run number
#          convert=0 if no conversiom =1 if yes

#source analysis.sh 21010 1

run="Run_ILC_06042020_cosmic_14_Ascii"
data_folder="/mnt/win2/Run_Data/"${run}"/"

repo_top_level=$(dirname $(dirname $(dirname $(realpath $BASH_SOURCE))))
mkdir "${repo_top_level}/converter_SLB/convertedfiles/"${run}
output="${repo_top_level}/converter_SLB/convertedfiles/"${run}"/"

cd ${repo_top_level}/converter_SLB
root -l -q ConvertDirectorySL.cc\(\"${data_folder}\",false,\"${output}\"\) 
hadd ${output}/${run}.root ${output}/*.root 
cd -

root -l DummyDisplay.cc\(\"${output}/${run}\",\"${run}\"\) 
