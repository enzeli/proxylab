/*
 * 15213 2013 Fall
 * Proxy Lab
 * Name: Enze Li
 * Andrew id: enzel
 * ----------------------
 * 
 *
 *
 */

#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

typedef struct cache_file_t {
	struct cache_file_t *prev;
	struct cache_file_t *next;
	size_t size;
	char *uri;
	char *data;
} file_t;

typedef struct proxy_cache_t {
	file_t *head;
	file_t *last;
	size_t size;
	// pthread_rwlock_t rwlock;
} cache_t;

void init_file(file_t *file, char *uri, char *data, size_t size);
void init_cache(cache_t *cache);
void free_file(file_t *file);
void free_cache(cache_t *cache);
int insert_file(file_t *file, cache_t *cache);
int delete_file(file_t *file, cache_t *cache);
file_t *find_file(file_t *file, cache_t *cache);
void check_cache(cache_t *cache);
