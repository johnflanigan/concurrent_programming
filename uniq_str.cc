#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>

#include <algorithm>
#include <map>
#include <string>

#ifdef _OPENMP
#include <omp.h>
#endif
#include "qsort.h"

static double timer() {
    
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double) (tp.tv_sec) + 1e-6 * tp.tv_usec);

    /* The code below is for another high resolution timer */
    /* I'm using gettimeofday because it's more portable */

    /*
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return ((double) (tp.tv_sec) + 1e-9 * tp.tv_nsec);
    */
}

/* comparison routine for C's qsort */
static int qs_cmpf(const void *u, const void *v) {

    const char **u_s = (const char **) u;
    const char **v_s = (const char **) v;
    return (strcmp(*u_s, *v_s));

}

/* inline QSORT() comparison routine */
#define inline_qs_cmpf(a,b) (strcmp((*a),(*b)) < 0)

/* comparison routine for STL sort */
class compare_str_cmpf {
    public:
        bool operator() (char *u, char *v) {

            int cmpval = strcmp(u, v);

            if (cmpval < 0)
                return true;
            else
                return false;
        }
};

int find_uniq_qsort(char *str_array, const int str_array_size, 
        const int num_strings, const int num_iterations) {

    fprintf(stderr, "N %d\n", num_strings);
    fprintf(stderr, "Using C qsort\n");
    fprintf(stderr, "Execution times (ms) for %d iterations:\n", num_iterations);

    int iter;
    double avg_elt;

    char **B;
    B = (char **) malloc(num_strings * sizeof(char *));
    assert(B != NULL);

    int *counts;
    counts = (int *) malloc(num_strings * sizeof(int));

    avg_elt = 0.0;

    for (iter = 0; iter < num_iterations; iter++) {
        
        int i, j;

        B[0] = &str_array[0];
        j = 1;
        for (i=0; i<str_array_size-1; i++) {
            if (str_array[i] == '\0') {
                B[j] = &str_array[i+1];
                j++;    
            }
        }
        assert(j == num_strings);

        for (i=0; i<num_strings; i++) {
            counts[i] = 0;
        }

        double elt;
        elt = timer();

        qsort(B, num_strings, sizeof(char *), qs_cmpf);

        /*
        for (i=0; i<num_strings; i++) {
            fprintf(stderr, "%s\n", B[i]);
        }
        */

        /* determine number of unique strings 
           and count of each */
        int num_uniq_strings = 1;
        int string_occurrence_count = 1;
        for (i=1; i<num_strings; i++) {
            if (strcmp(B[i], B[i-1]) != 0) {
                num_uniq_strings++;
                counts[i-1] = string_occurrence_count;
                string_occurrence_count = 1;
            } else {
                string_occurrence_count++;
            }
        }
        counts[num_strings-1] = string_occurrence_count;

        elt = timer() - elt;
        avg_elt += elt;
        fprintf(stderr, "%9.3lf\n", elt*1e3);


        /* optionally print out unique strings */
        /*
        for (i=0; i<num_strings; i++) {
            if (counts[i] != 0) {
                fprintf(stderr, "%s\t%d\n", B[i], counts[i]);
            }
        }
        fprintf(stderr, "Number of unique strings: %d\n", num_uniq_strings);
        */

        /* an incomplete correctness check */
        int total_strings = counts[0];
        for (i=1; i<num_strings; i++) {
            assert(strcmp(B[i], B[i-1]) >= 0);
            total_strings += counts[i];
        }
        assert(total_strings == num_strings);

    }

    avg_elt = avg_elt/num_iterations;
    
    free(B);
    free(counts);

    fprintf(stderr, "Average time: %9.3lf ms.\n", avg_elt*1e3);
    fprintf(stderr, "Average sort rate: %6.3lf MB/s\n", str_array_size/(avg_elt*1e6));
    return 0;

}

