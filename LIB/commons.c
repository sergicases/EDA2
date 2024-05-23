// Include the commons header file
#include "commons.h"

// Include the standard argument library for handling variable argument functions
#include "stdarg.h"

// Function to generate a random integer between 0 and max-1
int random_int(int max)
{
    // Static variable to ensure random seed is initialized only once
    static bool is_random_init = false;
    
    // Initialize the random seed if not already done
    if (!is_random_init)
    {
        srand((unsigned int)time(NULL));
        is_random_init = true;
    }

    // If max is less than or equal to 1, return 0 as there's no range to generate a random number
    if (max <= 1)
        return 0;

    // Generate and return a random number between 0 and max-1
    return rand() % max;
}

// Function to consume and discard the remaining input in the standard input stream
void consume_stdin()
{
    char c;
    // Read characters from the input stream until a newline or EOF is encountered
    while ((c = getchar()) && (c != '\n') && (c != EOF));
}

// Function to read an option from the standard input
int read_option(void)
{
    char buffer[64];
    // Read a line of input from stdin into the buffer
    if (!fgets(buffer, 64, stdin))
        return -1;

    int value;
    // Attempt to parse an integer from the buffer
    if (sscanf(buffer, "%d", &value) != 1)
        return -1;

    // Return the parsed integer value
    return value;
}

// Function to compute a hash value for a string given the length of the hashtable
size_t string_hash(size_t hashtable_length, const char* str)
{
    size_t hash = 0;
    // Iterate through each character in the string, summing their ASCII values
    for (int i = 0; str[i] != '\0'; i++)
        hash += str[i];

    // Return the computed hash value modulo the hashtable length
    return hash % hashtable_length;
}

// Function to print an error message with variable arguments
void print_error(const char* message, ...)
{
    // Initialize the variable argument list
    va_list args;
    va_start(args, message);

    // Print the error message prefix
    printf("[ERROR]: ");
    // Print the formatted error message
    vprintf(message, args);

    // Clean up the variable argument list
    va_end(args);
}
