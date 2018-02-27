#!/bin/bash
make

minSectors=${1}
maxSectors=${2}

./aliFitFlowToy -m ideal -n 5000
for i in `seq ${minSectors} $(($maxSectors-1))`;
do
  ./aliFitFlowToy -m FIT -s ${i} -n 5000
done
