#!/bin/bash

#SBATCH -p development
#SBATCH -n 16
#SBATCH -t 0:20:0

export OMP_NUM_THREADS=4
# Test C qsort
./flt_val_sort 10000 0 0
./flt_val_sort 10000 1 0
./flt_val_sort 10000 2 0
./flt_val_sort 10000 3 0
./flt_val_sort 10000 4 0
./flt_val_sort 1000000 0 0
./flt_val_sort 1000000 1 0
./flt_val_sort 1000000 2 0
./flt_val_sort 1000000 3 0
./flt_val_sort 1000000 4 0
./flt_val_sort 100000000 0 0
./flt_val_sort 100000000 1 0
./flt_val_sort 100000000 2 0
./flt_val_sort 100000000 3 0
./flt_val_sort 100000000 4 0

# Test inline sort
./flt_val_sort 10000 0 1
./flt_val_sort 10000 1 1
./flt_val_sort 10000 2 1
./flt_val_sort 10000 3 1
./flt_val_sort 10000 4 1
./flt_val_sort 1000000 0 1
./flt_val_sort 1000000 1 1
./flt_val_sort 1000000 2 1
./flt_val_sort 1000000 3 1
./flt_val_sort 1000000 4 1
./flt_val_sort 100000000 0 1
./flt_val_sort 100000000 1 1
./flt_val_sort 100000000 2 1
./flt_val_sort 100000000 3 1
./flt_val_sort 100000000 4 1

# Test parallel merge sort
./flt_val_sort 10000 0 2
./flt_val_sort 10000 1 2
./flt_val_sort 10000 2 2
./flt_val_sort 10000 3 2
./flt_val_sort 10000 4 2
./flt_val_sort 1000000 0 2
./flt_val_sort 1000000 1 2
./flt_val_sort 1000000 2 2
./flt_val_sort 1000000 3 2
./flt_val_sort 1000000 4 2
./flt_val_sort 100000000 0 2
./flt_val_sort 100000000 1 2
./flt_val_sort 100000000 2 2
./flt_val_sort 100000000 3 2
./flt_val_sort 100000000 4 2

export OMP_NUM_THREADS=16
# Test C qsort
./flt_val_sort 10000 0 0
./flt_val_sort 10000 1 0
./flt_val_sort 10000 2 0
./flt_val_sort 10000 3 0
./flt_val_sort 10000 4 0
./flt_val_sort 1000000 0 0
./flt_val_sort 1000000 1 0
./flt_val_sort 1000000 2 0
./flt_val_sort 1000000 3 0
./flt_val_sort 1000000 4 0
./flt_val_sort 100000000 0 0
./flt_val_sort 100000000 1 0
./flt_val_sort 100000000 2 0
./flt_val_sort 100000000 3 0
./flt_val_sort 100000000 4 0

# Test inline sort
./flt_val_sort 10000 0 1
./flt_val_sort 10000 1 1
./flt_val_sort 10000 2 1
./flt_val_sort 10000 3 1
./flt_val_sort 10000 4 1
./flt_val_sort 1000000 0 1
./flt_val_sort 1000000 1 1
./flt_val_sort 1000000 2 1
./flt_val_sort 1000000 3 1
./flt_val_sort 1000000 4 1
./flt_val_sort 100000000 0 1
./flt_val_sort 100000000 1 1
./flt_val_sort 100000000 2 1
./flt_val_sort 100000000 3 1
./flt_val_sort 100000000 4 1

# Test parallel merge sort
./flt_val_sort 10000 0 2
./flt_val_sort 10000 1 2
./flt_val_sort 10000 2 2
./flt_val_sort 10000 3 2
./flt_val_sort 10000 4 2
./flt_val_sort 1000000 0 2
./flt_val_sort 1000000 1 2
./flt_val_sort 1000000 2 2
./flt_val_sort 1000000 3 2
./flt_val_sort 1000000 4 2
./flt_val_sort 100000000 0 2
./flt_val_sort 100000000 1 2
./flt_val_sort 100000000 2 2
./flt_val_sort 100000000 3 2
./flt_val_sort 100000000 4 2