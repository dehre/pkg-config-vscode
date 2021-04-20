#pragma once
#include <stdbool.h>

typedef enum
{
    OK,
    INVALID_SHELL_COMMAND,
    INVALID_LIB_PROVIDED,
    JSON_FAILED_READING,
    JSON_FAILED_WRITING,
} app_err;

app_err app_err_new();

bool app_err_happened(app_err *err);

void app_err_set(app_err *err, app_err new_value, char *msg);

void app_err_reset(app_err *err);

void app_err_print();

#define CHECK_APP_ERR_OR_RETURN(x) \
    {                              \
        if (*err != OK)            \
        {                          \
            return x;              \
        }                          \
    }