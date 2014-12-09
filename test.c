#include"file.h"
#include"array.h"
#include<stdio.h>
#include<stdlib.h>


int main( int argc, char* argv[] ){

    FILE* f;
    char* line;
    while( argc-- > 1 )
        f = fopen( argv[argc], "r" );
        while( ( line = readLine( f ) ) != NULL ){
            fprintf( stdout, "%s\n", line );
            //array of tokens
            fprintf( stdout, "Tokens: " );
            size_t n;
            size_t i;
            char** tokens = split( line, " \t", &n );
            for( i = 0 ; i < n ; ++i ){
                if( i != n - 1 ){
                    fprintf( stdout, "%s, ", tokens[i] );
                }else{
                    fprintf( stdout, "%s", tokens[i] );
                }
                //free token
                free( tokens[i] );
            }
            free( tokens );
            fprintf( stdout, "\n" );
            //as double
            fprintf( stdout, "As double array: " );
            double* vec = parseDoubles( line, " \t", &n );
            for( i = 0 ; i < n ; ++i ){
                fprintf( stdout, "%lf ", vec[i] );
            }
            fprintf( stdout, "\n" );
            free( vec );
            free( line );
        }

}
