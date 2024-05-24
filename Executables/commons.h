// Prevent multiple inclusions of this header file
#ifndef COMMONS_H
#define COMMONS_H

// Standard library includes
#include <stdio.h>   // For standard input/output functions
#include <stdlib.h>  // For memory allocation, process control, conversions, and others
#include <string.h>  // For string handling functions
#include <stdbool.h> // For boolean type and values
#include <time.h>    // For time-related functions

// Include cJSON library for JSON handling
#include "cJSON.h"

// Define constants for maximum string lengths
#define NAMES_STR_LEN 64  // Maximum length for name strings
#define TEXT_LEN 1024     // Maximum length for text strings

// Define a macro to get the maximum of two values
#ifndef max
#define max(_A, _B) ((_A) > (_B) ? (_A) : (_B))
#endif

// Define a macro to get the minimum of two values
#ifndef min
#define min(_A, _B) ((_A) < (_B) ? (_A) : (_B))
#endif

// Define a macro to clamp a value between a minimum and maximum range
#ifndef clamp
#define clamp(_Value, _Min, _Max) max((_Min), min((_Max), (_Value)))
#endif

// Define a macro to print an error message and return false
#define print_error_and_return_false(...) print_error(__VA_ARGS__), (false)

// Function prototypes

// Generate a random integer between 0 and max-1
int random_int(int max);

// Consume and discard the remaining input in the standard input stream
void consume_stdin();

// Read an option from the standard input
int read_option(void);

// Compute a hash value for a string given the length of the hashtable
size_t string_hash(size_t hashtable_length, const char* str);

// Print an error message
void print_error(const char* message, ...);

// End of the header guard
#endif //COMMONS_H
