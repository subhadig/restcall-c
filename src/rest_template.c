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

#include "../include/rest_template.h"
#include <stdlib.h>

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
    char *payload;
} request;

typedef struct response {
    unsigned status;
    keyvalue *headers;
    unsigned size;
    char *body;
} response;

typedef struct rest_template {
    request *request;
    response *response;
} rest_template;

rest_template *rest_template_create(char *method) {
    rest_template *rest_template_p = (rest_template *)malloc(sizeof(rest_template));
    return NULL;
}
