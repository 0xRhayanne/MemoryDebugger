#include "memdebug.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _WIN32
static CRITICAL_SECTION mem_mutex;
__attribute__((constructor)) static void init_mutex(void){
    InitializeCriticalSection(&mem_mutex);
}
#define LOCK() EnterCriticalSection(&mem_mutex)
#define UNLOCK() LeaveCriticalSection(&mem_mutex)
#else
#endif

#define GUARD_SIZE 16
#define GUARD_PATTERN 0xAB

// ANSI colors
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_CYAN    "\033[36m"

typedef struct MemBlock {
    void* base_ptr;
    void* user_ptr;
    size_t size;
    const char* file;
    int line;
    struct MemBlock* next;
} MemBlock;

static MemBlock* head = NULL;
static Language lang = ENGLISH;
static Verbosity verbosity = NORMAL;
static int log_to_file = 0;
static FILE* log_file = NULL;

// Memory stats
static size_t total_allocations = 0;
static size_t total_frees = 0;
static size_t total_reallocs = 0;
static size_t total_allocated_bytes = 0;
static size_t peak_allocated_bytes = 0;
static size_t largest_allocation = 0;

// Timestamp
static void timestamp(char* buf, size_t size){
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(buf,size,"%Y-%m-%d %H:%M:%S",t);
}

// Logging
static void open_log_file() {
    if(log_to_file && !log_file){
        log_file = fopen("memdebug_report.log","a");
        if(log_file){
            char buf[64]; timestamp(buf,sizeof(buf));
            fprintf(log_file,"\n=== Session started at %s ===\n",buf);
        }
    }
}

static void log_message(const char* msg, const char* color){
    printf("%s%s%s",color,msg,COLOR_RESET);
    if(log_file){
        fprintf(log_file,"%s",msg);
        fflush(log_file);
    }
}

// Guard helpers
static void fill_guard(unsigned char* g){
    for(int i=0;i<GUARD_SIZE;i++) g[i]=GUARD_PATTERN;
}

static int check_guard(unsigned char* g){
    for(int i=0;i<GUARD_SIZE;i++)
        if(g[i]!=GUARD_PATTERN) return 0;
    return 1;
}

// Add/Remove block
static void add_block(void* base_ptr, void* user_ptr, size_t size, const char* file, int line){
    MemBlock* block = (MemBlock*)malloc(sizeof(MemBlock));
    if(!block) return;
    block->base_ptr=base_ptr;
    block->user_ptr=user_ptr;
    block->size=size;
    block->file=file;
    block->line=line;

    LOCK();
    block->next=head;
    head=block;

    total_allocations++;
    total_allocated_bytes+=size;
    if(total_allocated_bytes>peak_allocated_bytes) peak_allocated_bytes=total_allocated_bytes;
    if(size>largest_allocation) largest_allocation=size;
    UNLOCK();
}

static int remove_block(void* user_ptr){
    LOCK();
    MemBlock* cur=head;
    MemBlock* prev=NULL;
    while(cur){
        if(cur->user_ptr==user_ptr){
            if(prev) prev->next=cur->next;
            else head=cur->next;
            total_frees++;
            total_allocated_bytes-=cur->size;
            free(cur);
            UNLOCK();
            return 1;
        }
        prev=cur;
        cur=cur->next;
    }
    UNLOCK();
    return 0;
}

// Wrappers
void* md_malloc(size_t size, const char* file, int line){
    open_log_file();
    size_t total_size = GUARD_SIZE + size + GUARD_SIZE;
    unsigned char* base = (unsigned char*)malloc(total_size);
    if(!base) return NULL;

    fill_guard(base);
    fill_guard(base + GUARD_SIZE + size);
    void* user_ptr = base + GUARD_SIZE;

    add_block(base,user_ptr,size,file,line);

    if(verbosity>=DEBUG){
        char buf[256], ts[64]; timestamp(ts,sizeof(ts));
        snprintf(buf,sizeof(buf),"[%s] [DEBUG] malloc %zu bytes at %p (%s:%d)\n",ts,size,user_ptr,file,line);
        log_message(buf,COLOR_CYAN);
    }

    return user_ptr;
}

void* md_calloc(size_t nmemb, size_t size, const char* file, int line){
    void* ptr = md_malloc(nmemb*size,file,line);
    if(ptr) memset(ptr,0,nmemb*size);
    return ptr;
}

void* md_realloc(void* user_ptr, size_t size, const char* file, int line){
    if(!user_ptr) return md_malloc(size,file,line);

    MemBlock* cur=head;
    void* base=NULL;
    while(cur){ if(cur->user_ptr==user_ptr){ base=cur->base_ptr; break; } cur=cur->next; }
    if(!base) return md_malloc(size,file,line);

    if(!remove_block(user_ptr)) return NULL;

    unsigned char* base_new=(unsigned char*)realloc(base,GUARD_SIZE+size+GUARD_SIZE);
    if(!base_new) return NULL;

    fill_guard(base_new);
    fill_guard(base_new + GUARD_SIZE + size);
    void* user_new = base_new + GUARD_SIZE;
    add_block(base_new,user_new,size,file,line);

    total_reallocs++;
    return user_new;
}

void md_free(void* user_ptr, const char* file, int line){
    if(!user_ptr) return;

    MemBlock* cur=head;
    void* base=NULL;
    size_t size=0;
    while(cur){ if(cur->user_ptr==user_ptr){ base=cur->base_ptr; size=cur->size; break; } cur=cur->next; }

    if(!base){
        char buf[256], ts[64]; timestamp(ts,sizeof(ts));
        snprintf(buf,sizeof(buf),"[%s] [Aviso] Dupla liberação ou free inválido %p\n",ts,user_ptr);
        log_message(buf,COLOR_YELLOW);
        return;
    }

    if(!remove_block(user_ptr)) return;

    unsigned char* pre = (unsigned char*)base;
    unsigned char* post = ((unsigned char*)user_ptr) + size;
    if(!check_guard(pre) || !check_guard(post)){
        char buf[256], ts[64]; timestamp(ts,sizeof(ts));
        snprintf(buf,sizeof(buf),"[%s] [Erro] Sobrescrita de memória detectada em %p\n",ts,user_ptr);
        log_message(buf,COLOR_RED);
    }

    free(base);
}

// Settings
void md_set_language(Language l){lang=l;}
void md_set_verbosity(Verbosity v){verbosity=v;}
void md_enable_file_logging(int enable){log_to_file=enable;}

// Report
void md_report(void){
    printf("\n=== RELATÓRIO DE MEMÓRIA ===\n");
    printf("┌──────────────┬────────┬─────────────┬──────┐\n");
    printf("│ Ponteiro     │ Tamanho│ Arquivo     │ Linha│\n");
    printf("├──────────────┼────────┼─────────────┼──────┤\n");
    MemBlock* cur=head;
    while(cur){
        printf("│ %12p │ %6zu │ %-11s │ %4d │\n",cur->user_ptr,cur->size,cur->file,cur->line);
        cur=cur->next;
    }
    printf("└──────────────┴────────┴─────────────┴──────┘\n");
    printf("Total de alocações: %zu | Total de frees: %zu | Total de reallocs: %zu\n", total_allocations,total_frees,total_reallocs);
    printf("Pico de memória: %zu bytes | Maior alocação: %zu bytes\n",peak_allocated_bytes,largest_allocation);
}
