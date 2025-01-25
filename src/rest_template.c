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

#include <stdlib.h>
#include <string.h>
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
    free(resttemplate_p->request->http_method);
    free(resttemplate_p->request);
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

char *rest_template_serialize(rest_template *resttemplate_p) {
    char *json_str = NULL;
    cJSON *rt = NULL;

    GOTO_END_IF_NULL(resttemplate_p->request);

    rt = cJSON_CreateObject();
    GOTO_END_IF_NULL(rt);

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
    json_str = cJSON_Print(rt);

end:
    cJSON_Delete(rt);
    return json_str;
}
