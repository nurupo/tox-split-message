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

#include <tox_split_message.h>

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * Tries to find split position of a UTF-8 encoded string at a UTF-8 codepoint closest to the provided position.
 * @param in_max_split_position A position at which, or before, we want to split the string.
 * @param in_string UTF-8 encoded string we want to split.
 * @param out_split_position Position of the last byte of the split string.
 * @return true if the position was found, false otherwise.
 */
bool codepoint_split(size_t in_max_split_position, const uint8_t *in_string, size_t *out_split_position)
{
    const uint8_t multibyte_codepoint = 1 << 7; // 1xxxxxxx mask
    const uint8_t multibyte_codepoint_start = 1 << 6; // x1xxxxxx mask
    // UTF-8 codepoints are maximum of 4 bytes in length (per RFC3629), so we have to look only at 4 bytes
    for (size_t i = in_max_split_position+1; i >= in_max_split_position - 3; i --) {
        // if we are at multibyte codepoint but not at its start yet -- keep moving back
        if ((in_string[i] & multibyte_codepoint) && !(in_string[i] & multibyte_codepoint_start)) {
            continue;
        }
        // we are either at a single-byte codepoint or at the beginning of a multibyte one -- split the message
        *out_split_position = i;

        // compensate for starting multibyte codepoint, since we don't want to split in the middle of multibyte codepoint
        if ((in_string[i] & multibyte_codepoint) && (in_string[i] & multibyte_codepoint_start)) {
            *out_split_position -= 1;
        } else {
            // we are at single-yte codepoint, exclude it if it's past the max position
            *out_split_position -= (i == in_max_split_position+1 ? 1 : 0);
        }

        return true;
    }

    return false;
}

/**
 * Tries to find split position of a UTF-8 encoded string, splitting on one of in_split_on codepoints the in last
 * in_split_on_lookup_range bytes of the string, keeping the codepoint we split on at the end of the split string.
 * @param in_max_split_position A position at which, or before, we want to split the string.
 * @param in_string UTF-8 encoded string we want to split.
 * @param in_split_on UTF-8 encoded and null terminated string containing codepoints that we would like to split on.
 * @param in_split_on_lookup_range Number of bytes from the end of string to check against against in_split_on
 * charaters.
 * @param out_split_position Position of the last byte of the split string.
 * @return true if the position was found, false otherwise.
 */
bool special_split(size_t in_max_split_position, const uint8_t *in_string, const uint8_t *in_split_on,
                   size_t in_split_on_lookup_range, size_t *out_split_position)
{
    const uint8_t multibyte_codepoint = 1 << 7; // 1xxxxxxx mask
    const uint8_t multibyte_codepoint_start = 1 << 6; // x1xxxxxx mask

    // look for them in the last 1/4 of the message only, since splitting in the beginning of a string might look a bit
    // weird, for example in case of "example aaaaaaaaaa..." it would be split into very short "example" and very long
    // "aaaaaaaaaa..." messages, when it would be better to split in into long "example aaaaaaaaaa..." and another
    // message with the rest of "...aaaaaaaaaa"
    for (size_t i = in_max_split_position; i >= in_max_split_position - (in_split_on_lookup_range-1); i --) {
        // if we are at multibyte codepoint but not at its start yet -- keep moving back
        if ((in_string[i] & multibyte_codepoint) && !(in_string[i] & multibyte_codepoint_start)) {
            continue;
        }

        // we are either at a single-byte codepoint or at the beginning of a multibyte one
        // check if it matches any of in_split_on codepoints
        size_t split_i = 0; // start from the very first codepoint
        while (in_split_on[split_i] != 0) {
            size_t split_j = 0; // start from the very first byte of the codepoint
            while (true) {
                // if the in_split_on codepoint is longer than the remaining of the string -- stop working with that
                // codepoint
                if (i + split_j > in_max_split_position) {
                    break;
                }

                // if we hit next in_split_on codepoint -- we have match
                if (!((in_split_on[split_i + split_j] & multibyte_codepoint) &&
                      !(in_split_on[split_i + split_j] & multibyte_codepoint_start)) && split_j != 0) {
                    *out_split_position = i + split_j - 1;
                    return true;
                }
                // if we hit end of in_split_on codepoints -- we have match
                if (in_split_on[split_i + split_j] == 0) {
                    *out_split_position = i + split_j - 1;
                    return true;
                }

                // if a byte of in_split_on codepoint doesn't match -- stop working with that codepoint
                if (in_split_on[split_i + split_j] != in_string[i + split_j]) {
                    break;
                }

                // the byte of in_split_on codepoint matched, so let's move to the next byte of it
                split_j ++;
            }

            // the codepoint didn't match
            // advance to the next in_split_on codepoint
            do {
                split_i ++;
            // while not at codepoint start
            } while ((in_split_on[split_i] & multibyte_codepoint) &&
                     !(in_split_on[split_i] & multibyte_codepoint_start));
        }
    }

    return false;
}

bool tsm_split_message(const uint8_t *string, size_t length, size_t max_split_string_length,
                       const uint8_t *split_on, size_t split_on_lookup_range,
                       tsm_callback *callback, void *user_data)
{
    if (!string || length == 0 || max_split_string_length < 4 || !callback) {
        return false;
    }

    if (split_on) {
        if (split_on_lookup_range == 0 || split_on_lookup_range > max_split_string_length) {
            return false;
        }
    }

    size_t offset = 0;
    // keep splitting string while it's too big
    while (length - offset > max_split_string_length) {
        const size_t split_max_position = offset + max_split_string_length - 1;

        size_t codepoint_split_position; // inclusive
        bool found_codepoint_split = codepoint_split(split_max_position, string, &codepoint_split_position);

        // try to split on whitespace or punctuation instead of just cutting off at a codepoint
        size_t special_split_position; // inclusive
        bool found_special_split = false;
        if (split_on) {
            found_special_split = special_split(split_max_position, string, split_on, split_on_lookup_range,
                                                &special_split_position);
        }

        // shouldn't happen if the string is a valid UTF8 string
        if (!found_codepoint_split && !found_special_split) {
            return false;
        }

        size_t split_position = special_split_position;
        if (!found_special_split) {
            split_position = codepoint_split_position;
        }

        callback(string + offset, split_position + 1 - offset, user_data);

        offset = split_position + 1;
    }

    // see if there is a leftover string that is too small to be split
    if (length - offset > 0) {
        callback(string + offset, length - offset, user_data);
    }

    return true;
}
