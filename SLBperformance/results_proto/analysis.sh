run=$1
for slab in {0..14}
do
    is_cob=false
    # if [[ $slab == 5 ]] || [[ $slab == 8 ]]; then is_cob=true; fi
    root -l -q analysis.cc\(\"$run\",$slab,$is_cob\) 
done
