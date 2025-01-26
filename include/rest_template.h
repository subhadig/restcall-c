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

#include <stdbool.h>

#ifndef INCLUDE_REST_TEMPLATE_H
#define INCLUDE_REST_TEMPLATE_H

typedef struct rest_template rest_template;

/**
 * Create a rest template.
 * Inputs:
 * - method: GET, POST, PUT etc
 * Output:
 * - pointer to the generated rest template
 **/
rest_template *rest_template_create(char *method);

/**
 * Execute the rest template.
 * Inputs:
 * - resttemplate_p: pointer to the rest_template
 * Output:
 * - true if execution is successful. false otherwise.
 **/
bool rest_template_execute(rest_template *resttemplate_p);

/**
 * Free a rest template.
 * Inputs:
 * - resttemplate_p: pointer to the rest_template
 **/
void rest_template_free(rest_template *resttemplate_p);

/**
 * Serializes the rest template into a string.
 * Inputs:
 * - resttemplate_p: pointer to the rest_template
 * Output:
 * - Serialized rest_template string 
 **/
char *rest_template_serialize(rest_template *resttemplate_p);

/**
 * Deserializes the string into rest template.
 * Inputs:
 * - resttemplate_str: Serialized rest_template string 
 * Output:
 * - Pointer to the created rest_template
 **/
rest_template *rest_template_deserialize(char *resttemplate_str);

#endif /* INCLUDE_REST_TEMPLATE_H */
