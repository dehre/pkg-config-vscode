#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdbool.h>

typedef struct _string_vector
{
    char **data;
    int len;
} string_vector;

string_vector string_vector_empty();

void string_vector_free(string_vector string_vec);

// Splits the strings into tokens.
// The returned vector owns its data, so it's up to the callee to free it.
// If no delimiter is found, the vector will contain a single element.
string_vector string_split(char *src_string, char delimiter);

bool string_starts_with(char *src_string, char *prefix);

// If the source string doesn't start with prefix, a NULL pointer is returned.
// Otherwise, the returned pointer points to the source string, but is shifted prefix_len characters.
// No memory is allocated by this function.
char *string_strip_prefix(char *src_string, char *prefix);

#endif