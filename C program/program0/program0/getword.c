//
//  getword.c
//  program0
//
//  Created by LeonZ on 8/26/21.
//
//goal:
//remove leading space
//remove backslash
//newline return 0
//EOF if size = 0, and return -1
//return word size, and get the word seperate
// $ sign is count

#include "getword.h"


int getword(char *w)
{
    int ch1;
    int ch2;
    int count = 0;
    int need_escape = 0;

    for (;;) {
        if ((ch2 = getchar()) == EOF) {
            if (count == 0) {
                w[0] = 0;
                return -1;
            }

            break;
        }


        if (ch2 == '\n') {
            if (count > 0) {
                ungetc(ch2, stdin);
            }

            break;
        }

        if (count >= STORAGE) {
            /* maximum */
            break;
        }

        if (count == 0) {
            if (ch2 == ' ') {
                /* skip leading spaces */
                continue;
            }

            if (ch2 == '\\') {
                /* got a backslash, skip */
                continue;
            }

            ch1 = ch2;
            w[count] = ch2;
            count = count + 1;
        } else {
            if (ch2 == ' ') {
                if (need_escape == 0) {
                    break;
                }

                ch1 = ch2;
                w[count] = ch2;
                need_escape = 0;
                count = count + 1;
                continue;
            }

            if (ch2 == '\\') {
                /* escape */
                need_escape = 1;
                continue;
            }


            if (ch1 == ch2) {
                if (need_escape == 0) {
                    ungetc(ch2, stdin);
                    break;
                }

                /* escape */
                w[count] = ch2;
                need_escape = 0;
                count = count + 1;
                continue;
            }

            ch1 = ch2;
            w[count] = ch2;
            count = count + 1;
            if (need_escape == 1) {
                need_escape = 0;
            }
        }
    }

    w[count] = 0;

    return count;
}
