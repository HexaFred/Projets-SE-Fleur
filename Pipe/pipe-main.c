#include <stdio.h>     /*  fprintf()                    */
#include <unistd.h>    /*  sleep()                      */
#include <getopt.h>    /*  get_opt_long()               */
#include <stdlib.h>    /*  strtol() 			        */
#include <string.h>    /*  strcmp(), strcpy()	        */
#include <sys/types.h> /*  Primitive System Data Types  */
#include <sys/ipc.h>   /*  Interprocess communication   */
#include <fcntl.h>

#include "pipe-common.h"
#include "pipe-flower.h"

#define USAGE "Usage: ./pipe-main.out [OPTION]..."

int main(int argc, char *argv[])
{
    int opt;

    char *color = "Default color";
    char *name = "Default name";
    int seconds_s = 1;
    int tries_s = 1;
    int seconds_r = 1;
    int tries_r = -1;

    pipe_flower_t flower;
    pipe_flower_t new_flower;
    pid_t cpid;

    int fd[2];
    char prefix[PREFIX_SIZE];

    static struct option long_options[] = {
        {"flower-color", required_argument, 0, 'c'},
        {"help", no_argument, 0, 'h'},
        {"flower-name", required_argument, 0, 'n'},
        {"seconds-for-receiving", required_argument, 0, 'r'},
        {"seconds-for-sending", required_argument, 0, 's'},
        {"tries-for-receiving", required_argument, 0, 't'},
        {"tries-for-sending", required_argument, 0, 'u'},
        {"version", no_argument, 0, 'v'}};

    while ((opt = getopt_long(argc, argv, "hc:r:n:s:t:u:v", long_options, NULL)) != EOF)
        switch (opt)
        {
        case 'h':
            printf("%s\n", USAGE);
            printf("Send and receive flowers through a pipe.\n\n");
            printf("Options:\n");

            printf("\t-c, --flower-color=COLOR\n"
                   "\t\tset the flower color to COLOR (the default value is \"Default color\")\n");

            printf("\t-h, --help\n"
                   "\t\tdisplay this help and exit\n");

            printf("\t-n, --flower-name=NAME\n"
                   "\t\tset the flower name to NAME (the default value is \"Default name\")\n");

            printf("\t-r, --seconds-for-receiving=SECONDS\n"
                   "\t\tset the seconds between each try for receiving to SECONDS (the default value is \"1\", a value less than or equal to 0 enables the interactive mode where the input stream is read)\n");

            printf("\t-s, --seconds-for-sending=SECONDS\n"
                   "\t\tset the seconds between each try for sending to SECONDS (the default value is \"1\", a value less than or equal to 0 enables the interactive mode where the input stream is read)\n");

            printf("\t-t, --tries-for-receiving=TRIES\n"
                   "\t\tset the number of tries for receiving to TRIES (the default value is \"-1\", a negative value means repeat for ever)\n");

            printf("\t-u, --tries-for-sending=TRIES\n"
                   "\t\tset the number of tries for sending to TRIES (the default value is \"1\", a negative value means repeat for ever)\n");

            printf("\t-v, --version\n");
            printf("\t\toutput version information and exit\n\n");

            printf("Report bugs to Frédéric Borri <fborri@etud.univ-pau.fr> and Quentin Michelix <qmichelix@etud.univ-pau.fr>.\n");
            return (0);

        case 'c':
            color = optarg;
            break;

        case 'n':
            name = optarg;
            break;

        case 'r':
            seconds_r = strtol(optarg, NULL, 10);
            break;

        case 's':
            seconds_s = strtol(optarg, NULL, 10);
            break;

        case 't':
            tries_r = strtol(optarg, NULL, 10);
            break;

        case 'u':
            tries_s = strtol(optarg, NULL, 10);
            break;

        case 'v':
            printv();
            return (0);

        default:
            fprintf(stderr, "%s\n", USAGE);
        }

    if (pipe_flower_set_color(&flower, color) == pipe_flower_error_color)
    {
        fprintf(stderr, "%s:%s:%d: Unable to set the flower color to \"%s\"\n", argv[0], __FILE__, __LINE__, color);
        return 1;
    }

    if (pipe_flower_set_name(&flower, name) == pipe_flower_error_name)
    {
        fprintf(stderr, "%s:%s:%d: Unable to set the flower name to \"%s\"\n", argv[0], __FILE__, __LINE__, name);
        return 1;
    }

    if (pipe2(fd, O_CLOEXEC | O_NONBLOCK) == -1)
    {
        fprintf(stderr, "%s:%s:%d: Unable to create the pipe.\n", argv[0], __FILE__, __LINE__);
        return 1;
    }

    cpid = fork();
    if (cpid == -1)
    {
        fprintf(stderr, "%s:%s:%d: Unable to create a child process.\n", argv[0], __FILE__, __LINE__);
        return 1;
    }
    else if (cpid > 0)
    {
        if (close(fd[1]) == -1)
        {
            fprintf(stderr, "%s:%s:%d: Unable to close the writing end of the pipe in the parent process.\n", argv[0], __FILE__, __LINE__);
            return 1;
        }
        while (tries_r != 0)
        {
            if (seconds_r <= 0)
            {
                printf("Press the Enter key to read...\n");
                while (getchar() != '\n')
                    ;
            }

            if (read(fd[0], &new_flower, PIPE_FLOWER_COLOR_SIZE + PIPE_FLOWER_NAME_SIZE) > 0)
            {
                if (sprintf(prefix, "%d read", getpid()) < 0)
                {
                    fprintf(stderr, "%s:%s:%d: Unable to create the prefix in the parent process.\n", argv[0], __FILE__, __LINE__);
                    return 1;
                }
                pipe_flower_print(new_flower, prefix);
            }

            if (tries_r > 0)
                tries_r--;

            sleep(seconds_r);
        }
        if (close(fd[0]) == -1)
        {
            fprintf(stderr, "%s:%s:%d: Unable to close the reading end of the pipe in the parent process.\n", argv[0], __FILE__, __LINE__);
            return 1;
        }
    }
    else
    {
        if (close(fd[0]) == -1)
        {
            fprintf(stderr, "%s:%s:%d: Unable to close the reading end of the pipe in the child process.\n", argv[0], __FILE__, __LINE__);
            return 1;
        }

        while (tries_s != 0)
        {
            if (write(fd[1], &flower, PIPE_FLOWER_COLOR_SIZE + PIPE_FLOWER_NAME_SIZE) == -1)
            {
                fprintf(stderr, "%s:%s:%d: Unable to write in the pipe.\n", argv[0], __FILE__, __LINE__);
                return 1;
            }
            else
            {
                if (sprintf(prefix, "%d wrote", getpid()) < 0)
                {
                    fprintf(stderr, "%s:%s:%d: Unable to create the prefix in the child process.\n", argv[0], __FILE__, __LINE__);
                    return 1;
                }
                pipe_flower_print(flower, prefix);
            }

            if (seconds_s <= 0)
            {
                printf("Press the Enter key to write...\n");
                while (getchar() != '\n')
                    ;
            }

            if (tries_s > 0)
                tries_s--;

            sleep(seconds_s);
        }

        if (close(fd[1]) == -1)
        {
            fprintf(stderr, "%s:%s:%d: Unable to close the writing end of the pipe in the child process.\n", argv[0], __FILE__, __LINE__);
            return 1;
        }
        return 0;
    }

    return 0;
}