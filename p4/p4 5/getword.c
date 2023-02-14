//Name: Chonglin ZHang
//RedID: 822942790
//Class account: cssc0094
//Course: CS480 Operating Systems
//Instructor: John Carroll
//Due date: 8 31, 2021

//purpose:
//The getword() function gets one word from the input stream.
//It returns -1 iff end-of-file is encountered;
//otherwise, it returns the number of characters in the word.
//The metacharacters are "<", ">", "|", ";", "'", "&", and the combination ">!" .
//The last word on a line may be terminated by the newline character OR by
//end-of-file.
//what is the program 1 should do:
//1.getword() skips leading blanks
//2.The backslash character "\" is special, and may change the behavior of
//the character that directly follows it on the input line.
//3.three calls applied to the input -> Null>!void ->return 4,2,4
//4.A backslash preceding any other character should simply be ignored;
//in particular, a backslash before a newline will NOT affect the meaning of that
//newline.
//5. Allow spaces inside a word is to use pairs of single quotes
//For example: 'Null void' -> return 9

#include "getword.h"



#define N_META_CHAR 7
char* meta_chars[N_META_CHAR] = {
    "<",
    ">",
    "|",
    ";",
    "'",
    "&",
    ">!"
};

int is_meta_chars(char* chs, int n)
{
    int i = 0;
    for (; i < N_META_CHAR; ++i) {
        if (strncmp(meta_chars[i], chs, (size_t)n) == 0) {
            return 1;
        }
    }

    return 0;
}

int getword(char *w)
{
    int i;
    char ch;
    int count = 0;
    int meta_count = 0;
    char meta_chars[STORAGE];
    /* quoting flag */
    int is_quoting = 0;
    /* escaping falg */
    int is_escaping = 0;
    /* meta combining flag */
    int is_meta_combining = 0;

    for (;;) {
        if ((ch = (char)getchar()) == EOF) {
            if (count == 0) {
                w[0] = 0;
                return -1;
            }

            break;
        }

        if (count >= STORAGE - 1) {
            /* maximum */
            ungetc(ch, stdin);
            break;
        }

        /* printf("got %x\n", ch); */

        if (ch == '\n' || ch == ';') {
            if (count > 0) {
                ungetc(ch, stdin);
            }

            if (meta_count > 0) {
                ungetc(ch, stdin);
                for (i=0; i<meta_count; ++i) {
                    w[count] = meta_chars[i];
                    count = count + 1;
                }
            }

            break;
        }


        if (count == 0 && meta_count == 0) {
            if (ch == ' ' || ch == '\t') {
                /* skip leading spaces */
                if (is_quoting) {
                    w[count] = ch;
                    count = count + 1;
                }
                continue;
            }

            if (ch == '\'') {
                is_quoting = 1;
                continue;
            }

            if (ch == '\\' && !is_escaping) {
                //TODO: is_escaping = 1;
                continue;
            }

            // printf("got %c\n", ch);
            if (is_meta_chars(&ch, 1)) {
                if (is_quoting) {
                    w[count] = ch;
                    count = count + 1;
                    continue;
                }
                // printf("%c is meta char\n", ch);
                /* meta char, set flag */
                meta_chars[meta_count] = ch;
                meta_count = meta_count + 1;
                is_meta_combining = 1;
                continue;
            }

            w[count] = ch;
            count = count + 1;
            continue;
        }

        if (ch == ' ' || ch == '\t') {
            if (is_quoting || is_escaping) {
                /* part of word  */
                w[count] = ch;
                count = count + 1;

                if (is_escaping) {
                    is_escaping = 0;
                }
                continue;
            }

            if (meta_count > 0) {
                for (i=0; i<meta_count; ++i) {
                    w[count] = meta_chars[i];
                    count = count + 1;
                }
            }

            /* end of word */
            break;
        }

        if (ch == '\'') {
            if (is_escaping) {
                /* 'hello\'world' -> hello'world */
                w[count-1] = ch;
                is_escaping = 0;
                continue;
            }

            if (is_quoting) {
                is_quoting = 0;
                continue;
                // break;
            }

            /* set quoting flag */
            is_quoting = 1;
            continue;
        }

        if (ch == '\\') {
            if (is_escaping) {
                w[count] = ch;
                count = count + 1;
                is_escaping = 0;
                continue;
            }

            if (is_quoting) {
                w[count] = ch;
                count = count + 1;
                is_escaping = 1;
                continue;
            }

            /* set escaping flag */
            is_escaping = 1;
            continue;
        }

        if (is_meta_combining) {
            meta_chars[meta_count] = ch;
            meta_count = meta_count + 1;
            if (is_meta_chars(meta_chars, meta_count)) {
                /* combination */
                continue;
            }

            ungetc(ch, stdin);
            meta_count = meta_count - 1;
            for (i=0; i<meta_count; ++i) {
                w[count] = meta_chars[i];
                count = count + 1;
            }

            break;
        }

        if (is_meta_chars(&ch, 1)) {
            if (is_escaping) {
                w[count] = ch;
                count = count + 1;
                is_escaping = 0;
                continue;
            }

            if (is_quoting) {
                w[count] = ch;
                count = count + 1;
                continue;
            }

            /* first meta char */
            ungetc(ch, stdin);
            break;
        }

        /* part of word */
        w[count] = ch;
        count = count + 1;
        if (is_escaping) {
            is_escaping = 0;
        }
        continue;
    }
    
    w[count] = 0;

    if (is_quoting) {
        return -2;
    }

    return count;
}
