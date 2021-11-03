#Needs to be run from the pc with the data -> pc-calice2

import subprocess
import argparse
import os

parser = argparse.ArgumentParser()

parser.add_argument("-r","--run", type=str, help="Run name to analyze. Format: (Year)(Month)(Day)_dac(x)V_chn(A)to(B)", metavar="\b")
parser.add_argument("-d","--data", type=str, help="Path to the data folder", metavar="\b")
parser.add_argument("-c","--converter", type=str, help="Path to the converter folder", metavar="\b")
parser.add_argument("-i","--injection", type=str, help="Path to the injection folder", metavar="\b")

args = parser.parse_args()

run_name = "20200226_dac1.15V_chn0to3"
data_folder = "/home/calice/hecgc/Run_data/"  #pc-calice2 -> "/mnt/win2/Run_data/"
converter_folder = "../../converter_SLB/"  #pc-calice2 -> "/home/calice/TB2020/commissioning/SiWECAL-TB-analysis_TB2020/..."
injection_folder = "../../SLBcommissioning/injection/"  #pc-calice2 -> "/home/calice/TB2020/commissioning/SiWECAL-TB-analysis_TB2020/..."

if args.run:
    run_name = args.run
    
if args.data:
    data_folder = args.data

if args.converter:
    converter_folder = args.converter

if not os.path.exists(converter_folder + "convertedfiles"):
    os.mkdir(converter_folder + "convertedfiles")

if args.injection:
    injection_folder = args.injection

if not os.path.exists(injection_folder + "results"):
    os.mkdir(injection_folder + "results")
        
for run in os.listdir(data_folder):
    if run_name in run:

        conv_out_folder_name = converter_folder + "convertedfiles/" + run
        
        if not os.path.exists(conv_out_folder_name):
            os.mkdir(conv_out_folder_name)

        subprocess.call(["root","-l","-q", converter_folder + "ConvertDirectorySL.cc(\"" + data_folder + run + "/\",false, \"" + conv_out_folder_name + "\")"])
        
        hold = run[run.find("hold") + 4:run.find("0_") + 1]
            
        os.system("hadd -f " + conv_out_folder_name + "/holdscan_hold" + str(hold) + ".root " + conv_out_folder_name + "/converted*.root")
        os.system("rm " + conv_out_folder_name + "/converted*.root")
    
        if not os.path.exists(converter_folder + "convertedfiles/" + run_name):
            os.mkdir(converter_folder + "convertedfiles/" + run_name)

        os.system("cp " + conv_out_folder_name + "/holdscan_hold*.root " + converter_folder + "convertedfiles/" + run_name)


subprocess.call(["root","-l","-q", injection_folder + "HoldscanGraphs.cc(\"" + converter_folder + "convertedfiles/\",\"" + run_name+ "\")"])

subprocess.call(["root","-l","-q", injection_folder + "FitHoldscan4Channels.cc(\"" + run_name + "\")"])
