#pragma once

#include <stddef.h>

/**
 * @brief Read a csv file with 'total_item_count' from 'csv_path' into 'output_array'
 *
 * @param csv_path
 *        Path to load the csv file from. Uses dfs_read.
 * @param total_item_count
 *        Total amount of items the array supports.
 * @param output_array
 *        Array to input the data from the csv file.
 */
void csv_reader_from_ints(const char *csv_path, size_t total_item_count, int output_array[]);

/**
 * @brief Read a csv file with 'total_item_count' from 'csv_path' into 'output_array'
 *
 * @param csv_path
 *        Path to load the csv file from. Uses dfs_read.
 * @param total_item_count
 *        Total amount of items the array supports.
 * @param output_array
 *        Array to input the data from the csv file.
 */
void csv_reader_from_chars(const char *csv_path, size_t total_item_count, char output_array[]);