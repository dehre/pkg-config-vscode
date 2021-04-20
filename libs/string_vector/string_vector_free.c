#include <stdlib.h>
#include "string_vector.h"

void string_vector_free(StringVector string_vec)
{
    for (int i = 0; i < string_vec.len; i++)
    {
        free(*(string_vec.data + i));
    }
    if (string_vec.data != NULL)
    {
        free(string_vec.data);
    }
}