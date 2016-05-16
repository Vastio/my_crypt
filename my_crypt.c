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
#include <getopt.h>     // Command line option
#include <gcrypt.h>     // libgcrypt

// Parts of gcrypt.h may be excluded by defining these macros
#define GCRYPT_NO_MPI_MACROS
#define GCRYPT_NO_DEPRECATED
#define VERSION "v0.1"



// Define a struct for command line options
typedef struct Options {
    char *algo;         // Algorythm to use default AES256
    char *filename;     // File to encrypt/decrypt <-----ALLOCATE
} Opts;


// Function declaration
Opts* parseCommandLineOpts(int argc, char *argv[]);
void print_help(void);



//
// MAIN
int main(int argc, char *argv[]) {

    Opts *opts;

    opts = parseCommandLineOpts(argc, argv);
    printf("\nProgram starting...\n");

    return 0;
} /*-*/



//
// Parse command line options
Opts* parseCommandLineOpts(int argc, char *argv[]) {

    int opt = 0;
    Opts *opts;

    /** From man page **/

    // Long options descriptions
    static struct option longopts[] = {
        {"decript", required_argument,  0,  'd'},
        {"encrypt", required_argument, 0,   'e'},
        {"help",    no_argument, 0,  'h'},
        {"version", no_argument,    NULL,   'V'},
        {NULL,  0,  NULL,   0}
    };

    // Short options descriptions
    char *shortopts = "d:e:hV";

    while ((opt = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1) {

        switch (opt) {
            case 'd' :
                printf("Decrypt\n");
                break;
            case 'e' :
                printf("Encrypt\n");
                break;
            case 'h' :
                print_help();
                break;
            case 'V' :
                printf("\n%s %s\n\n", argv[0], VERSION);
                exit(EXIT_SUCCESS);
                break;
            default :
                print_help();
        }
    }

    return opts;
} /*-*/



//
// Print help and exit
void print_help(void) {

    printf("\nUsage : my_crypt [opts] filename\n\n"
    "Options: \n"
    "   -d | --decrypt ALGO:    algorythm to decrypt file.\n"
    "   -e | --encrypt ALGO:    algorythm to encrypt file.\n"
    "   -h | --help:            print help and exit.\n"
    "   -V | --version:         print version and exit.\n"
    "\n\n"
    );

    exit(EXIT_SUCCESS);
} /*-*/
