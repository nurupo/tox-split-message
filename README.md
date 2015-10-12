#Tox Split Message
Library providing split message functionality for sending messages over [Tox](https://tox.chat/).

It tries to split a UTF-8 encoded message on the nearest space or punctuation character, or if there are none, then on the nearest UTF-8 codepoint.

##Example

Usage example with no error handling.

```C
#include <tox_split_message.h>

void my_client_send_message(Tox *tox, uint32_t friend_number, uint8_t *message,
                            size_t length)
{
    uint8_t **messages;
    size_t *lengths;
    size_t count;

    tsm_split_message(message, length, &messages, &lengths, &count);

    for (size_t i = 0; i < count; i ++) {
        tox_friend_send_message(tox, friend_number, TOX_MESSAGE_TYPE_NORMAL,
                                messages[i], lengths[i], NULL);
    }
    
    free(messages);
    free(lengths);
}
```

##Build
See [INSTALL.md](INSTALL.md).

##Contribute
1. Find a thing to fix/implement in [Issues](issues?direction=desc&sort=created&state=open) or come up with your own idea, [create a discussion issue](issues/new) for it and get a feedback.
2. Make sure that no one is already working on it.
3. [Fork the repo](https://help.github.com/articles/fork-a-repo).
4. Create your feature branch (`git checkout -b my-new-feature`).
5. Commit your changes (`git commit -am 'Add some feature'`).
6. Push to the branch (`git push origin my-new-feature`).
7. [Create new Pull Request](https://help.github.com/articles/using-pull-requests).

##Contact
Leave nurupo an email on the address he uses in commits or ping `nurupo` on `#tox-dev` at Freenode IRC ([Freenode Webchat](https://webchat.freenode.net/?channels=#tox-dev), [KiwiIRC](https://kiwiirc.com/client/chat.freenode.net/?theme=basic#tox-dev)).

##License
This project is licensed under [Apache 2.0](LICENSE).