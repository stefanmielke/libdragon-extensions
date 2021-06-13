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

/**
 * @brief Creates all structs and functions for the Object Pool for the type provided.
 *
 * @param[in] TYPE
 *            Type that will be used on the Object Pool.
 */
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

/**
 * @brief Allocates an object pool for the type provided.
 *
 * @param[in] TYPE
 *            Type used when creating the object pool.
 * @param[in] MEMORY_POOL
 *            MemZone that will be used to allocate the object pool. If NULL will used malloc/calloc
 * instead (remember to call 'objpool_free' in that case).
 * @param[in] NUM
 *            Amount of objects in the object pool.
 */
#define objpool_create(TYPE, MEMORY_POOL, NUM) objpool_##TYPE##_create(MEMORY_POOL, NUM)

/**
 * @brief Destroy the Object Pool provided. Only use this if not using a memory pool.
 *
 * @param[in] TYPE
 *            Type used when creating the object pool.
 * @param[in] POOL
 *            Object pool to return the object to.
 */
#define objpool_destroy(TYPE, POOL) objpool_##TYPE##_destroy(POOL)

/**
 * @brief Get a new object from the pool. Returns NULL if there are no objects available (all are in
 * use).
 *
 * @param[in] TYPE
 *            Type used when creating the object pool.
 * @param[in] POOL
 *            Object pool to get the object from.
 */
#define objpool_get(TYPE, POOL) objpool_##TYPE##_get(POOL)

/**
 * @brief Returns an object to the object pool so that it can be reused later.
 *
 * @param[in] TYPE
 *            Type used when creating the object pool.
 * @param[in] POOL
 *            Object pool to get the object from.
 * @param[in] OBJ
 *            Object that will be returned to the pool. Do not use this object after this call.
 */
#define objpool_free(TYPE, POOL, OBJ) objpool_##TYPE##_free(POOL, OBJ)
