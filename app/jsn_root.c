#include <stdio.h>
#include <sys/stat.h>
#include <json-c/json.h>
#include "app_err.h"
#include "jsn_root.h"
#include "log.h"

#define VSCODE_DIR_NAME ".vscode"
#define JSON_FILE_NAME "c_cpp_properties.json"
#define PATH_TO_JSON_FILE VSCODE_DIR_NAME "/" JSON_FILE_NAME

// Flags here: https://github.com/json-c/json-c/blob/041cef434afe0d0c6da8b6ac1d1fa26087246dda/json_object.h#L49
#define JSON_C_TO_STRING_SPACED_PRETTY_NOSLASHESCAPE 19 /* 0b10011 */

void jsn_root_free(JsnRoot jsn_rt)
{
    json_object_put(jsn_rt.jsn);
}

JsnRoot jsn_root_read_from_file(app_err *err)
{
    JsnRoot jsn_rt = {
        .jsn = NULL,
    };

    json_object *jsn = json_object_from_file(PATH_TO_JSON_FILE);
    if (!jsn)
    {
        app_err_set(err, FAILED_READING_FROM_JSON_FILE, (char *)json_util_get_last_err());
        return jsn_rt;
    }

    jsn_rt.jsn = jsn;
    return jsn_rt;
}

JsnRoot jsn_root_initialize()
{
    json_object *jsn = json_object_new_object();
    json_object_object_add(jsn, "version", json_object_new_int(4));

    JsnRoot jsn_rt = {
        .jsn = jsn,
    };
    return jsn_rt;
}

JsnRoot jsn_root_get()
{
    app_err local_err = app_err_new();
    JsnRoot jsn_rt = jsn_root_read_from_file(&local_err);
    if (app_err_happened(&local_err))
    {
        log_warnf("json file not found at \"%s\", initializing a new one\n", PATH_TO_JSON_FILE);
        jsn_rt = jsn_root_initialize();
    }
    return jsn_rt;
}

void jsn_root_print(JsnRoot *jsn_rt)
{
    log_debugf("updated json file:\n%s\n", json_object_to_json_string_ext(jsn_rt->jsn, JSON_C_TO_STRING_SPACED_PRETTY_NOSLASHESCAPE));
}

void jsn_root_write_to_file(JsnRoot *jsn_rt, app_err *err)
{
    struct stat st = {0};
    if (stat(VSCODE_DIR_NAME, &st) == -1)
    {
        log_warnf("creating directory \"%s\"\n", VSCODE_DIR_NAME);
        mkdir(VSCODE_DIR_NAME, 0700);
    }
    if (stat(PATH_TO_JSON_FILE, &st) == -1)
    {
        log_warnf("creating json file \"%s\"\n", PATH_TO_JSON_FILE);
    }

    int write_result = json_object_to_file_ext(PATH_TO_JSON_FILE, jsn_rt->jsn, JSON_C_TO_STRING_SPACED_PRETTY_NOSLASHESCAPE);
    if (write_result == -1)
    {
        app_err_set(err, FAILED_WRITING_TO_JSON_FILE, (char *)json_util_get_last_err());
    }
}
