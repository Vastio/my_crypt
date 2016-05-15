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
#include <gcrypt.h>     // libgcrypt

// Parts of gcrypt.h may be excluded by defining these macros
#define GCRYPT_NO_MPI_MACROS
#define GCRYPT_NO_DEPRECATED



//
// MAIN
int main(int argc, char *argv[]) {

    printf("Hello world\n");

    return 0;
}
