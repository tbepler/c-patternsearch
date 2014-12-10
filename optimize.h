#ifndef INCLUDED_OPTIMIZE__H
#define INCLUDED_OPTIMIZE__H

#include<stdio.h>

typedef struct PatternSearchOpts{

    FILE* report;
    double* upper_bound;
    double* lower_bound;
    double* scale_factor;
    double mesh_tol;
    double fnc_tol;
    double mesh_scale;
    double mesh_size_init;
    unsigned long max_iters;

} PatternSearchOpts;

const PatternSearchOpts DEFAULT_PATTERN_SEARCH_OPTS;

double patternSearch( double(*fnc)( const double*, unsigned long, void*), const double* v0, double* result, unsigned long n, void* fnc_param, const PatternSearchOpts* opts );


#endif


