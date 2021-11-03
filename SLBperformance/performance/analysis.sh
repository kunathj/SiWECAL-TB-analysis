run=$1
slab=$2
if [ "$3" != "" ]; then
    is_cob=$3
else
    is_cob=false
fi
root -l -q analysis.cc\(\"$run\",$slab,$is_cob\) 
