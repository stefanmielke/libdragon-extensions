/**
 * @file object_pool.h
 * @brief Macro-based Object Pooling (FreeList) based off of djoldman's objpool.
 * @see https://github.com/djoldman/objpool
 */

#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "mem_pool.h"

#define OBJPOOL_INIT(TYPE)                                                                         \
	typedef union objpool_##TYPE##_item_s objpool_##TYPE##_item_t;                                 \
	union objpool_##TYPE##_item_s {                                                                \
		objpool_##TYPE##_item_t *next;                                                             \
		TYPE data;                                                                                 \
	};                                                                                             \
	typedef struct objpool_##TYPE##_s {                                                            \
		objpool_##TYPE##_item_t *items;                                                            \
		objpool_##TYPE##_item_t *head;                                                             \
		size_t num;                                                                                \
	} objpool_##TYPE##_t;                                                                          \
	static inline objpool_##TYPE##_t *objpool_##TYPE##_create(MemZone *memory_pool,                \
															  const size_t num) {                  \
		if (num == 0) {                                                                            \
			return NULL; /* creating pool with zero items */                                       \
		}                                                                                          \
		objpool_##TYPE##_t *P;                                                                     \
		if (memory_pool) {                                                                         \
			P = mem_zone_alloc(memory_pool, sizeof(objpool_##TYPE##_t));                           \
			P->items = mem_zone_alloc(memory_pool, num * sizeof(objpool_##TYPE##_item_t));         \
		} else {                                                                                   \
			P = malloc(sizeof(objpool_##TYPE##_t));                                                \
			P->items = calloc(num, sizeof(objpool_##TYPE##_item_t));                               \
		}                                                                                          \
		P->head = &P->items[0];                                                                    \
		P->num = num;                                                                              \
		for (size_t i = 0; i < num - 1; i++) {                                                     \
			P->items[i].next = &P->items[i + 1];                                                   \
		}                                                                                          \
		P->items[num - 1].next = NULL;                                                             \
		return P;                                                                                  \
	}                                                                                              \
                                                                                                   \
	static inline void objpool_##TYPE##_destroy(objpool_##TYPE##_t *P) {                           \
		free(P->items);                                                                            \
		free(P);                                                                                   \
	}                                                                                              \
                                                                                                   \
	static inline TYPE *objpool_##TYPE##_get(objpool_##TYPE##_t *P) {                              \
		objpool_##TYPE##_item_t *item = P->head;                                                   \
		if (item == NULL) {                                                                        \
			return NULL;                                                                           \
		}                                                                                          \
		P->head = item->next;                                                                      \
		return &item->data;                                                                        \
	}                                                                                              \
                                                                                                   \
	static inline bool objpool_##TYPE##_free(objpool_##TYPE##_t *P, TYPE *OBJ) {                   \
		objpool_##TYPE##_item_t *I = (objpool_##TYPE##_item_t *)OBJ;                               \
		if ((I < P->items) || (I >= (P->items + P->num))) {                                        \
			return false;                                                                          \
		}                                                                                          \
		I->next = P->head;                                                                         \
		P->head = I;                                                                               \
		return true;                                                                               \
	}

#define objpool_t(TYPE) objpool_##TYPE##_t
#define objpool_create(TYPE, MEMORY_POOL, NUM) objpool_##TYPE##_create(MEMORY_POOL, NUM)
#define objpool_destroy(TYPE, POOL) objpool_##TYPE##_destroy(POOL)
#define objpool_get(TYPE, POOL) objpool_##TYPE##_get(POOL)
#define objpool_free(TYPE, POOL, OBJ) objpool_##TYPE##_free(POOL, OBJ)
