#include "../include/send.h"
#include "../include/recv.h"
#include "../include/show.h"

static int run(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    if (run(argc, argv) < 0)
    {
        if (errno != 0)
        {
            perror("run");
        }
        return 1;
    }
    return 0;
}

int run(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "command not provided\n");
        return -1;
    }

    struct sockaddr_in a1 = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_LOOPBACK),
        .sin_port = htons(52888),
    };

    struct sockaddr_in a2 = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_LOOPBACK),
        .sin_port = htons(52887),
    };

    if (strcmp(argv[1], "send") == 0)
    {
        if (argc < 3)
        {
            fprintf(stderr, "send needs 1 arg\n");
            return -1;
        }
        return send_cmd(&a1, sizeof(a1), &a2, sizeof(a2), argv[2]);
    }
    else if (strcmp(argv[1], "recv") == 0)
    {
        if (argc < 3)
        {
            fprintf(stderr, "recv needs 1 arg\n");
            return -1;
        }
        return recv_cmd(&a2, sizeof(a2), &a1, sizeof(a1), argv[2]);
    }
    else
    {
        return show_cmd();
    }
    return 0;
}
