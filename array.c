#include"array.h"
#include<stdlib.h>
#include<string.h>

static const float FACTOR = 1.5;
static const size_t INIT = 10;

static inline void appendDouble( double d, double** p_arr, size_t i, size_t* p_n ){
    
    double* arr = *p_arr;
    size_t n = *p_n;
    //check that enough capacity remains in arr
    if( i >= n ){
        //realloc arr with more capacity
        n = n < INIT ? INIT : FACTOR * n + 1; //incr by at least 1
        *p_n = n;
        arr = (double*) realloc( arr, n * sizeof( double ) );
        *p_arr = arr;
    }
    
    arr[i] = d;
}

static inline void appendStr( char* str, char*** p_arr, size_t i, size_t* p_n ){
    
    char** arr = *p_arr;
    size_t n = *p_n;
    //check that enough capacity remains in arr
    if( i >= n ){
        //realloc arr with more capacity
        n = n < INIT ? INIT : FACTOR * n + 1; //incr by at least 1
        *p_n = n;
        arr = (char**) realloc( arr, n * sizeof( char* ) );
        *p_arr = arr;
    }
    
    arr[i] = str;
}

static inline size_t len( const char* str ){
        
    size_t i = 0;
    if( str == NULL ) return i;
    while( str[i++] != '\0' ) {}
    return i;

}

static inline char* copy( const char* str ){

    size_t l = len( str );
    char* cpy = malloc( l * sizeof( char ) );
    strcpy( cpy, str );
    return cpy;

}

double* parseDoubles( const char* str, const char* delims, unsigned long* p_n ){
    char* cpy = copy( str );

    size_t n = 0;
    size_t i = 0;
    double* arr = NULL;

    char* token = strtok( cpy, delims );
    while( token != NULL ){
        appendDouble( atof( token ), &arr, i++, &n );
        token = strtok( NULL, delims );
    }

    free( cpy );

    *p_n = i;
    return arr;

}

char** split( const char* str, const char* delims, unsigned long* p_n ){
    
    char* cpy = copy( str );

    size_t n = 0;
    size_t i = 0;
    char** arr = NULL;

    char* token = strtok( cpy, delims );
    while( token != NULL ){
        
        char* entry = copy( token ); //copy entry to prevent mishaps when cpy is free'd
        appendStr( entry, &arr, i++, &n );
        token = strtok( NULL, delims );

    }

    free( cpy );

    *p_n = i;
    return arr;


}


