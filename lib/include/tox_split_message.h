/*
    This software is licensed under the Apache 2 license, quoted below.

    Copyright 2015 Maxim Biro <nurupo.contributions@gmail.com>

    Licensed under the Apache License, Version 2.0 (the "License"); you may not
    use this file except in compliance with the License. You may obtain a copy of
    the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
    License for the specific language governing permissions and limitations under
    the License.
*/

#ifndef TOX_SPLIT_MESSAGE_H
#define TOX_SPLIT_MESSAGE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <tox_split_message_export.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Splits a string into several, such that they will be at most TOX_MAX_MESSAGE_LENGTH in length,
 * trying to split on a whitespace or punctuation sign if possible.
 * @param in_string The UTF-8 encoded string to be split.
 * @param in_length Length of the string excluding the null ternimator, if any.
 * @param out_strings Pointer to array of strings. The array will be allocated by the function. You are responsible for
 * freeing the array only when the function succeeds. The strings in that array point back to different offsets of the
 * original string, so the strings are not null terminated.
 * @param out_lengths Array of length values of the corresponding strings. The array will be allocated by the function.
 * You are responsible for freeing the array only when the function succeeds.
 * @param out_count Number of elements in strings and length arrays.
 * @return true on success, false on failure.
 */
TOX_SPLIT_MESSAGE_EXPORT
bool tsm_split_message(uint8_t *in_string, size_t in_length, uint8_t ***out_strings, size_t **out_lengths,
                  size_t *out_count);

#ifdef __cplusplus
}
#endif

#endif // TOX_SPLIT_MESSAGE_H
