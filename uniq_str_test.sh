#!/bin/bash

#SBATCH -p development
#SBATCH -n 16
#SBATCH -t 0:20:0

export OMP_NUM_THREADS=4

./uniq_str Q2input/dict_almostsorted 25481 0
./uniq_str Q2input/dict_randomized 25481 0
./uniq_str Q2input/dict_revsorted 25481 0
./uniq_str Q2input/dict_sorted 25481 0

./uniq_str Q2input/rdfquads_almostsorted 45595 0
./uniq_str Q2input/rdfquads_randomized 45595 0
./uniq_str Q2input/rdfquads_revsorted 45595 0
./uniq_str Q2input/rdfquads_sorted 45595 0

./uniq_str Q2input/rdfquadssub_almostsorted 45595 0
./uniq_str Q2input/rdfquadssub_randomized 45595 0
./uniq_str Q2input/rdfquadssub_revsorted 45595 0
./uniq_str Q2input/rdfquadssub_sorted 45595 0

./uniq_str Q2input/dict_almostsorted 25481 1
./uniq_str Q2input/dict_randomized 25481 1
./uniq_str Q2input/dict_revsorted 25481 1
./uniq_str Q2input/dict_sorted 25481 1

./uniq_str Q2input/rdfquads_almostsorted 45595 1
./uniq_str Q2input/rdfquads_randomized 45595 1
./uniq_str Q2input/rdfquads_revsorted 45595 1
./uniq_str Q2input/rdfquads_sorted 45595 1

./uniq_str Q2input/rdfquadssub_almostsorted 45595 1
./uniq_str Q2input/rdfquadssub_randomized 45595 1
./uniq_str Q2input/rdfquadssub_revsorted 45595 1
./uniq_str Q2input/rdfquadssub_sorted 45595 1

./uniq_str Q2input/dict_almostsorted 25481 2
./uniq_str Q2input/dict_randomized 25481 2
./uniq_str Q2input/dict_revsorted 25481 2
./uniq_str Q2input/dict_sorted 25481 2

./uniq_str Q2input/rdfquads_almostsorted 45595 2
./uniq_str Q2input/rdfquads_randomized 45595 2
./uniq_str Q2input/rdfquads_revsorted 45595 2
./uniq_str Q2input/rdfquads_sorted 45595 2

./uniq_str Q2input/rdfquadssub_almostsorted 45595 2
./uniq_str Q2input/rdfquadssub_randomized 45595 2
./uniq_str Q2input/rdfquadssub_revsorted 45595 2
./uniq_str Q2input/rdfquadssub_sorted 45595 2

./uniq_str Q2input/dict_almostsorted 25481 3
./uniq_str Q2input/dict_randomized 25481 3
./uniq_str Q2input/dict_revsorted 25481 3
./uniq_str Q2input/dict_sorted 25481 3

./uniq_str Q2input/rdfquads_almostsorted 45595 3
./uniq_str Q2input/rdfquads_randomized 45595 3
./uniq_str Q2input/rdfquads_revsorted 45595 3
./uniq_str Q2input/rdfquads_sorted 45595 3

./uniq_str Q2input/rdfquadssub_almostsorted 45595 3
./uniq_str Q2input/rdfquadssub_randomized 45595 3
./uniq_str Q2input/rdfquadssub_revsorted 45595 3
./uniq_str Q2input/rdfquadssub_sorted 45595 3

./uniq_str Q2input/dict_almostsorted 25481 4
./uniq_str Q2input/dict_randomized 25481 4
./uniq_str Q2input/dict_revsorted 25481 4
./uniq_str Q2input/dict_sorted 25481 4

./uniq_str Q2input/rdfquads_almostsorted 45595 4
./uniq_str Q2input/rdfquads_randomized 45595 4
./uniq_str Q2input/rdfquads_revsorted 45595 4
./uniq_str Q2input/rdfquads_sorted 45595 4

./uniq_str Q2input/rdfquadssub_almostsorted 45595 4
./uniq_str Q2input/rdfquadssub_randomized 45595 4
./uniq_str Q2input/rdfquadssub_revsorted 45595 4
./uniq_str Q2input/rdfquadssub_sorted 45595 4

