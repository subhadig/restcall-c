/**
 * MIT License

 * Copyright (c) 2025 Subhadip Ghosh
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "rest_template.h"
#include "cJSON.h"

#ifndef SRC_REST_TEMPLATE_C
#define SRC_REST_TEMPLATE_C
#define GOTO_LABEL_IF_NULL(x,y) if (x == NULL) goto y;
#define GOTO_END_IF_NULL(x) GOTO_LABEL_IF_NULL(x, end);
#endif

typedef struct keyvalue {
    char *key;
    char *value;
} keyvalue;

typedef struct request {
    char *url;
    char *http_method;
    char *auth_type;
    char *auth_token;
    char *content_type;
    keyvalue *headers;
    unsigned headers_size;
    char *payload;
} request;

typedef struct response {
    unsigned status;
    keyvalue *headers;
    unsigned headers_size;
    unsigned size;
    char *body;
} response;

typedef struct rest_template {
    request *request;
    response *response;
} rest_template;

/* Validates the https method */
static bool validate_method(char *method) {
    return method != NULL && strcmp(method, "GET") == 0;
}

rest_template *rest_template_create(char *method) {
    rest_template *resttemplate_p = NULL;

    if (!validate_method(method)) {
        goto failed;
    }

    resttemplate_p = (rest_template *)malloc(sizeof(rest_template));
    GOTO_LABEL_IF_NULL(resttemplate_p, failed);

    resttemplate_p->request = (request *)malloc(sizeof(request));
    GOTO_LABEL_IF_NULL(resttemplate_p->request, failed);

    resttemplate_p->request->http_method = (char *)malloc(strlen(method)+1);
    GOTO_LABEL_IF_NULL(resttemplate_p->request->http_method, failed_request);
    strcpy(resttemplate_p->request->http_method, method);

    return resttemplate_p;

failed_request:
    free(resttemplate_p->request);
failed:
    free(resttemplate_p);
    return NULL;
}

void rest_template_free(rest_template *resttemplate_p) {
    if (resttemplate_p == NULL) {
        return;
    }
    if (resttemplate_p->request != NULL) {
        free(resttemplate_p->request->url);
        free(resttemplate_p->request->http_method);
        free(resttemplate_p->request->auth_type);
        free(resttemplate_p->request->auth_token);
        free(resttemplate_p->request->content_type);
        free(resttemplate_p->request->headers);
        free(resttemplate_p->request->payload);
        free(resttemplate_p->request);
    }
    free(resttemplate_p);
}

static char *get_string_or_default(char *str, char *default_value) {
    if (str == NULL) {
        return default_value;
    } else {
        return str;
    }
}

static cJSON *serialize_headers(keyvalue *arr, unsigned size) {
    cJSON *headers_json = cJSON_CreateObject();
    GOTO_LABEL_IF_NULL(headers_json, failed);
    for (int i = 0; i < size; i++) {
        GOTO_LABEL_IF_NULL(cJSON_AddStringToObject(headers_json,
                    get_string_or_default(arr[i].key, ""),
                    get_string_or_default(arr[i].value, "")),
                failed);
    }
    return headers_json;
failed:
    cJSON_Delete(headers_json);
    return NULL;
}