int find_uniq_inline_qsort(char *str_array, const int str_array_size, 
        const int num_strings, const int num_iterations) {

    fprintf(stderr, "N %d\n", num_strings);
    fprintf(stderr, "Using inline qsort\n");
    fprintf(stderr, "Execution times (ms) for %d iterations:\n", num_iterations);

    int iter;
    double avg_elt;

    char **B;
    B = (char **) malloc(num_strings * sizeof(char *));
    assert(B != NULL);

    int *counts;
    counts = (int *) malloc(num_strings * sizeof(int));

    avg_elt = 0.0;

    for (iter = 0; iter < num_iterations; iter++) {
        
        int i, j;

        B[0] = &str_array[0];
        j = 1;
        for (i=0; i<str_array_size-1; i++) {
            if (str_array[i] == '\0') {
                B[j] = &str_array[i+1];
                j++;    
            }
        }
        assert(j == num_strings);

        for (i=0; i<num_strings; i++) {
            counts[i] = 0;
        }

        double elt;
        elt = timer();

        QSORT(char*, B, num_strings, inline_qs_cmpf);

        /*
        for (i=0; i<num_strings; i++) {
            fprintf(stderr, "%s\n", B[i]);
        }
        */

        /* determine number of unique strings 
           and count of each string */
        int num_uniq_strings = 1;
        int string_occurrence_count = 1;
        for (i=1; i<num_strings; i++) {
            if (strcmp(B[i], B[i-1]) != 0) {
                num_uniq_strings++;
                counts[i-1] = string_occurrence_count;
                string_occurrence_count = 1;
            } else {
                string_occurrence_count++;
            }
        }
        counts[num_strings-1] = string_occurrence_count;

        /* optionally print out unique strings */
        /*
        for (i=0; i<num_strings; i++) {
            if (counts[i] != 0) {
                fprintf(stderr, "%s\t%d\n", B[i], counts[i]);
            }
        }
        fprintf(stderr, "Number of unique strings: %d\n", num_uniq_strings);
        */

        elt = timer() - elt;
        avg_elt += elt;
        fprintf(stderr, "%9.3lf\n", elt*1e3);

        /* an incomplete correctness check */
        int total_strings = counts[0];
        for (i=1; i<num_strings; i++) {
            assert(strcmp(B[i], B[i-1]) >= 0);
            total_strings += counts[i];
        }
        assert(total_strings == num_strings);

    }

    avg_elt = avg_elt/num_iterations;
    
    free(B);
    free(counts);

    fprintf(stderr, "Average time: %9.3lf ms.\n", avg_elt*1e3);
    fprintf(stderr, "Average sort rate: %6.3lf MB/s\n", str_array_size/(avg_elt*1e6));


    return 0;

}

