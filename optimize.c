#include"optimize.h"
#include<stdlib.h>
#include<math.h>

#define DEFAULT_SCALE 1e-4

static inline void copy( double* dest, const double* src, size_t n ){
    size_t i;
    for( i = 0 ; i < n ; ++i ){
        dest[i] = src[i];
    }
}

const PatternSearchOpts DEFAULT_PATTERN_SEARCH_OPTS = {

    NULL,
    NULL,
    NULL,
    NULL,
    1e-6,
    1e-6,
    2,
    1,
    -1

};

static inline void calculateScaleFactor( double* scale_factor, double* lower_bound, double* upper_bound, size_t n ){

    size_t i;
    
    if( lower_bound == NULL || upper_bound == NULL ){
        
        for( i = 0 ; i < n ; ++i ){
            scale_factor[i] = 1;
        }

    }else{

        double min;
        double max;
        for( i = 0 ; i < n ; ++i ){
            min = lower_bound[i];
            max = upper_bound[i];
            if( isfinite( min ) && isfinite( max ) ){
                scale_factor[i] = ( max - min ) * DEFAULT_SCALE;
            }else{
                scale_factor[i] = 1;
            }
        }
    }

}

double patternSearch( double(*fnc)( const double*, unsigned long, void* ), const double* v0, double* result, unsigned long n, void* fnc_param, const PatternSearchOpts* opts ){
    
    if( opts == NULL ){
        opts = &DEFAULT_PATTERN_SEARCH_OPTS;
    }
    
    FILE* report = opts->report;
    double* upper_bound = opts->upper_bound;
    double* lower_bound = opts->lower_bound;
    double* scale_factor = opts->scale_factor;
    double mesh_tol = opts->mesh_tol;
    double fnc_tol = opts->fnc_tol;
    double mesh_scale = opts->mesh_scale;
    double mesh_size = opts->mesh_size_init;
    size_t max_iters = opts->max_iters;

    //check and intialize scale factor if null
    double scale[n];
    if( scale_factor == NULL ){
        calculateScaleFactor( scale, lower_bound, upper_bound, n );
        scale_factor = scale;
    }

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
 
    double feval_best = fnc( cur, n, fnc_param );
    if( report != NULL ){
        fprintf( report, "%ld\t%lf\t%lf\tInitializing\n", iters, feval_best, mesh_size );
    }
    double best[n];
    copy( best, cur, n );
    double point[n];
    int reduce;
    double feval_cur;
    double scaled_mesh;
    do{    
        reduce = 1;
        feval_cur = feval_best;
        copy( point, cur, n );
        size_t i;
        //search points
        for( i = 0 ; i < n ; ++i ){
            //check whether parameter can be skipped
            if( scale_factor[i] == 0 || ( upper_bound != NULL && lower_bound != NULL && upper_bound[i] == lower_bound[i] ) ){
                continue;
            }
            //scale mesh by the scale factor
            scaled_mesh = mesh_size * scale_factor[i];
            //score + direction
            if( upper_bound == NULL || cur[i] + scaled_mesh <= upper_bound[i] ){
                point[i] += scaled_mesh;
                double feval = fnc( point, n, fnc_param );
                //fprintf( stdout, "%lf, %lf\n", feval, feval_best );
                if( feval < feval_best ){
                    copy( best, point, n );
                    feval_best = feval;
                    if( fabs( feval_best - feval_cur ) >= fnc_tol ){
                        reduce = 0;
                    }
                }
                //restore point
                point[i] = cur[i];
            }
            //score - direction
            if( lower_bound == NULL || cur[i] - scaled_mesh >= lower_bound[i] ){
                point[i] -= scaled_mesh;
                double feval = fnc( point, n, fnc_param );
                //fprintf( stdout, "%lf, %lf\n", feval, feval_best );
                if( feval < feval_best ){
                    copy( best, point, n );
                    feval_best = feval;
                    if( fabs( feval_best - feval_cur ) >= fnc_tol ){
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
            mesh_size = mesh_size / mesh_scale;
        }else{
            //update cur to best and expand mesh
            copy( cur, best, n );
            mesh_size = mesh_size * mesh_scale;
        }
        
        if( report != NULL ){
            fprintf( report, "%ld\t%lf\t%lf\t%s\n",
                ++iters,
                feval_best,
                mesh_size,
                reduce ? "Reducing" : "Polling" );
        }else if( max_iters > 0 ){
            ++iters;
        }
    
    }while( ( mesh_size >= mesh_tol || fabs( feval_best - feval_cur ) >= fnc_tol )  && ( max_iters <= 0 || iters < max_iters ) );

    copy( result, cur, n );
    return feval_best;

}




