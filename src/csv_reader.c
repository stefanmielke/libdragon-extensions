#include "../include/csv_reader.h"

#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <libdragon.h>

#define CSV_READER_INTERNAL_NUMBERS(TYPE)                                                          \
	{                                                                                              \
		const char *tok;                                                                           \
		int fp = dfs_open(csv_path);                                                               \
		char *buffer = malloc(dfs_size(fp));                                                       \
		int bytes_read;                                                                            \
		size_t i = 0;                                                                              \
		while ((bytes_read = dfs_read(buffer, sizeof(char), dfs_size(fp), fp)) > 0 &&              \
			   i < total_item_count) {                                                             \
			for (tok = strtok(buffer, ","); tok && *tok; tok = strtok(NULL, ",\n\r")) {            \
				output_array[i] = (TYPE)atoi(tok);                                                 \
				--output_array[i];                                                                 \
				++i;                                                                               \
			}                                                                                      \
		}                                                                                          \
                                                                                                   \
		free(buffer);                                                                              \
		dfs_close(fp);                                                                             \
	}

void csv_reader_from_ints(const char *csv_path, size_t total_item_count, int16_t output_array[]) {
	CSV_READER_INTERNAL_NUMBERS(int)
}

void csv_reader_from_chars(const char *csv_path, size_t total_item_count, char output_array[]) {
	CSV_READER_INTERNAL_NUMBERS(char)
}