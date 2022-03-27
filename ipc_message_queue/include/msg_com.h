/* A common header file to describe the message we wish to send/receive. */

#define TEXT_SZ 256

struct my_message {
    long message_type;
    char text[TEXT_SZ];
};

