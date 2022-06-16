#include <stdio.h>     /*  fprintf()                    */
#include <unistd.h>    /*  sleep()                      */
#include <getopt.h>    /*  get_opt_long()               */
#include <stdlib.h>    /*  strtol() 			        */
#include <string.h>    /*  strcmp(), strcpy()	        */
#include <sys/types.h> /** Primitive System Data Types  */
#include <sys/ipc.h>   /*  Interprocess communication   */
#include <sys/shm.h>   /* 	Shared memory			   **/

#include "shm-common.h"
#include "shm-flower.h"

#define USAGE "Usage: ./shm-server.out [OPTION]..."
#define SHM_SIZE 1024

int main(int argc, char *argv[])
{
    int opt;
    int i;

    int proj_id = 1;
    char *pathname = "file.ftok";
    int seconds = 1;
    int tries =-1;

    key_t key;
	int shmid;
	shm_flower_t flower, *shmaddr;

    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"key-proj-id", required_argument, 0, 'i'},
        {"key-pathname", required_argument, 0, 'p'},
        {"seconds", required_argument, 0, 's'},
        {"tries", required_argument, 0, 't'},
        {"version", no_argument, 0, 'v'}};

    while ((opt = getopt_long(argc, argv, "hi:p:s:t:v", long_options, NULL)) != EOF)
        switch (opt)
        {
        case 'h':
            printf("%s\n", USAGE);
            printf("Receive flowers from clients through shared memory.\n\n");

            printf("Options:\n");

            printf("\t-h, --help\n"
                   "\t\tdisplay this help and exit\n");

            printf("\t-i, --key-proj-id=PROJ_ID\n"
                   "\t\tset the key project identifier to PROJ_ID (the default value is \"1\")\n");

            printf("\t-p, --key-pathname=PATHNAME\n"
                   "\t\tset the key pathname to PATHNAME (the default value is \"file.ftok\")\n");

            printf("\t-s, --seconds=SECONDS\n"
                   "\t\tset the seconds beween each try (the default value is \"1\", a value less than or equal to 0 enables the interactive mode where the input stream is read)\n");

            printf("\t-t, --tries=TRIES\n"
                   "\t\tset the number of tries to TRIES (the default value is \"-1\", a negative value means repeat for ever)\n");

            printf("\t-v, --version\n");
            printf("\t\toutput version information and exit\n\n");

            printf("Report bugs to Frédéric Borri <fborri@univ-pau.fr> and Quentin Michelix <qmichelix@univ-pau.fr>.\n");
            return (0);

        case 'i':
            proj_id = strtol(optarg, NULL, 10);
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
            printf("shm-server 20220218\n\n");
            printf("Copyright (C) 2022 Frédéric Borri and Quentin Michelix.\n\n");
            printf("Written by Frédéric Borri <fborri@univ-pau.fr> and Quentin Michelix <qmichelix@univ-pau.fr>.\n");
            return(0);

        default:
            fprintf(stderr, "%s\n", USAGE);
        }

    print_settings(pathname,proj_id);

    if(create_key(pathname, proj_id, &key)) {
		fprintf(stderr, "%s:%s:%d: Unable to create the System V IPC key from the \"%s\" pathname and the \"%d\" project identifier.\n", argv[0], __FILE__, __LINE__, pathname, proj_id);
		return -1;
	}

	if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
		fprintf(stderr, "%s:%s:%d: Unable to get the identifier of the System V shared memory segment from the \"0x%08x\" key.\n", argv[0], __FILE__, __LINE__, key);
		return -1;
	}

	if ((shmaddr = shmat(shmid, NULL, 0)) == (shm_flower_t *) -1) {
		fprintf(stderr, "%s:%s:%d: Unable to attach the System V shared memory segment identified by \"%d\" to the address space of the calling process.", argv[0], __FILE__ , __LINE__ , shmid);
		return -1;
	}

	if(tries>=0) {
		for(i=0; i<tries; i++){
			shm_flower_empty(&flower);
			shm_flower_copy(*shmaddr, &flower);

			if(shm_flower_is_empty(flower)==0) {
				shm_flower_print(flower);
				shm_flower_empty(shmaddr);
			}

			if(seconds<=0) {
				do {
					printf("Press the Enter key to continue...");
				} while(getchar() != 10);
			} 
            
			else 
				sleep(seconds);
		}
	} else {
		while(1){
			shm_flower_empty(&flower);
			shm_flower_copy(*shmaddr, &flower);

			if(shm_flower_is_empty(flower)==0) {
				shm_flower_print(flower);
				shm_flower_empty(shmaddr);
			}

			if(seconds<=0) {
				do {
					printf("Press the Enter key to continue...");
				} while(getchar() != 10);

			} else 
				sleep(seconds);
		}
	}

	shmctl(shmid, IPC_RMID, 0);

    return (0);
}