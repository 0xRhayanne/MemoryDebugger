#ifndef MEMDEBUG_H
#define MEMDEBUG_H


#include <stdio.h>

typedef enum { ENGLISH, PORTUGUESE } Language;
typedef enum { QUIET, NORMAL, DEBUG } Verbosity;

// Settings
void md_set_language(Language l);
void md_set_verbosity(Verbosity v);
void md_enable_file_logging(int enable);

// Wrappers
void* md_malloc(size_t size, const char* file, int line);
void* md_calloc(size_t nmemb, size_t size, const char* file, int line);
void* md_realloc(void* ptr, size_t size, const char* file, int line);
void md_free(void* ptr, const char* file, int line);

// Reporting
void md_report(void);

#endif
