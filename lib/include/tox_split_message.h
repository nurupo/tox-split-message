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
 * It point back to the original string provided, so the string is
 * not null terminated.
 * @param user_data Pointer that was provided whe nsettign up callback.
 * @param length Length of the split string.
 */
typedef void tsm_callback(const uint8_t *split_string, size_t length, void *user_data);

/**
 * Splits a string into several, such that they will be at most TOX_MAX_MESSAGE_LENGTH in length,
 * trying to split on a whitespace or punctuation sign if possible.
 * @param in_string The UTF-8 encoded string to be split.
 * @param in_length Length of the string excluding the null ternimator, if any.
 * @param callback Function to be called for each split string.
 * @param user_data Pointer that will be passed into the callback function. May be null.
 * @return true on success, false on failure.
 */
TOX_SPLIT_MESSAGE_EXPORT
bool tsm_split_message(const uint8_t *in_string, size_t in_length, tsm_callback *callback, void *user_data);

#ifdef __cplusplus
}
#endif

#endif // TOX_SPLIT_MESSAGE_H
