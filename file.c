#include"file.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

static const float FACTOR = 1.5;
static const size_t INIT = 10;

inline void append( char c, char** p_str, size_t i, size_t* p_n ){
    
    char* str = *p_str;
    size_t n = *p_n;
    //check that enough capacity remains in str
    if( i >= n ){
        //realloc str with more capacity
        n = n < INIT ? INIT : FACTOR * n + 1; //incr by at least 1
        *p_n = n;
        str = (char*) realloc( str, n * sizeof( char ) );
        *p_str = str;
    }
    
    str[i] = c;

}

char* readLine( FILE* f ){

    size_t n = 0;
    char* str = NULL;
    size_t i = 0;

    char c;
    while( ( c = (char) fgetc( f ) ) != '\n' && c != EOF ){
        append( c, &str, i++, &n );
    }

    if( str != NULL ){
        //null terminate str
        append( '\0', &str, i, &n );
    }

    return str;

}
