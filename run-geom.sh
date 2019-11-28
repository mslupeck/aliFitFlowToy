#!/bin/bash
make && ./aliFitFlowToy -m sim -g V0A_1x8-V0C_1x8-TPC -i inputs/ -n 1000

#minSectors=${1}
#maxSectors=${2}

#./aliFitFlowToy -m ideal -n 5000
#for i in `seq ${minSectors} $(($maxSectors-1))`;
#do
#  ./aliFitFlowToy -m FIT -s ${i} -n 5000
#done
