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

#ifndef INCLUDE_RESTCALL_H
#define INCLUDE_RESTCALL_H

/**
 * Generates a rest template and stores in the input filepath.
 * Inputs:
 * - filepath: path where the rest template will be stored.
 * Output:
 * - true if successful. false otherwise.
 **/
bool restcall_generate_template(char *filepath);

/**
 * Executes a restcall using the template from the given filepath.
 * Inputs:
 * - filepath: path from where the template is read.
 * Output:
 * - true if successful. false otherwise.
 **/
bool restcall_execute(char *filepath);

#endif /* INCLUDE_RESTCALL_H */
