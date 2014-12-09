#include"optimize.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

double quadratic( const double* arr, size_t n ){
    double sum = 0;
    size_t i;
    for( i = 0 ; i < n ; ++i ){
        sum += pow( arr[i], 2 );
    }
    return sum;
}

int main(){
        
    double v0[] = { 4, 1, 2, 10000, -1238, 19, 149149.1494181, 149 };
    double ub[] = { 10, 5, 10, 10001, 2, 25, 15000, 1000000000 };
    double lb[] = { 7, -13, -149, 5, -1499, 10, 100, -48810401 };
    size_t n = 8;

    double optim[n];

    double eval = patternSearch( &quadratic, v0, optim, n, ub, lb, stdout );

    printf( "Optimum: " );
    size_t i;
    for( i = 0 ; i < n - 1 ; ++i ){
        printf( "%lf, ", optim[i] );
    }
    printf( "%lf\n", optim[n-1] );
    printf( "Evaluation: %lf\n", eval );

    return 0;

}
