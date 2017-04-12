# Tox Split Message
Library providing split message functionality for sending messages over [Tox](https://tox.chat/).

Although it is written for Tox use-case in mind, it's generic enough to be used to split any UTF-8 string in any, i.e. even not Tox-related, project.

It tries to split a UTF-8 encoded message on the nearest space or punctuation character, or if there are none, then on the nearest UTF-8 codepoint.

## Example

Usage example with no error handling.

Splits a message in messages of `TOX_MAX_MESSAGE_LENGTH` size, trying to split on space, dot, comma or dash if present in last 1/4 of the message, instead of splitting in the middle of a word.

```C
#include <tox/tox.h>
#include <tox_split_message.h>

typedef struct Message_info {
    Tox *tox;
    uint32_t friend_number;
} Message_info;

void split_message_handler(const uint8_t *message, size_t length, void *user_data)
{
    Message_info *mi = user_data;
    tox_friend_send_message(mi->tox, mi->friend_number, TOX_MESSAGE_TYPE_NORMAL,
                            message, length, NULL);
}

void my_client_send_message(Tox *tox, uint32_t friend_number, uint8_t *message,
                            size_t length)
{
    Message_info mi = {tox, friend_number};
    tsm_split_message(message, length, TOX_MAX_MESSAGE_LENGTH, (uint8_t*)" .,-",
                      TOX_MAX_FILENAME_LENGTH/4, split_message_handler, &mi);
}
```

## Build
See [INSTALL.md](INSTALL.md).

Alternatively, you could just drop the source code in your project, it's just 2 files of C with no external dependencies.

## Contribute
1. Find a thing to fix/implement in [Issues](https://github.com/nurupo/tox-split-message/issues) or come up with your own idea, [create a discussion issue](https://github.com/nurupo/tox-split-message/issues/new) for it and get a feedback.
2. Make sure that no one is already working on it.
3. [Fork the repo](https://help.github.com/articles/fork-a-repo).
4. Create your feature branch (`git checkout -b my-new-feature`).
5. Commit your changes (`git commit -am 'Add some feature'`).
6. Push to the branch (`git push origin my-new-feature`).
7. [Create new Pull Request](https://help.github.com/articles/using-pull-requests).

## Contact
Leave nurupo an email on the address he uses in commits or ping `nurupo` on `#tox-dev` at Freenode IRC ([Freenode Webchat](https://webchat.freenode.net/?channels=#tox-dev), [KiwiIRC](https://kiwiirc.com/client/chat.freenode.net/?theme=basic#tox-dev)).

## License
This project is licensed under [Apache 2.0](LICENSE).
