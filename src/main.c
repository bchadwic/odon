#include "../include/send.h"
#include "../include/recv.h"
#include "../include/show.h"
#include "../include/errors.h"
#include "../include/fmt.h"

struct command
{
    const char *name;
    int (*fn)(int argc, char **argv);
};

const struct command cmds[] = {
    {.name = "send", .fn = send_cmd},
    {.name = "recv", .fn = recv_cmd},
    {.name = "show", .fn = show_cmd},
};

static int run(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    const char *bin = *argv;
    argc--;
    argv++;

    int err = run(argc, argv);
    if (err != OK)
    {
        odon_perror(bin, err);
        return 1;
    }
    return 0;
}

int run(int argc, char *argv[])
{
    if (argc < 1)
    {
        return ERR_NO_CMD;
    }

    const char *cmd = *argv;
    argc--;
    argv++;

    int cmd_list = sizeof(cmds) / sizeof(cmds[0]);
    for (int i = 0; i < cmd_list; i++)
    {
        if (strcmp(cmd, cmds[i].name) == 0)
        {
            return cmds[i].fn(argc, argv);
        }
    }
    return ERR_INVALID_CMD;
}
