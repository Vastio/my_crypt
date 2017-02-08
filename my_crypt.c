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


typedef struct Options {
  int algo;             // Algorythm to use default AES256
  int encrypt;          // 1 Encrypt file, 0 decrypt file
  char *filename;
} Opts;



// Function declaration
Opts* parseCommandLineOpts(int argc, char *argv[]);
void print_help(void);



//
// MAIN
int main(int argc, char*argv[]){

  Opts *cmd_opts;

  cmd_opts = parseCommandLineOpts(argc, argv);
  printf("\n[*] Program starting...\n");
  
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
	 "   -I | --init-vector:     define an inizialization vector. ToDo\n"
	 "   -k | --key:             generate key to decrypt file. ToDo\n"
	 "   -V | --version:         print version and exit.\n\n"
	 "Supported algorythms: AES256.\n\n"
	 "\n"
	 );

    exit(EXIT_SUCCESS);
} /*-*/
