#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#include <pthread.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/bio.h>

#define SIZE 8192
#define MAX_TASKS 10

typedef struct {
    pthread_t tasks[MAX_TASKS];
    BIO *key_file;
    char key_content[SIZE];
    bool found;
    time_t st;
} rsa_work_t;

/* global variables */
rsa_work_t work;
char *key_path;
//pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void siginfo( __attribute__((unused))int s) {
    printf("key: %s, tasks: %d\n", key_path, MAX_TASKS);
    
    /* lazy calculation */
    printf("time since started: ");
    time_t c = time(NULL);
    long int m = 0,ss = (c - work.st);
    if ( ss > 60 ) {
        m = ss/60;
        ss = (ss - (m*60));
        printf("%ld minutes, ", m);
    }
    printf("%ld seconds\n", ss);
}

void check_pwd(void) {
	char *ptr;
    char pwd[SIZE];
	BIO *key_mem = BIO_new_mem_buf(work.key_content, -1);

	for (;;) {
        if (work.found)
            break;
        //pthread_mutex_lock(&mtx);
        if (!fgets(pwd, SIZE, stdin)) {
            //pthread_mutex_unlock(&mtx);
            break;
        }
        //pthread_mutex_unlock(&mtx);

        ptr = pwd;
        while (*ptr != '\n' && *ptr != '\0')
            ptr++;
        *ptr = '\0';

        if (
                (PEM_read_bio_PrivateKey(key_mem, NULL, NULL, pwd)) != NULL
        ) {
            work.found = true;
            printf("Passphrase is: %s\n", pwd);
            break;
        }
        BIO_reset(key_mem);
	} 
	
    if (key_mem)
		BIO_free(key_mem);
    pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
    short int ret = EXIT_FAILURE;

	FILE *fp;
	if (argc < 2) {
		fprintf(stderr, "Usage:\n\t%s <rsa key .pem>\n\n", argv[0]);
		exit(EXIT_FAILURE);
	}

    memset(&work, 0, sizeof(rsa_work_t));

    /* catch INFO signal and show current cracking information */
    signal(SIGINFO, siginfo);
    
    key_path = argv[1];
	fp = fopen(key_path, "r");
    if (fp == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    work.st = time(NULL);

	memset(work.key_content, 0x00, SIZE);
	SSL_library_init();

	work.key_file = BIO_new(BIO_s_file());
	BIO_set_fp(work.key_file, fp, BIO_CLOSE);
	BIO_read(work.key_file, work.key_content, SIZE);
	BIO_free(work.key_file);

    work.found = false;
    for (int ti = 0;ti <= MAX_TASKS;ti++) {
        pthread_create(&work.tasks[ti], NULL, (void *)&check_pwd, NULL);
    }
    for (int ti = 0;ti <= MAX_TASKS;ti++) {
        pthread_join(work.tasks[ti], NULL);
    }

    if (work.found) {
        ret = EXIT_SUCCESS;
    } else {
	    printf("Passphrase not found.\n");
    }

	return ret;
}