export OMP_NUM_THREADS=16

./uniq_str Q2input/dict_almostsorted 25481 0
./uniq_str Q2input/dict_randomized 25481 0
./uniq_str Q2input/dict_revsorted 25481 0
./uniq_str Q2input/dict_sorted 25481 0

./uniq_str Q2input/rdfquads_almostsorted 45595 0
./uniq_str Q2input/rdfquads_randomized 45595 0
./uniq_str Q2input/rdfquads_revsorted 45595 0
./uniq_str Q2input/rdfquads_sorted 45595 0

./uniq_str Q2input/rdfquadssub_almostsorted 45595 0
./uniq_str Q2input/rdfquadssub_randomized 45595 0
./uniq_str Q2input/rdfquadssub_revsorted 45595 0
./uniq_str Q2input/rdfquadssub_sorted 45595 0

./uniq_str Q2input/dict_almostsorted 25481 1
./uniq_str Q2input/dict_randomized 25481 1
./uniq_str Q2input/dict_revsorted 25481 1
./uniq_str Q2input/dict_sorted 25481 1

./uniq_str Q2input/rdfquads_almostsorted 45595 1
./uniq_str Q2input/rdfquads_randomized 45595 1
./uniq_str Q2input/rdfquads_revsorted 45595 1
./uniq_str Q2input/rdfquads_sorted 45595 1

./uniq_str Q2input/rdfquadssub_almostsorted 45595 1
./uniq_str Q2input/rdfquadssub_randomized 45595 1
./uniq_str Q2input/rdfquadssub_revsorted 45595 1
./uniq_str Q2input/rdfquadssub_sorted 45595 1

./uniq_str Q2input/dict_almostsorted 25481 2
./uniq_str Q2input/dict_randomized 25481 2
./uniq_str Q2input/dict_revsorted 25481 2
./uniq_str Q2input/dict_sorted 25481 2

./uniq_str Q2input/rdfquads_almostsorted 45595 2
./uniq_str Q2input/rdfquads_randomized 45595 2
./uniq_str Q2input/rdfquads_revsorted 45595 2
./uniq_str Q2input/rdfquads_sorted 45595 2

./uniq_str Q2input/rdfquadssub_almostsorted 45595 2
./uniq_str Q2input/rdfquadssub_randomized 45595 2
./uniq_str Q2input/rdfquadssub_revsorted 45595 2
./uniq_str Q2input/rdfquadssub_sorted 45595 2

./uniq_str Q2input/dict_almostsorted 25481 3
./uniq_str Q2input/dict_randomized 25481 3
./uniq_str Q2input/dict_revsorted 25481 3
./uniq_str Q2input/dict_sorted 25481 3

./uniq_str Q2input/rdfquads_almostsorted 45595 3
./uniq_str Q2input/rdfquads_randomized 45595 3
./uniq_str Q2input/rdfquads_revsorted 45595 3
./uniq_str Q2input/rdfquads_sorted 45595 3

./uniq_str Q2input/rdfquadssub_almostsorted 45595 3
./uniq_str Q2input/rdfquadssub_randomized 45595 3
./uniq_str Q2input/rdfquadssub_revsorted 45595 3
./uniq_str Q2input/rdfquadssub_sorted 45595 3

./uniq_str Q2input/dict_almostsorted 25481 4
./uniq_str Q2input/dict_randomized 25481 4
./uniq_str Q2input/dict_revsorted 25481 4
./uniq_str Q2input/dict_sorted 25481 4

./uniq_str Q2input/rdfquads_almostsorted 45595 4
./uniq_str Q2input/rdfquads_randomized 45595 4
./uniq_str Q2input/rdfquads_revsorted 45595 4
./uniq_str Q2input/rdfquads_sorted 45595 4

./uniq_str Q2input/rdfquadssub_almostsorted 45595 4
./uniq_str Q2input/rdfquadssub_randomized 45595 4
./uniq_str Q2input/rdfquadssub_revsorted 45595 4
./uniq_str Q2input/rdfquadssub_sorted 45595 4
