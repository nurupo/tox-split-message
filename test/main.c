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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tox_split_message.h>

void callback(const uint8_t *split_string, size_t length, void *user_data)
{
    (void) user_data;
    assert(user_data == NULL);

    uint8_t string[length + 1];
    memcpy(string, split_string, length);
    string[length] = '\0';

    printf("length: %u\n", length);
    printf("string: \"%s\"\n\n", string);
}

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

    bool result = tsm_split_message(in_string, in_length, callback, NULL);

    printf("result: %d\n", result);

    return result;
}