char *replace_tabs_with_spaces(char *str) {
    char *str_copy = str;
    unsigned new_len = 0;
    bool is_new_line = true;

    //Get the required size for the output string
    while (*str_copy != '\0') {
        if (*str_copy++ == '\t' && is_new_line) {
            new_len = new_len + 4;
        } else {
            ++new_len;
        }
        if (*str_copy == '\n') {
            is_new_line = true;
        } else if (*str_copy != '\t') { //Continous tabs should not set is_new_line
            is_new_line = false;
        }
    }

    //Allocate memory for the new string
    char *new_str = (char *)calloc(new_len + 1, sizeof(char));
    if (new_str == NULL) {
        fprintf(stderr, "Unable to allocate memory for new str.\n");
        goto end;
    }

    //Populate the new string replacing tabs with spaces
    str_copy = str;
    char *new_str_copy = new_str;
    is_new_line = true;
    while (*str_copy != '\0') {
        if (*str_copy == '\t') {
            unsigned spaces_to_be_replaced_with = 1; //Inner tabs are replaced with 1 space
            if (is_new_line) {
                spaces_to_be_replaced_with = 4; //When tab occurs at the beginning of the line, it will be replaced with 4 spaces
            }
            for (unsigned i = 0; i < spaces_to_be_replaced_with; i++) {
                *new_str_copy++ = ' ';
            }
        } else {
            *new_str_copy++ = *str_copy;
        }
        if (*str_copy == '\n') {
            is_new_line = true;
        } else if (*str_copy != '\t') {
            is_new_line = false;
        }
        str_copy++;
    }
    *str_copy = '\0';
end:
    //Free the original string
    free(str);
    return new_str;
}

char *rest_template_serialize(rest_template *resttemplate_p) {
    char *json_str = NULL;
    cJSON *rt = NULL;

    GOTO_END_IF_NULL(resttemplate_p->request);

    //Create the root Json
    rt = cJSON_CreateObject();
    GOTO_END_IF_NULL(rt);

    //Create the request Json
    cJSON *request = cJSON_AddObjectToObject(rt, "request");
    GOTO_END_IF_NULL(request);

    GOTO_END_IF_NULL(cJSON_AddStringToObject(request, "url", get_string_or_default(resttemplate_p->request->url, "")));
    GOTO_END_IF_NULL(cJSON_AddStringToObject(request, "httpMethod", get_string_or_default(resttemplate_p->request->http_method, "")));
    GOTO_END_IF_NULL(cJSON_AddStringToObject(request, "authType", get_string_or_default(resttemplate_p->request->auth_type, "")));
    GOTO_END_IF_NULL(cJSON_AddStringToObject(request, "authToken", get_string_or_default(resttemplate_p->request->auth_token, "")));
    GOTO_END_IF_NULL(cJSON_AddStringToObject(request, "contentType", get_string_or_default(resttemplate_p->request->content_type, "")));
    cJSON *headers = serialize_headers(resttemplate_p->request->headers, resttemplate_p->request->headers_size);
    GOTO_END_IF_NULL(headers);
    cJSON_AddItemToObject(request, "headers", headers);
    GOTO_END_IF_NULL(cJSON_AddStringToObject(request, "body", get_string_or_default(resttemplate_p->request->payload, "")));
    json_str = cJSON_Print(rt);
    if(json_str == NULL) {
        fprintf(stderr, "Unable to convert Json to string\n");
        goto end;
    }
    json_str = replace_tabs_with_spaces(json_str);

end:
    cJSON_Delete(rt);
    return json_str;
}

static char *read_string_value_from_json(cJSON *json, char *key) {
    cJSON *value = cJSON_GetObjectItemCaseSensitive(json, key);
    if (value == NULL || !cJSON_IsString(value)) {
        fprintf(stderr, "Unable to read %s from json or it's not in correct format\n", key);
        goto failed;
    }
    return value->valuestring;

failed:
    return NULL;
}

static keyvalue *deserialize_headers(cJSON *headers_json, unsigned *headers_size) {
    *headers_size = cJSON_GetArraySize(headers_json);
    if (*headers_size == 0) goto failed;

    keyvalue *headers = calloc(*headers_size, sizeof(keyvalue));
    if (headers == NULL) {
        fprintf(stderr, "Unable to allocate memory for headers\n");
        goto cleanup_headers;
    }

    cJSON *keyvalue_json = NULL;
    keyvalue *headers_cpy = headers;
    cJSON_ArrayForEach(keyvalue_json, headers_json) {
        headers_cpy->key = keyvalue_json->string;
        headers_cpy->value = read_string_value_from_json(keyvalue_json, "value");
        if (headers_cpy->key == NULL || headers_cpy->value == NULL) {
            fprintf(stderr, "Unable to read key value for header\n");
            goto cleanup_headers;
        }
    }

    return headers;

cleanup_headers:
    free(headers);
failed:
    return NULL;
}