int find_uniq_stl_sort(char *str_array, const int str_array_size,
        const int num_strings, const int num_iterations) {

    fprintf(stderr, "N %d\n", num_strings);
    fprintf(stderr, "Using STL sort\n");
    fprintf(stderr, "Execution times (ms) for %d iterations:\n", num_iterations);

    int iter;
    double avg_elt;

    char **B;
    B = (char **) malloc(num_strings * sizeof(char *));
    assert(B != NULL);

    int *counts;
    counts = (int *) malloc(num_strings * sizeof(int));

    avg_elt = 0.0;

    for (iter = 0; iter < num_iterations; iter++) {
        
        int i, j;

        B[0] = &str_array[0];
        j = 1;
        for (i=0; i<str_array_size-1; i++) {
            if (str_array[i] == '\0') {
                B[j] = &str_array[i+1];
                j++;    
            }
        }
        assert(j == num_strings);

        for (i=0; i<num_strings; i++) {
            counts[i] = 0;
        }

        double elt;
        elt = timer();

        compare_str_cmpf cmpf;
        std::sort(B, B+num_strings, cmpf);

        /*
        for (i=0; i<num_strings; i++) {
            fprintf(stderr, "%s\n", B[i]);
        }
        */

        /* determine number of unique strings 
           and count of each string */
        int num_uniq_strings = 1;
        int string_occurrence_count = 1;
        for (i=1; i<num_strings; i++) {
            if (strcmp(B[i], B[i-1]) != 0) {
                num_uniq_strings++;
                counts[i-1] = string_occurrence_count;
                string_occurrence_count = 1;
            } else {
                string_occurrence_count++;
            }
        }
        counts[num_strings-1] = string_occurrence_count;

        /* optionally print out unique strings */
        /*
        for (i=0; i<num_strings; i++) {
            if (counts[i] != 0) {
                fprintf(stderr, "%s\t%d\n", B[i], counts[i]);
            }
        }
        fprintf(stderr, "Number of unique strings: %d\n", num_uniq_strings);
        */

        elt = timer() - elt;
        avg_elt += elt;
        fprintf(stderr, "%9.3lf\n", elt*1e3);

        /* an incomplete correctness check */
        int total_strings = counts[0];
        for (i=1; i<num_strings; i++) {
            assert(strcmp(B[i], B[i-1]) >= 0);
            total_strings += counts[i];
        }
        assert(total_strings == num_strings);

    }

    avg_elt = avg_elt/num_iterations;
    
    free(B);
    free(counts);

    fprintf(stderr, "Average time: %9.3lf ms.\n", avg_elt*1e3);
    fprintf(stderr, "Average sort rate: %6.3lf MB/s\n", str_array_size/(avg_elt*1e6));

    return 0;

}

int find_uniq_stl_map(char *str_array, int str_array_size, const int num_strings, const int num_iterations) {

    fprintf(stderr, "N %d\n", num_strings);
    fprintf(stderr, "Using a map\n");
    fprintf(stderr, "Execution times (ms) for %d iterations:\n", num_iterations);

    int iter;
    double avg_elt;

    char **B;
    B = (char **) malloc(num_strings * sizeof(char *));
    assert(B != NULL);

    int *counts;
    counts = (int *) malloc(num_strings * sizeof(int));

    avg_elt = 0.0;

    for (iter = 0; iter < num_iterations; iter++) {
        
        int i, j;

        B[0] = &str_array[0];
        j = 1;
        for (i=0; i<str_array_size-1; i++) {
            if (str_array[i] == '\0') {
                B[j] = &str_array[i+1];
                j++;    
            }
        }
        assert(j == num_strings);

        for (i=0; i<num_strings; i++) {
            counts[i] = 0;
        }

        double elt;
        elt = timer();

        std::map<std::string, int> str_map;

        for (i=0; i<num_strings; i++) {
            std::string curr_str(B[i]);
            //curr_str.assign(B[i], strlen(B[i]));
            str_map[curr_str]++;
        }

        fprintf(stderr, "Number of unique strings: %d\n", 
                ((int) str_map.size()));

        elt = timer() - elt;
        avg_elt += elt;
        fprintf(stderr, "%9.3lf\n", elt*1e3);

    }

    avg_elt = avg_elt/num_iterations;
    
    free(B);
    free(counts);

    fprintf(stderr, "Average time: %9.3lf ms.\n", avg_elt*1e3);
    fprintf(stderr, "Average sort rate: %6.3lf MB/s\n", str_array_size/(avg_elt*1e6));

    return 0;


    return 0;

}

static int binary_search(char* value, char** a, int left, int right )
{
    long low  = left;
    long high;
    if (left > right + 1) {
        high = left;
    } else {
        high = right + 1;
    }
    while( low < high )
    {
        long mid = ( low + high ) / 2;
        if ( strcmp(value, a[mid]) <= 0 ) high = mid;
        else                        low  = mid + 1; // because we compared to a[mid] and the value was larger than a[mid].
                                                    // Thus, the next array element to the right from mid is the next possible
                                                    // candidate for low, and a[mid] can not possibly be that candidate.
    }
    return high;
}

