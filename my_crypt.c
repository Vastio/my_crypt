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
 */



#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>     // Command line option
#include <gcrypt.h>     // libgcrypt


// Parts of gcrypt.h may be excluded by defining these macros
#define GCRYPT_NO_MPI_MACROS
#define GCRYPT_NO_DEPRECATED
#define VERSION "v0.1"


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
char *insertSecKey(void);
int gcrypt(int algo, int crypt, char *seckey);



//
// MAIN
int main(int argc, char *argv[]) {

    Opts *opts;
    char *seckey;

    opts = parseCommandLineOpts(argc, argv);
    printf("\nProgram starting...\n");

    printf("Insert secret key to crypt/decrypt file: ");
    if ((seckey = insertSecKey()) == NULL) {
        fprintf(stderr, "Error in input secret key!\n");
        exit(EXIT_FAILURE);
    }

    // Init libgcrypt
    // https://gnupg.org/documentation/manuals/gcrypt/Initializing-the-library.html#Initializing-the-library
    if (!gcry_check_version(GCRYPT_VERSION)) {
        fprintf(stderr, "Error in libgcrypt: check version error!\n");
        exit(EXIT_FAILURE);
    }

    gcry_control(GCRYCTL_SUSPEND_SECMEM_WARN); // Suspend warn for secure memory
    gcry_control(GCRYCTL_INIT_SECMEM, 32768, 0); // Allocate 32kb of secmem
    gcry_control(GCRYCTL_RESUME_SECMEM_WARN);   // After allocation resume secmem warning
    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);   // Init OK

    // Crypt/decrypt function
    if (!gcrypt(opts->algo, opts->encrypt, seckey))
        exit(EXIT_FAILURE);

    return 0;
} /*-*/



//
// Function which encrypt/decrypt data
int gcrypt(int algo, int crypt, char *seckey) {

    gcry_cipher_hd_t gcry_hd;
    gcry_error_t err = 0;
    size_t seckey_len = gcry_cipher_get_algo_keylen(algo);

    err = gcry_cipher_open(&gcry_hd, algo, GCRY_CIPHER_MODE_CBC, GCRY_CIPHER_SECURE);
    if (err) {
        fprintf(stderr, "Error to init cipher handle: %s - %s\n", gcry_strerror(err), gcry_strsource(err));
        return 0;
    }

    err = gcry_cipher_setkey(gcry_hd, seckey, seckey_len);
    if (err) {
        fprintf(stderr, "Error to set secret key: %s - %s\n", gcry_strerror(err), gcry_strsource(err));
        return 0;
    }

    //err = grcy_cipher_setiv(gcry_hd, )

    return 1;
} /*-*/



//
// Insert secret key
char *insertSecKey() {

    int MAX_BUF = 10, index = 0;
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
                if ((opts->initVector = (char *) malloc(sizeof(char) * strlen(optarg + 1))) == NULL) {
                    fprintf(stderr, "Error in memory allocation!");
                    exit(EXIT_FAILURE);
                }
                strncpy(opts->initVector, optarg, strlen(opts->initVector));
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
        if ((opts->filename = (char *) malloc(sizeof(char) * strlen(argv[num] + 1))) == NULL) {
            fprintf(stderr, "Error in memory allocation!");
            exit(EXIT_FAILURE);
        }
        strncpy(opts->filename, argv[num], strlen(opts->filename));
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
    "   -d | --decrypt ALGO:    set algorythm to decrypt file.\n"
    "   -e | --encrypt ALGO:    set algorythm to encrypt file.\n"
    "   -h | --help:            print help and exit.\n"
    "   -I | --init-vector:     define an inizialization vector.\n"
    "   -V | --version:         print version and exit.\n\n"
    "Supported algorythms: AES128, AES256\n\n"
    "\n"
    );

    exit(EXIT_SUCCESS);
} /*-*/
