#pragma once

#include <malloc.h>
#include <stdlib.h>

// A contiguous zone where memory can be allocated.
typedef struct {
    char* pos;    // Pointer to current free space position.
    char* start;  // Pointer to start of zone.
    char* end;    // Pointer to end of zone.
} MemZone;

// Allocate a memory zone with the given size.
void mem_zone_init(MemZone *z, size_t size);

// Allocate memory from the zone.
void *mem_zone_alloc(MemZone *z, size_t size);

// Free all objects in the zone.
void mem_zone_free_all(MemZone *z);