static void exchange( int* A, int* B )
{
    int t = *A;
    *A = *B;
    *B = t;
}

static void merge_ptr( char** a_start, char** a_end, char** b_start, char** b_end, char** dst )
{
    while( a_start < a_end && b_start < b_end ) {
        if ( strcmp(*a_start, *b_start) <= 0 )  *dst++ = *a_start++;    // if elements are equal, then a[] element is output
        else                                    *dst++ = *b_start++;
    }
    while( a_start < a_end ) *dst++ = *a_start++;
    while( b_start < b_end ) *dst++ = *b_start++;
}

static void merge_parallel(char** t, int p1, int r1, int p2, int r2, char** a, int p3 )
{
    int length1 = r1 - p1 + 1;
    int length2 = r2 - p2 + 1;
    if ( length1 < length2 ) {
        exchange(      &p1,      &p2 );
        exchange(      &r1,      &r2 );
        exchange( &length1, &length2 );
    }
    if ( length1 == 0 ) return;
    if (( length1 + length2 ) <= 16 )
        merge_ptr( &t[ p1 ], &t[ p1 + length1 ], &t[ p2 ], &t[ p2 + length2 ], &a[ p3 ] );
    else {
        int q1 = ( p1 + r1 ) / 2;
        int q2 = binary_search( t[ q1 ], t, p2, r2 );
        int q3 = p3 + ( q1 - p1 ) + ( q2 - p2 );
        a[ q3 ] = t[ q1 ];
        #pragma omp parallel sections
        {
            #pragma omp section
            { merge_parallel( t, p1,     q1 - 1, p2, q2 - 1, a, p3     ); }
            #pragma omp section
            { merge_parallel( t, q1 + 1, r1,     q2, r2,     a, q3 + 1 ); }
        }
    }
}

static void merge_sort_split(char** src, int l, int r, char** dst, bool srcToDst) {

	if (r == l) {    // termination/base case of sorting a single element
        if (srcToDst)  dst[ l ] = src[ l ];    // copy the single element from src to dst
        return;
    }
    int m = ( r + l ) / 2;

    #pragma omp parallel sections
    {
        #pragma omp section
    	{ merge_sort_split( src, l,     m, dst, !srcToDst ); } // reverse direction of srcToDst for the next level of recursion
        #pragma omp section 
        { merge_sort_split( src, m + 1, r, dst, !srcToDst ); } // reverse direction of srcToDst for the next level of recursion
    }
    if ( srcToDst ) merge_parallel( src, l, m, m + 1, r, dst, l );
    else            merge_parallel( dst, l, m, m + 1, r, src, l );
}

