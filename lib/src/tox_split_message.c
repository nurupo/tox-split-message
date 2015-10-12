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

#include <tox.h>

/**
 * Tries to increase capacity of strings and lengths arrays, by a multiple of the current capacity.
 * @param in_out_capacity Current capacity of strings and lengths arrays, which will get updated.
 * @param in_out_strings Pointer to strings array.
 * @param in_out_lengths Pointer to lengths array.
 * @return true on success, false on failure.
 */
bool insrease_capacity(size_t *in_out_capacity, uint8_t ***in_out_strings, size_t **in_out_lengths)
{
    if (*in_out_capacity == 0) {
        return false;
    }

    size_t capacity = ceil(*in_out_capacity * 1.5);

    // check for overflow of capacity, of sorts
    if (capacity <= *in_out_capacity) {
        return false;
    }

    {
        uint8_t **tmp;
        tmp = (uint8_t **) realloc(*in_out_strings, capacity * sizeof(*in_out_strings));
        if (!tmp) {
            return false;
        }
        *in_out_strings = tmp;
    }
    {
        size_t *tmp;
        tmp = (size_t *) realloc(*in_out_lengths, capacity * sizeof(*in_out_lengths));
        if (!tmp) {
            return false;
        }
        *in_out_lengths = tmp;
    }

    *in_out_capacity = capacity;

    return true;
}

/**
 * Tries to find split position of a UTF-8 encoded string at a UTF-8 codepoint closest to the provided position.
 * @param in_max_split_position A position at which, or before, we want to split the string.
 * @param in_string UTF-8 encoded string we want to split.
 * @param out_split_position Last position, after which you should split the string.
 * @return true if the position was found, false otherwise.
 */
bool codepoint_split(size_t in_max_split_position, const uint8_t *in_string, size_t *out_split_position)
{
    const uint8_t multibyte_codepoint = 1 << 7; // 1xxxxxxx mask
    const uint8_t multibyte_codepoint_start = 1 << 6; // x1xxxxxx mask
    // UTF-8 codepoints are maximum of 4 bytes in length (per RFC3629), so we have to look only at the last 4 bytes
    for (size_t i = in_max_split_position; i > in_max_split_position - 4; i --) {
        // if we are at multibyte codepoint but not at its start yet -- keep moving back
        if ((in_string[i] & multibyte_codepoint) && !(in_string[i] & multibyte_codepoint_start)) {
            continue;
        }
        // we are either at a single-byte codepoint or at the beginning of a multibyte one -- split the message
        *out_split_position = i;

        // compensate for starting codepoint, since we don't want to split in the middle of multibyte codepoint
        if ((in_string[i] & multibyte_codepoint) && (in_string[i] & multibyte_codepoint_start)) {
            *out_split_position -= 1;
        }

        return true;
    }

    return false;
}

/**
 * Tries to find split position of a UTF-8 encoded string at one of special characters in last 1/4 of the string.
 * @param in_max_split_position A position at which, or before, we want to split the string.
 * @param in_string UTF-8 encoded string we want to split.
 * @param out_split_position Last position, after which you should split the string.
 * @return true if the position was found, false otherwise.
 */
bool special_split(size_t in_max_split_position, const uint8_t *in_string, size_t *out_split_position)
{
    const uint8_t multibyte_codepoint = 1 << 7; // 1xxxxxxx mask
    const char *split_on = " .,-";
    // look for them in the last 1/4 of the message only, since splitting in the beginning of a string might look a bit
    // weird, for example in case of "example aaaaaaaaaa..." it would be split into very short "example" and very long
    // "aaaaaaaaaa..." messages, when it would be better to split in into long "example aaaaaaaaaa..." and another
    // message with the rest of "...aaaaaaaaaa"
    for (size_t i = in_max_split_position; i > in_max_split_position - TOX_MAX_MESSAGE_LENGTH / 4; i --) {
        // if we are at multibyte codepoint -- keep moving back, since all out special characters are single-byte
        if (in_string[i] & multibyte_codepoint) {
            continue;
        }
        // we are at single-byte codepoint, check if it matches any of split_on characters
        for (const char *split_char = split_on; *split_char != 0; split_char ++) {
            if (in_string[i] == *split_char) {
                *out_split_position = i;
                return true;
            }
        }
    }

    return false;
}

bool tsm_split_message(uint8_t *in_string, size_t in_length, uint8_t ***out_strings, size_t **out_lengths,
                  size_t *out_count)
{
    if (!in_string || in_length == 0 || !out_strings || !out_lengths || !out_count) {
        return false;
    }

    // the least number of splitted strings we will get (i.e. splitting strings exactly at TOX_MAX_MESSAGE_LENGTH)
    // * 1.5, to reduce number of reallocations. if we actually need more than that, we will realloc.
    size_t strings_capacity = ceil(ceil((in_length/(double)TOX_MAX_MESSAGE_LENGTH) + 1) * 1.5);

    // overflow check
    // shouldn't really happen, since TOX_MAX_MESSAGE_LENGTH > 1.5
    if (in_length/(double)TOX_MAX_MESSAGE_LENGTH > strings_capacity) {
        return false;
    }

    *out_strings = (uint8_t**) malloc(strings_capacity * sizeof(*out_strings));
    if (!*out_strings) {
        return false;
    }

    *out_lengths = (size_t *) malloc(strings_capacity * sizeof(*out_lengths));
    if (!*out_lengths) {
        free(*out_strings);
        return false;
    }

    *out_count = 1;
    size_t offset = 0;
    // keep slicing string while it's too big
    while (in_length - offset > TOX_MAX_MESSAGE_LENGTH) {
        const size_t split_max_position = offset + TOX_MAX_MESSAGE_LENGTH - 1;

        size_t codepoint_split_position; // inclusive
        bool found_codepoint_split = codepoint_split(split_max_position, in_string, &codepoint_split_position);

        // try to split on whitespace or punctuation instead of just cutting off at a codepoint
        size_t special_split_position; // inclusive
        bool found_special_split = special_split(split_max_position, in_string, &special_split_position);

        // shouldn't happen if the string is a valid UTF8 string
        if (!found_codepoint_split && !found_special_split) {
            free(*out_strings);
            free(*out_lengths);
            return false;
        }

        size_t split_position = special_split_position;
        if (!found_special_split) {
            split_position = codepoint_split_position;
        }

        if (*out_count > strings_capacity) {
            if (!insrease_capacity(&strings_capacity, out_strings, out_lengths) || (*out_count > strings_capacity)) {
                free(*out_strings);
                free(*out_lengths);
                return false;
            }
        }

        (*out_strings)[*out_count-1] = in_string + offset;
        (*out_lengths)[*out_count-1] = split_position + 1 - offset;

        offset = split_position + 1;

        (*out_count) ++;
    }

    if (in_length - offset > 0) {
        if (*out_count > strings_capacity) {
            if (!insrease_capacity(&strings_capacity, out_strings, out_lengths) || (*out_count > strings_capacity)) {
                free(*out_strings);
                free(*out_lengths);
                return false;
            }
        }

        (*out_strings)[*out_count-1] = in_string + offset;
        (*out_lengths)[*out_count-1] = in_length - offset;
    }

    return true;
}