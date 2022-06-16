#include <stdio.h>     /*  fprintf()                    */
#include <unistd.h>    /*  sleep()                      */
#include <getopt.h>    /*  get_opt_long()               */
#include <stdlib.h>    /*  strtol() 			        */
#include <string.h>    /*  strcmp(), strcpy()	        */
#include <sys/types.h> /*  Primitive System Data Types  */
#include <sys/ipc.h>   /*  Interprocess communication   */
#include <sys/msg.h>   /*  Message queue			    */

#include "msq-common.h"
#include "msq-flower.h"

#define USAGE "Usage: ./msq-client.out [OPTION]..."
#define VERSION 20220328
#define msq_SIZE 1024

int main(int argc, char *argv[])
{
    int opt;
    int i;

    char *color = "Default color";
    int proj_id = 1;
    char *name = "Default name";
    char *pathname = "file.ftok";
    int seconds = 1;
    int tries = -1;

    key_t key;
    int msqid;
    msq_flower_t flower;
    msq_flower_message_t message;

    static struct option long_options[] = {
        {"flower-color", required_argument, 0, 'c'},
        {"help", no_argument, 0, 'h'},
        {"key-proj-id", required_argument, 0, 'i'},
        {"flower-name", required_argument, 0, 'n'},
        {"key-pathname", required_argument, 0, 'p'},
        {"seconds", required_argument, 0, 's'},
        {"tries", required_argument, 0, 't'},
        {"version", no_argument, 0, 'v'}};

    while ((opt = getopt_long(argc, argv, "hc:i:n:p:s:t:v", long_options, NULL)) != EOF)
        switch (opt)
        {
        case 'h':
            printf("%s\n", USAGE);
            printf("Send a flower to a server through a message queue.\n\n");
            printf("Options:\n");

            printf("\t-c, --flower-color=COLOR\n"
                   "\t\tset the flower color to COLOR (the default value is \"Default color\")\n");

            printf("\t-h, --help\n"
                   "\t\tdisplay this help and exit\n");

            printf("\t-i, --key-proj-id=PROJ_ID\n"
                   "\t\tset the key project identifier to PROJ_ID (the default value is \"1\")\n");

            printf("\t-n, --flower-name=NAME\n"
                   "\t\tset the flower name to NAME (the default value is \"Default name\")\n");

            printf("\t-p, --key-pathname=PATHNAME\n"
                   "\t\tset the key pathname to PATHNAME (the default value is \"file.ftok\")\n");

            printf("\t-s, --seconds=SECONDS\n"
                   "\t\tset the seconds beween each try (the default value is \"1\", a value less than or equal to 0 enables the interactive mode where the input stream is read)\n");

            printf("\t-t, --tries=TRIES\n"
                   "\t\tset the number of tries to TRIES (the default value is \"-1\", a negative value means repeat for ever)\n");

            printf("\t-v, --version\n");
            printf("\t\toutput version information and exit\n\n");

            printf("Report bugs to Frédéric Borri <fborri@etud.univ-pau.fr> and Quentin Michelix <qmichelix@etud.univ-pau.fr>.\n");
            return (0);

        case 'c':
            color = optarg;
            break;

        case 'i':
            proj_id = strtol(optarg, NULL, 10);
            break;

        case 'n':
            name = optarg;
            break;

        case 'p':
            pathname = optarg;
            break;

        case 's':
            seconds = strtol(optarg, NULL, 10);
            break;

        case 't':
            tries = strtol(optarg, NULL, 10);
            break;

        case 'v':
            printf("msq-client %d\n\n", VERSION);
            printf("Copyright (C) 2022 Frédéric Borri and Quentin Michelix.\n\n");
            printf("Written by Frédéric Borri <fborri@etud.univ-pau.fr> and Quentin Michelix <qmichelix@etud.univ-pau.fr>.\n");
            return (0);

        default:
            fprintf(stderr, "%s\n", USAGE);
        }

    if (msq_flower_set_color(&flower, color) == msq_flower_error_color)
    {
        fprintf(stderr, "%s:%s:%d: Unable to set the flower color to \"%s\"\n", argv[0], __FILE__, __LINE__, color);
        return 1;
    }

    if (msq_flower_set_name(&flower, name) == msq_flower_error_name)
    {
        fprintf(stderr, "%s:%s:%d: Unable to set the flower name to \"%s\"\n", argv[0], __FILE__, __LINE__, name);
        return 1;
    }

    print_settings(pathname, proj_id);

    if (create_key(pathname, proj_id, &key))
    {
        fprintf(stderr, "%s:%s:%d: Unable to create the System V IPC key from the \"%s\" pathname and the \"%d\" project identifier.\n", argv[0], __FILE__, __LINE__, pathname, proj_id);
        return -1;
    }

    if ((msqid = msgget(key, 0666)) == -1)
    {
        fprintf(stderr, "%s:%s:%d: Unable to get the identifier of the System V message queue from the \"0x%08x\" key.\n", argv[0], __FILE__, __LINE__, key);
        return 1;
    }

    if (tries < 0)
    {
        while (1)
        {
            msq_flower_serialize(flower, &message);
            if (msgsnd(msqid, &message, MSQ_FLOWER_COLOR_SIZE + MSQ_FLOWER_NAME_SIZE, 0) == -1)
            {
                fprintf(stderr, "%s:%s:%d: Unable to send the message into the message queue.\n", argv[0], __FILE__, __LINE__);
                return 1;
            }
            msq_flower_print(flower);

            if (seconds <= 0)
            {
                printf("Press the Enter key to continue...");
                while (getchar() != 10)
                    ;
            }
            else
                sleep(seconds);
        }
    }
    else
    {
        for (i = 0; i < tries; i++)
        {

            msq_flower_serialize(flower, &message);
            if (msgsnd(msqid, &message, MSQ_FLOWER_COLOR_SIZE + MSQ_FLOWER_NAME_SIZE, 0) == -1)
            {
                fprintf(stderr, "%s:%s:%d: Unable to send the message into the message queue.\n", argv[0], __FILE__, __LINE__);
                return 1;
            }
            msq_flower_print(flower);

            if (seconds <= 0)
            {
                printf("Press the Enter key to continue...");
                while (getchar() != 10)
                    ;
            }
            else if (i == tries - 1)
                return 0;
            else
                sleep(seconds);
        }
    }

    return 0;
}