int find_uniq_parallel_merge_sort(char *str_array, const int str_array_size,
        const int num_strings, const int num_iterations) {

    fprintf(stderr, "N %d\n", num_strings);
    fprintf(stderr, "Using parallel merge sort\n");
    fprintf(stderr, "Execution times (ms) for %d iterations:\n", num_iterations);

    int iter;
    double avg_elt;

    char **src;
    src = (char **) malloc(num_strings * sizeof(char *));
    assert(src != NULL);
    
    char **dest;
    dest = (char **) malloc(num_strings * sizeof(char *));
    assert(dest != NULL);

    int *counts;
    counts = (int *) malloc(num_strings * sizeof(int));

    avg_elt = 0.0;

    for (iter = 0; iter < num_iterations; iter++) {
        
        int i, j;

        src[0] = &str_array[0];
        j = 1;
        for (i=0; i<str_array_size-1; i++) {
            if (str_array[i] == '\0') {
                src[j] = &str_array[i+1];
                j++;    
            }
        }
        assert(j == num_strings);

        for (i=0; i<num_strings; i++) {
            counts[i] = 0;
        }

        double elt;
        elt = timer();

        /* Parallel merge sort */
        merge_sort_split (src, 0, num_strings-1, dest, true);

        /* determine number of unique strings 
           and count of each string */
        int num_uniq_strings = 1;
        int string_occurrence_count = 1;
        for (i=1; i<num_strings; i++) {
            if (strcmp(dest[i], dest[i-1]) != 0) {
                num_uniq_strings++;
                counts[i-1] = string_occurrence_count;
                string_occurrence_count = 1;
            } else {
                string_occurrence_count++;
            }
        }
        counts[num_strings-1] = string_occurrence_count;

        /* optionally print out unique strings */
        /*
        for (i=0; i<num_strings; i++) {
            if (counts[i] != 0) {
                fprintf(stderr, "%s\t%d\n", B[i], counts[i]);
            }
        }
        fprintf(stderr, "Number of unique strings: %d\n", num_uniq_strings);
        */

        elt = timer() - elt;
        avg_elt += elt;
        fprintf(stderr, "%9.3lf\n", elt*1e3);

        /* an incomplete correctness check */
        int total_strings = counts[0];
        for (i=1; i<num_strings; i++) {
            assert(strcmp(dest[i], dest[i-1]) >= 0);
            total_strings += counts[i];
        }
        assert(total_strings == num_strings);

    }

    avg_elt = avg_elt/num_iterations;
    
    free(src);
    free(dest);
    free(counts);

    fprintf(stderr, "Average time: %9.3lf ms.\n", avg_elt*1e3);
    fprintf(stderr, "Average sort rate: %6.3lf MB/s\n", str_array_size/(avg_elt*1e6));

    return 0;

}

int main(int argc, char **argv) {

    if (argc != 4) {
        fprintf(stderr, "%s <input file> <n> <alg_type>\n", argv[0]);
        fprintf(stderr, "alg_type 0: use C qsort, then find unique strings\n");
        fprintf(stderr, "         1: use inline qsort, then find unique strings\n");
        fprintf(stderr, "         2: use STL sort, then find unique strings\n");
        fprintf(stderr, "         3: use STL map\n");
        fprintf(stderr, "         4: use parallel merge sort\n");
        exit(1);
    }

    char *filename = argv[1];
    
    int num_strings;
    num_strings = atoi(argv[2]);

    /* get file size */
    struct stat file_stat;
    stat(filename, &file_stat);
    int file_size_bytes = file_stat.st_size;
    fprintf(stderr, "File size: %d bytes\n", file_size_bytes);

    /* load all strings from file */
    FILE *infp;
    infp = fopen(filename, "r");
    if (infp == NULL) {
        fprintf(stderr, "Error: Couldn't open file!\n");
        exit(2);
    }
    
    char *str_array = (char *) malloc(file_size_bytes * sizeof(char));
    fread(str_array, sizeof(char), file_size_bytes, infp);
    fclose(infp);

    /* replace end of line characters with string delimiters */
    int i;
    int num_strings_in_file = 0;
    for (i=0; i<file_size_bytes; i++) {
        if (str_array[i] == '\n') {
            str_array[i] = '\0';
            num_strings_in_file++;
        }
    }
    fprintf(stderr, "num strings read %d\n", num_strings_in_file);
    assert(num_strings == num_strings_in_file);

    int alg_type = atoi(argv[3]);
    assert((alg_type >= 0) && (alg_type <= 4));
    
    int num_iterations = 10;

    if (alg_type == 0) {
        find_uniq_qsort(str_array, file_size_bytes, num_strings, num_iterations);
    } else if (alg_type == 1) {
        find_uniq_inline_qsort(str_array, file_size_bytes, num_strings, num_iterations);
    } else if (alg_type == 2) {
        find_uniq_stl_sort(str_array, file_size_bytes, num_strings, num_iterations);
    } else if (alg_type == 3) {
        find_uniq_stl_map(str_array, file_size_bytes, num_strings, num_iterations);
    } else if (alg_type == 4) {
        find_uniq_parallel_merge_sort(str_array, file_size_bytes, num_strings, num_iterations);
    }

    free(str_array);

    return 0;
}
