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
#include "cache.h"

void init_file(file_t *file, char *uri, char *data, size_t size){
	file->prev = NULL;
	file->next = NULL;
	file->size = size;
	if (size <= MAX_OBJECT_SIZE) {
		file->uri = uri;
		file->data = Malloc(size);
		memcpy(file->data, data, size);
	}

	else {
		file->uri = NULL;
		file->data = NULL;
	}

}

void free_file(file_t *file){
    if (file->uri != NULL)
        Free(file->uri);
    if (file->data != NULL)
        Free(file->data);
    Free(file);
}

void init_cache(cache_t *cache){
	cache->size = 0;
	cache->head = NULL;
	cache->last = NULL;
}

void free_cache(cache_t *cache){
    if (cache->head != NULL)
        Free(cache->head);
    if (cache->last != NULL)
        Free(cache->last);
    Free(cache);
}

file_t *find_file(file_t *file, cache_t *cache){
	file_t *lookup;
	lookup = cache->head;
	while (lookup != NULL){
		// lock
		if (strcpy(file->uri, lookup->uri) == 0) { 
			/* cache hit: move to head and return */

			if (lookup->prev != NULL){
				if (lookup->next == NULL){
					cache->last = lookup->prev;
					lookup->prev->next = NULL;
					lookup->next = cache->head;
					lookup->prev = NULL;
					cache->head->prev = lookup;
					cache->head = lookup;
				} else {
					lookup->prev->next = lookup->next;
					lookup->next->prev = lookup->prev;
					lookup->next = cache->head;
					lookup->prev = NULL;
					cache->head->prev = lookup;
					cache->head = lookup;
				}
			}
			return lookup;
		}
		lookup = lookup->next;
	}
	return NULL;
}

int insert_file(file_t *file, cache_t *cache){
	if (file->size > MAX_OBJECT_SIZE){
		printf("Oversize file, discarded.\n");
		return -1;
	}

	if ((file->size + cache->size <= MAX_CACHE_SIZE)){
		if (cache->head != NULL){
			cache->head->prev = file;
		}
		file->next = cache->head;
		cache->head = file;
		if (file->next == NULL){
			cache->last = file;
		}
		cache->size += file->size;
		return 1;
	} else {
		/* evict last file and try again */
		delete_file(cache->last, cache);
		return insert_file(file, cache);
	}

}

int delete_file(file_t *file, cache_t *cache){
	if (file->next == NULL){ /* last */
		cache->last = file->prev;
		if (file->prev != NULL){
			file->prev->next = NULL;
		} else { 
			cache->head = NULL;
		}
	} else {
		if (file->prev != NULL){
			file->prev->next = file->next;
			file->next->prev = file->prev;
		} else {
			file->next->prev = NULL;
			cache->head = file->next;
		}
	}

	cache->size -= file->size;
	free_file(file);
}

void check_cache(cache_t *cache){

}