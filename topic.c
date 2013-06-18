/*

 TOPIC markup parser v1.02 [c]2013 Topcat Software LLC.

 |||(__) ^  FreeBSD TOPIC markup parser
  | (  )(   [c] 2013 Topcat Software LLC.
  |--  >_)  http://topcat.hypermart.net
  | _||_    All rights reserved.

 Redistribution and use in source and binary forms are permitted provided that
 the above  copyright notice  and this  paragraph are  duplicated in  all such
 forms and that any documentation,  advertising materials, and other materials
 related  to such  distribution  and  use acknowledge  that  the software  was
 developed by  Topcat Software LLC.  THIS SOFTWARE  IS PROVIDED ``AS  IS'' AND
 WITHOUT ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#include "markup.c"
#include "license.c"
#include "syntax.c"

#define MAX 32768  // maximum string size 32kb
#define TAG "TAG:" // record separator
#define SZE 4      // size of record separator

#ifndef BUILD
#define BUILD "undefined..."
#endif

/* ----------------------------------------------------------------------- */

char *trim(char *x) {

    char *y = strrchr(x, '\0');

    while (x < y && isspace(*x))       {++x;}
    while (y > x && isspace(*(y - 1))) {--y;}
    *y = '\0';

    return x;

}

/* ----------------------------------------------------------------------- */

int getindex(void) {

    register int x = 0;
    char buf[MAX]; memset(buf, 0, MAX);

    while (fgets(buf, MAX, stdin) != NULL) {
        if (strncasecmp(buf, TAG, SZE) == 0) {
            fputs(buf, stdout); x++;
        }
    }

    return (x > 0) ? 0 : 1;

}

/* ----------------------------------------------------------------------- */

int getblock(char *topic) {

    register int  x = 0, y = 0;
    char buf[MAX]; memset(buf, 0, MAX);
    char tmp[MAX]; memset(tmp, 0, MAX);
    char *tpc;

    while (fgets(buf, MAX, stdin) != NULL) {
        memset(tmp, 0, 1); strcpy(tmp, buf);
            if (strncasecmp(tmp, TAG, SZE) == 0) {
                y = 0;
                for (tpc = strtok(&tmp[SZE], ","); tpc; tpc = strtok(NULL, ",")) {
                    if (strcasecmp(trim(tpc), topic) == 0) {
                        x++; y = 1; tpc = NULL;
                    }
                }
            }
        if (y > 0) fputs(buf, stdout);
    }

    return (x > 0) ? 0 : 1;

}

/* ----------------------------------------------------------------------- */

int getblurb(int x, int y) {

    switch (x) {

       case 2: fputs((char *)markup_txt, stdout); break;

       case 1: fprintf(stdout, "%s%s%s", "\n BUILD: ", (char *)BUILD "\n", \
           (char *)license_txt); break;

       case 0: fputs((char *)syntax_txt, stdout); break;
    }

   return y;
}

/* ----------------------------------------------------------------------- */

int main(int argc, char *argv[]) {

    register int x = 0, y = (isatty(fileno(stdin) == 1)) ? 1 : 0;

    if (argc != 2) {
        x = (y == 1) ? getblurb(0, 0) : getblurb(0, 1);

    } else if (strcasecmp(argv[1], "-m") == 0) {
        x = (y == 1) ? getblurb(2, 0) : getblurb(0, 1);

    } else if (strcasecmp(argv[1], "-l") == 0) {
        x = (y == 1) ? getblurb(1, 0) : getblurb(0, 1);

    } else if (strcasecmp(argv[1], "-i") == 0) {
        x = (y == 1) ? getblurb(0, 1) :  getindex();

    } else {
        x = (y == 0) ? getblock(argv[1]) : getblurb(0, 1);
    }

    return x;

}

/* eof */

