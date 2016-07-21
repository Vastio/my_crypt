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
#include <getopt.h>     // Command line option
#include <gcrypt.h>     // libgcrypt


#define version "v0.1"


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




/* ********** *
 *    MAIN    *
 **************/
int main(int argc, char *argv[]) {

    Opts *opts;

    opts = parseCommandLineOpts(argc, argv);
    printf("\nProgram starting...\n");

    return 0;
 } /*-*/


 //
 // Print help and exit
 void print_help(void) {

     printf("\nUsage : my_crypt [opts] filename\n\n"
     "Options: \n"
     "   -d | --decrypt:         set decryption file.\n"
     "   -e | --encrypt ALGO:    set algorythm to encrypt file.\n"
     "   -h | --help:            print help and exit.\n"
     "   -I | --init-vector:     define an inizialization vector.\n"
     "   -V | --version:         print version and exit.\n\n"
     "Supported algorythms: AES128, AES256\n\n"
     "\n"
     );

     exit(EXIT_SUCCESS);
 } /*-*/
