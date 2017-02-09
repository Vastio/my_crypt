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
#include <getopt.h>           // For command line options
#include <gcrypt.h>           // libgcrypt


#define VERSION "v0.1"

// Parts of gcrypt.h may be excluded by defining these macros
#define GCRYPT_NO_MPI_MACROS
#define GCRYPT_NO_DEPRECATED

// Supported Algorythms
#define AES256 GCRY_CIPHER_AES256
#define AES256_BLOCK_SIZE 16


typedef struct Options {
     int algo;             // Algorythm to use default AES256
     int encrypt;          // Encrypt file
     int decrypt;          // Deccrypt file 
     char *filename;
} Opts;



// Function declaration
char* getSecKey(void);
Opts* parseCommandLineOpts(int argc, char *argv[]);
void print_help(void);
char* aes256Encrypt(int algo, char *sec_key, char *plain_text);



//
// MAIN
int main(int argc, char *argv[]) {
     
     Opts *cmd_opts;
     char *sec_key, *crypt_text;
     char *clean_text = "Ciao a tutto il mondo sono criptato";

     cmd_opts = parseCommandLineOpts(argc, argv);
     printf("\n[*] Program starting...\n");
     
     //
     // INIT libgcrypt
     printf("[*] Init libgcrypt...\n");
     if (!gcry_check_version(GCRYPT_VERSION)) {
	  fprintf(stderr, "[!] Error in libgcrypt: check version error!\n");
	  exit(EXIT_FAILURE);
     }
     gcry_control (GCRYCTL_SUSPEND_SECMEM_WARN);         // Suspend warn for secure memory
     gcry_control (GCRYCTL_INIT_SECMEM, 65536, 0);       // Allocate 64kb of secmem
     gcry_control (GCRYCTL_RESUME_SECMEM_WARN);
     gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);   // Init OK

     // Get secret key
     printf("[<>] Insert secret key to crypt/decrypt file: \n");
     if ((sec_key = getSecKey()) == NULL) {
	  fprintf(stderr, "[!] Error in input secret key!\n");
	  exit(EXIT_FAILURE);
     }

     if (cmd_opts->encrypt == 1) {
	  printf("[<>] Encrypt file...\n");
	  if ((crypt_text = aes256Encrypt(AES256, sec_key, clean_text)) == NULL)
	       exit(EXIT_FAILURE);
	  
	  printf("Text: %s\n", crypt_text);
     }
     else if (cmd_opts->decrypt == 1) {
	  printf("[<>] Decrypt file...\n");
     }
     
     return 0;
} /*-*/



//
// Encrypt plain_text with AES 256 algo
// Manual: https://gnupg.org/documentation/manuals/gcrypt/Working-with-cipher-handles.html#Working-with-cipher-handles
char* aes256Encrypt(int algo, char *sec_key, char *plain_text) {

     gcry_error_t err;
     gcry_cipher_hd_t hd;
     size_t key_len =  gcry_cipher_get_algo_keylen(algo);
     size_t blk_len = gcry_cipher_get_algo_blklen(algo);
     unsigned char init_vector[blk_len];

     err = gcry_cipher_open(&hd, algo, GCRY_CIPHER_MODE_CBC, GCRY_CIPHER_CBC_CTS);
     if (err) {
	  fprintf(stderr, "[!] Failure cipher_open %s/%s\n", gcry_strsource(err), gcry_strerror(err));
	  return NULL;
     }
     
     err = gcry_cipher_setkey(hd, sec_key, key_len);  
     if (err) {
	  fprintf(stderr, "[!] Failure cipher_setkey %s/%s\n", gcry_strsource(err), gcry_strerror(err));
	  return NULL;
     }

     
     
     gcry_cipher_close(hd);
     
     return NULL;
} /*-*/



//
// 
char *getSecKey(void) {

int MAX_BUF = 25, index = 0;
     char *sec_key = NULL, *tmp_key = NULL, ch;

     // Allocate buffer using gcrypt secure malloc
     if ((sec_key = (char *) gcry_malloc_secure(sizeof(char) * MAX_BUF)) == NULL) {
	  fprintf(stderr, "[!] Error to allocate buffer for secure key.\n");
	  exit(EXIT_FAILURE);
     }

     while(1) {
	  
	  // 0 is stdin
	  read(0, &ch, sizeof(char) *1);

	  if (ch == EOF || ch == '\n')
	       break;

	  // Increase buffer
	  if (index == MAX_BUF) {
	       
	       MAX_BUF += 5;

	       if ((tmp_key = (char *) gcry_realloc(sec_key, sizeof(char) * MAX_BUF)) == NULL) {
		    fprintf(stderr, "[!] Error to allocate buffer for secure key.\n");
		    exit(EXIT_FAILURE);
	       }
	       sec_key = tmp_key;	       
	  }
	  
	  sec_key[index++] = ch;
     }
     sec_key[index++] = '\0';
     
     return sec_key;
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
	  strncat(opts->filename, argv[num], (size_t) sizeof(opts->filename));
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
	    "Supported algorythms: AES256.\n\n"
	    "\n"
	  );
     
     exit(EXIT_SUCCESS);
} /*-*/
