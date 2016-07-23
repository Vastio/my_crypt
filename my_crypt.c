/*****************************************************************************
 *
 *  C 2016 Seba <freevoyager@autistici.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ****************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <getopt.h>     // Command line option
#include <gcrypt.h>     // libgcrypt


#define VERSION "v0.1"

// Parts of gcrypt.h may be excluded by defining these macros
#define GCRYPT_NO_MPI_MACROS
#define GCRYPT_NO_DEPRECATED

// Supported Algorythms
#define AES256  GCRY_CIPHER_AES256
#define AES128  GCRY_CIPHER_AES128


// Define a struct for command line options
typedef struct Options {
    int algo;                 // Algorythm to use default AES256
    int encrypt;
    int decrypt;
    char *initVector;
    char *filename;           // File to encrypt/decrypt <-----ALLOCATE
} Opts;


// Function declaration
Opts* parseCommandLineOpts(int argc, char *argv[]);
void print_help(void);
char* getTextFromFile(char *filename);
char* getSecKey(void);




/* ********** *
 *    MAIN    *
 **************/
int main(int argc, char *argv[]) {

    Opts *opts;
    char *text;

    opts = parseCommandLineOpts(argc, argv);
    printf("\nProgram starting...\n");

	printf("Insert secret key to crypt/decrypt file: \n");
	if ((seckey = getSecKey()) == NULL) {
		fprintf(stderr, "Error in input secret key!\n");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", seckey);

    // Extract text from filename
    text = getTextFromFile(opts->filename);

    return 0;
} /*-*/



//
// Extract text from the file
char* getTextFromFile(char *filename) {

    int fd;
    struct stat st;
    char *text = NULL, error_buf[BUFSIZ];

    if ((fd = open(filename, O_RDONLY)) < 0) {
        strerror_r(errno, error_buf, BUFSIZ);
        fprintf(stderr, "Error to open file %s: %s\n", filename, error_buf);
        exit(EXIT_FAILURE);
    }

    // Check the size of the file
    if (fstat(fd, &st) < 0) {
        strerror_r(errno, error_buf, BUFSIZ);
        fprintf(stderr, "Error to  obtain info for file %s: %s\n", filename, error_buf);
        exit(EXIT_FAILURE);
    }

    // Allocate memory for var where store file content
    if ((text = (char *) malloc(sizeof(char) * st.st_size)) == NULL) {
        fprintf(stderr, "Error to allocate memory!\n");
        exit(EXIT_FAILURE);
    }

    if (read(fd, (char *) text, st.st_size) < 0) {
        strerror_r(errno, error_buf, BUFSIZ);
        fprintf(stderr, "Unable to read file: %s\n", error_buf);
        exit(EXIT_FAILURE);
    }
    close(fd);
    return text;
} /*-*/



//
// 
char* getSecKey(void) {
	int MAX_BUF = 25, index = 0;
    char *seckey = NULL, *tmp = NULL, ch;

    if ((seckey = (char *) malloc(MAX_BUF * sizeof(char))) == NULL) {
        fprintf(stderr, "Error in memory allocation!");
        exit(EXIT_FAILURE);
    }

    while (1) {
        ch = getc(stdin);

        if (ch == EOF || ch == '\n')
            break;

        // Increase the size of buffer
        if (index == MAX_BUF) {
            MAX_BUF += 5;

            if ((tmp = (char*) realloc(seckey, MAX_BUF * sizeof(char))) == NULL) {
                fprintf(stderr, "Error in memory allocation!");
                exit(EXIT_FAILURE);
            }
            seckey = tmp;
        }
        seckey[index++] = ch;
    }
    return seckey;
} /*-*/



//
// Parse command line options
Opts* parseCommandLineOpts(int argc, char *argv[]) {

    int opt = 0;
    Opts *opts;

    if ((opts = (struct Options *) malloc(sizeof(struct Options))) == NULL) {
        fprintf(stderr, "Error in memory allocation!");
        exit(EXIT_FAILURE);
    }

    // Setting default options
    opts->decrypt = 0;
    opts->encrypt = 1;
    opts->initVector = NULL;

    /** From man page **/

    // Long options descriptions
    static struct option longopts[] = {
        {"decript", required_argument,  0,  'd'},
        {"encrypt", required_argument, 0,   'e'},
        {"help",    no_argument, 0,  'h'},
        {"init-vector", required_argument, 0, 'I'},
        {"version", no_argument,    NULL,   'V'},
        {NULL,  0,  NULL,   0}
    };

    // Short options descriptions
    char *shortopts = "d:e:hI:V";

    while ((opt = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1) {

        switch (opt) {
            case 'd' :
                opts->decrypt = 1;
                opts->encrypt = 0;
                break;
            case 'e' :
                // Already defined above
                break;
            case 'h' :
                print_help();
                break;
            case 'I' :
                printf("-> To define <-\n");
                break;
            case 'V' :
                printf("\n%s %s\n\n", argv[0], VERSION);
                exit(EXIT_SUCCESS);
                break;
            default :
                print_help();
        }
    }

    if (optind < argc) {
        // Allocate space for filename
        int num = optind++;
        if ((opts->filename = (char *) malloc(sizeof(char *) * strlen(argv[num]) + 1)) == NULL) {
            fprintf(stderr, "Error in memory allocation!");
            exit(EXIT_FAILURE);
        }
        strncat(opts->filename, argv[num], sizeof(opts->filename));
    }
    else
        print_help();

    return opts;
} /*-*/



//
// Print help and exit
void print_help(void) {

    printf("\nUsage : my_crypt [opts] filename\n\n"
    "Options: \n"
    "   -d | --decrypt:         set decryption file.\n"
    "   -e | --encrypt ALGO:    set algorythm to encrypt file.\n"
    "   -h | --help:            print help and exit.\n"
    "   -I | --init-vector:     define an inizialization vector. ToDo\n"
    "   -V | --version:         print version and exit.\n\n"
    "Supported algorythms: AES128, AES256\n\n"
    "\n"
    );

    exit(EXIT_SUCCESS);
} /*-*/
