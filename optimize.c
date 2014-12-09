#include"optimize.h"
#include<stdlib.h>
#include<math.h>


static inline void copy( double* dest, const double* src, size_t n ){
    size_t i;
    for( i = 0 ; i < n ; ++i ){
        dest[i] = src[i];
    }
}

double patternSearch( double(*fnc)( const double*, unsigned long ), const double* v0, double* result, unsigned long n, const double* upper_bound, const double* lower_bound, FILE* report ){
    
    double MESH_TOL = 1e-6;
    double FNC_TOL = 1e-6;
    double MESH_SCALE = 2;
    double mesh_size = 1;

    double cur[n];
    copy( cur, v0, n );

    //check bounds
    int within_bounds = 1;
    if( upper_bound != NULL ){
        size_t i;
        for( i = 0 ; i < n ; ++i ){
            if( cur[i] > upper_bound[i] ){
                cur[i] = upper_bound[i];
                within_bounds = 0;
            }
        }
    }
    if( lower_bound != NULL ){
        size_t i;
        for( i = 0 ; i < n ; ++i ){
            if( cur[i] < lower_bound[i] ){
                cur[i] = lower_bound[i];
                within_bounds = 0;
            }
        }
    }
    
    if( report != NULL && !within_bounds ){
        fprintf( report, "Initial values not within specified bounds. Adjusting to within bounds.\n" );
    }

    size_t iters = 0;
    if( report != NULL ){
        fprintf( report, "Iters\tFeval\tMeshSize\tAction\n" );
    }
 
    double feval_best = fnc( cur, n );
    if( report != NULL ){
        fprintf( report, "%ld\t%lf\t%lf\tInitializing\n", iters, feval_best, mesh_size );
    }
    double best[n];
    copy( best, cur, n );
    double point[n];
    int reduce;
    double feval_cur;
    do{    
        reduce = 1;
        feval_cur = feval_best;
        copy( point, cur, n );
        size_t i;
        //search points
        for( i = 0 ; i < n ; ++i ){
            if( upper_bound == NULL || cur[i] + mesh_size <= upper_bound[i] ){
                point[i] += mesh_size;
                double feval = fnc( point, n );
                //fprintf( stdout, "%lf, %lf\n", feval, feval_best );
                if( feval < feval_best ){
                    copy( best, point, n );
                    feval_best = feval;
                    if( fabs( feval_best - feval_cur ) >= FNC_TOL ){
                        reduce = 0;
                    }
                }
                //restore point
                point[i] = cur[i];
            }
            if( lower_bound == NULL || cur[i] - mesh_size >= lower_bound[i] ){
                point[i] -= mesh_size;
                double feval = fnc( point, n );
                //fprintf( stdout, "%lf, %lf\n", feval, feval_best );
                if( feval < feval_best ){
                    copy( best, point, n );
                    feval_best = feval;
                    if( fabs( feval_best - feval_cur ) >= FNC_TOL ){
                        reduce = 0;
                    }
                }
                //restore point
                point[i] = cur[i];
            }
        }
        //update location
        if( reduce ){
            //shrink the mesh
            mesh_size = mesh_size / MESH_SCALE;
        }else{
            //update cur to best and expand mesh
            copy( cur, best, n );
            mesh_size = mesh_size * MESH_SCALE;
        }
        if( report != NULL ){
            fprintf( report, "%ld\t%lf\t%lf\t%s\n",
                ++iters,
                feval_best,
                mesh_size,
                reduce ? "Reducing" : "Polling" );
        }
    
    }while( mesh_size >= MESH_TOL || fabs( feval_best - feval_cur ) >= FNC_TOL );

    copy( result, cur, n );
    return feval_best;

}




