#ifndef INCLUDED_OPTIMIZE__H
#define INCLUDED_OPTIMIZE__H

#include<stdio.h>

double patternSearch( double(*fnc)( const double*, unsigned long), const double* v0, double* result, unsigned long n, const double* upper_bound, const double* lower_bound, FILE* report );


#endif


