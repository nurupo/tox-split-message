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
 * Function to be called for each split string.
 * @param split_string Pointer to the beginning of the split string.
 * Points back to the original string provided, so the string is not null terminated.
 * @param length Length of the split string.
 * @param user_data Pointer that was provided whe nsettign up callback.
 */
typedef void tsm_callback(const uint8_t *split_string, size_t length, void *user_data);

/**
 * Splits a UTF-8 encoded string into several, such that they are at most max_split_string_length
 * bytes in length.
 * First it goes in reverse over the last split_on_lookup_range bytes of a max_split_string_length
 * byte part of the string, trying to split on one of split_on codepoints. If no split_on codepoints
 * are found, it tries to split on the closest to the end of max_split_string_length bytes string
 * codepoint.
 * @param string UTF-8 encoded string to be split. Can't be NULL.
 * @param length Length of the string in bytes, excluding the null ternimator, if any. Can't be 0.
 * @param max_split_string_length Maximum allowed length in bytes of the split strings. Can be greater
 * than original string's length, in which case the original string will be returned in the callback.
 * Should be at least 4 bytes, because UTF-8 codepoints vary from 1 to 4 bytes and we don't split in
 * the middle of a codepoint, so we won't be able to split a single 4 byte codepoint into smaller
 * pieces.
 * @param split_on UTF-8 encoded and null terminated string containing codepoints that we would like
 * to split the string on. For example, " .,-。" would mean that we want to split the string on space,
 * dot, comma, dash and ideographic full stop. A split_on codepoint is kept in the split string.
 * This argument is optional and may be set to NULL to always split the sting at the nearest to
 * max_split_string_length codepoint.
 * @param split_on_lookup_range Length in bytes of how deep from the end of max_split_string_length
 * sized string to look for split_on codepoints. Can be set to at most max_split_string_length bytes.
 * Can't be 0. It's ignored if split_on is set to NULL.
 * @param callback Function to be called for each split string.
 * @param user_data Pointer that will be passed into the callback function. May be NULL.
 * @return true on success, false on failure. Failure indicates that invalid arguments were passed.
 */
TOX_SPLIT_MESSAGE_EXPORT
bool tsm_split_message(const uint8_t *string, size_t length, size_t max_split_string_length,
                       const uint8_t *split_on, size_t split_on_lookup_range,
                       tsm_callback *callback, void *user_data);

#ifdef __cplusplus
}
#endif

#endif // TOX_SPLIT_MESSAGE_H
