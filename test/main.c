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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tox_split_message.h>

int main(void)
{
    uint8_t in_string[] = "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting."
                          "This is a very long string that requires some splitting.";

    size_t in_length = sizeof(in_string);

    uint8_t **out_strings;
    size_t *out_lengths;
    size_t out_count;

    bool result = tsm_split_message(in_string, in_length, &out_strings, &out_lengths, &out_count);

    printf("result: %d\n", result);

    if (!result) {
        return 1;
    }

    for (size_t i = 0; i < out_count; i ++) {
        printf("%u/%u\n", i+1, out_count);

        printf("length: %u\n", out_lengths[i]);

        uint8_t tmp = out_strings[i][out_lengths[i]];
        out_strings[i][out_lengths[i]] = '\0';

        printf("string: \"%s\"\n\n", out_strings[i]);

        out_strings[i][out_lengths[i]] = tmp;
    }

    free(out_strings);
    free(out_lengths);

    return 0;
}