static bool convert_json_rest_template(cJSON *resttemplate_json, rest_template *resttemplate_p) {
    cJSON *request_json = cJSON_GetObjectItemCaseSensitive(resttemplate_json, "request");
    if (request_json == NULL) {
        fprintf(stderr, "Unable to read request from json\n");
        goto failed;
    }

    if (cJSON_GetObjectItemCaseSensitive(resttemplate_json, "response") != NULL) {
        fprintf(stderr, "Rest template also contains response\n");
        goto failed;
    }

    resttemplate_p->request = (request *)malloc(sizeof(request));
    if (resttemplate_p->request == NULL) {
        fprintf(stderr, "Unable to allocate memory for request\n");
        goto cleanup_request;
    }

    resttemplate_p->request->url = read_string_value_from_json(request_json, "url");
    if (resttemplate_p->request->url == NULL) goto cleanup_request;

    resttemplate_p->request->http_method = read_string_value_from_json(request_json, "httpMethod");
    if (resttemplate_p->request->http_method == NULL) goto cleanup_request;

    resttemplate_p->request->auth_type = read_string_value_from_json(request_json, "authType");
    if (resttemplate_p->request->auth_type == NULL) goto cleanup_request;

    resttemplate_p->request->auth_token = read_string_value_from_json(request_json, "authToken");
    if (resttemplate_p->request->auth_type == NULL) goto cleanup_request;

    resttemplate_p->request->content_type = read_string_value_from_json(request_json, "contentType");
    if (resttemplate_p->request->content_type == NULL) goto cleanup_request;

    resttemplate_p->request->payload = read_string_value_from_json(request_json, "body");
    if (resttemplate_p->request->payload == NULL) goto cleanup_request;

    cJSON *headers_json = cJSON_GetObjectItemCaseSensitive(request_json, "headers");
    if (headers_json == NULL) {
        fprintf(stderr, "Unable to read headers\n");
        goto cleanup_request;
    }
    resttemplate_p->request->headers = deserialize_headers(headers_json, &resttemplate_p->request->headers_size);

    return true;

cleanup_request:
    if (resttemplate_p->request != NULL) {
        free(resttemplate_p->request->url);
        free(resttemplate_p->request->auth_type);
        free(resttemplate_p->request->auth_token);
        free(resttemplate_p->request->content_type);
        free(resttemplate_p->request->payload);
    }
    free(resttemplate_p->request);
failed:
    return false;
}

rest_template *rest_template_deserialize(char *resttemplate_str) {
    rest_template *resttemplate_p = NULL;
    cJSON *resttemplate_json = NULL;
    if (resttemplate_str == NULL || strlen(resttemplate_str) == 0) {
        fprintf(stderr, "NULL or empty string is provided as input\n");
        goto failed;
    }

    resttemplate_json = cJSON_Parse(resttemplate_str);
    if (resttemplate_json == NULL) {
        fprintf(stderr, "Json conversion failed for input\n");
        goto failed;
    }

    resttemplate_p = (rest_template *)malloc(sizeof(rest_template));
    if (resttemplate_p == NULL) {
        fprintf(stderr, "Failed allocating memory for rest_template\n");
        goto cleanup_json;
    }

    if (convert_json_rest_template(resttemplate_json, resttemplate_p) != true) {
        fprintf(stderr, "Failed to convert Json to rest_template\n");
        goto cleanup_resttemplate;
    }
    return resttemplate_p;

cleanup_resttemplate:
    free(resttemplate_p);
cleanup_json:
    cJSON_Delete(resttemplate_json);
failed:
    return NULL;
}
