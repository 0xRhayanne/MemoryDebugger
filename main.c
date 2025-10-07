#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "memdebug.h"

// Only in main.c
#define malloc(sz) md_malloc(sz,__FILE__,__LINE__)
#define calloc(n,sz) md_calloc(n,sz,__FILE__,__LINE__)
#define realloc(p,sz) md_realloc(p,sz,__FILE__,__LINE__)
#define free(p) md_free(p,__FILE__,__LINE__)

int main(){
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    md_set_language(PORTUGUESE);
    md_set_verbosity(DEBUG);
    md_enable_file_logging(1);

    int* a = malloc(40);
    free(a);

    char* b = malloc(50); // leak

    int* c = malloc(80);
    free(c);
    free(c); // double free

    char* d = calloc(15,1);
    d = realloc(d,100);
    free(d);

    md_report();

    return 0;
}
