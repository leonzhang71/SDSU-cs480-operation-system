//Name: Chonglin ZHang
//RedID: 822942790
//Class account: cssc0094
//Course: CS480 Operating Systems
//Instructor: John Carroll
//Due date: 10-6-2021
/*
 purpose:
 This program is a command line interperter for Unix system, and it acts like shell.
 It should handle unix redirection with metacharacters like ">","<",";","'",and "&".
 Also, the program still can handle the build in commands like cd, echo, ls, sleep,
 and show the path for the file. If the command is an executable then an execution
 of the command is made inside the child of a fork. Furthermore, the program use
 the signal handling like 'echo hi > filename.'
 
 Program logic:
 void reset(): reset everything to null or 0, so the program will reset everything
 to 0 or null. If we don't reset it, the program will save everything from the input
 until max item 100.
 void showprompt(): print the prompt
 int parse(): this part is going to find the metacharacter. Therefore, I use the
 strcmp to compare the metacharacter (first character) of each string. Other normal
 strings, I use strdup function which I get it from internet. The function will
 return a pointer to a new string. Memory for the new string can be freed with
 free(). Also, if the item is larger than 100, and should say too many arg.
 int run(): this section is going to check the input and output exist or not, and
 do the cd and ls-f command.
 int run_test(): testing and checking the run()
 int run_single_cmd(): similar idea for dup2.c. First, we have to open the file to
 see that it is exist or not. Also, we have to use dup2 to a file which open for
 the output. We don't want to have both file descriptors open, so close()
 int run_piped_cmds(): we are doing the vertical pipe. Therefore, we have to use
 'two' fork() for two pid which mean parent will create two children. Using wait
 system call in c. A call to wait() blocks the calling process until one of its
 child processes exits or a signal is recevied. For vertical pipe, we have to do
 child to child, and then do what run_single_cmd do, and child to do something
 similar.
 int run_cd(): if there are too many argument which over item 100, print too many
 argument. If there are only 'cd' with no parameters should be interpreted as cd
 'cd $HOME'. Also, for pathname, I use the getenv() with handling the path, and
 set '$' in zero position.
 int run_ls_F(): p2 handle 'ls-F' as a built-in. I read the man page about errno.
 There are three conditions that I will use for the ls-F. EACCES: permission denied.
 ENOENT: no such file or directory. ENOTDIR: not a direcory. When argument is 'ls-f'
 , the extern int variable errno will be charge. I set up statement to find which
 condition for errno.
 */



#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include "getword.h"
#include <stdbool.h>
#include "p2.h"
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include "CHK.h"

/*make splint happy*/
#ifndef EACCES
#define EACCES
#endif

#ifndef ENOENT
#define ENOENT
#endif

#ifndef ENOTDIR
#define ENOTDIR
#endif

// complete
int complete = 0;
// eof flag
int eof_flag = 0;
// bad syntax flag
int bad_syntax = 0;
// pipe flag (|)
int piping_flag = 0;
// meta char flag (> < | &)
int meta_char_flag = 0;
// (&)
int background_flag = 0;
// (<)
int redirecting_input = 0;
// (>)
int redirecting_output = 0;
// first command
char* first_word = NULL;
// arguments of first command
char* first_args[MAXITEM];
// num of first arguments
int first_args_num = 0;
// second command
char* second_word = NULL;
// arguments of second command
char* second_args[MAXITEM];
// num of second arguments
int second_args_num = 0;
// redirected input file
char* input_file = NULL;
// redirected output file
char* output_file = NULL;
extern int errno;



// reset global variables
void reset();
// print prompt
void showprompt();
// parse commands
int parse();
// run commands
int run();
// run test
void run_test();
// check file
int check_file(char* file);

int run_cd();
int run_ls_F();
int run_single_cmd();
int run_piped_cmds();

int killpg(int pgrp, int sig);



/*unidentifier: strdup in string.h,and copy the strdup from the stackoverflow */
char* strdup (const char* s)
{
  size_t slen = strlen(s);
  char* result = malloc(slen + 1);
  if(result == NULL)
  {
    return NULL;
  }

  memcpy(result, s, slen+1);
  return result;
}

void myhandler(int signum)
{
    // printf("Received SIGTERM (%d), and the special handler is running...\n", signum);
    complete = 1;
}


int main(int argc, char* argv[])
{
    if (setpgid(0, 0) < 0) {
        perror("setpgid");
        return -1;
    }
   (void)signal(SIGTERM,myhandler);
   // signal(SIGTERM, myhandler);

    for (; !complete ;) {
        if (eof_flag) {
            // quit
            break;
        }

        // reset global variables
        reset();

        showprompt();

        parse();

        if (bad_syntax) {
            // printf("bad syntax\n");
            fprintf(stderr, "bad syntax\n");
            continue;
        }

        if (first_word == NULL) {
            continue;
        }

        // run commands
        run();

        // run_test();
    }

    killpg(getpgrp(), SIGTERM);
    printf("p2 terminated.\n");

    exit(0);
}

// reset global variables
void reset()
{
/* Declaration of Locals */
    int i;
    
    eof_flag = 0;
    bad_syntax = 0;
    piping_flag = 0;
    meta_char_flag = 0;
    background_flag = 0;
    redirecting_input = 0;
    redirecting_output = 0;
    first_args_num = 0;
    second_args_num = 0;

    if (first_word) {
        free(first_word);
        first_word = NULL;
    }
    for (i=0; i<MAXITEM; ++i) {
        if (first_args[i]) {
            free(first_args[i]);
            first_args[i] = NULL;
        }
    }

    if (second_word) {
        free(second_word);
        second_word = NULL;
    }
    for (i=0; i<MAXITEM; ++i) {
        if (second_args[i]) {
            free(second_args[i]);
            second_args[i] = NULL;
        }
    }

    if (input_file) {
        free(input_file);
        input_file = NULL;
    }

    if (output_file) {
        free(output_file);
        output_file = NULL;
    }
}


void showprompt()
{
    /* issue prompt */
    printf("%s ", PROMPT_STRING);
    /* flush stdout */
    fflush(stdout);
}


int parse()
{
/* Declaration of Locals */
    int c;
    char w[STORAGE];
    for (;;) {

        char** args;
        int* args_num;

        c = getword(w);
        if (c == -1) {
            /* EOF */
            eof_flag = 1;
            break;
        }

        if (c == 0) {
            /* newline or ; */
            // printf("got newline or ;\n");
            break;
        }

        // printf("got %s, pid:%d\n", w, getpid());

        if (strcmp(w, "<") == 0) {
            if (meta_char_flag) {
                bad_syntax = 1;
            }

            meta_char_flag = 1;
            redirecting_input = 1;
            continue;
        } else if (strcmp(w, ">") == 0) {
            if (meta_char_flag) {
                bad_syntax = 1;
            }

            meta_char_flag = 1;
            redirecting_output = 1;
            continue;
        } else if (strcmp(w, "|") == 0) {
            if (meta_char_flag) {
                bad_syntax = 1;
            }

            meta_char_flag = 1;
            piping_flag = 1;
            continue;
        } else if (strcmp(w, "&") == 0) {
            if (meta_char_flag) {
                bad_syntax = 1;
            }

            background_flag = 1;
            break;
        }


        /* normal words */
        if (redirecting_input) {
            meta_char_flag = 0;
            redirecting_input = 0;

            // another one
            if (input_file) {
                free(input_file);
            }
            input_file = strdup(w);
            continue;
        }

        if (redirecting_output) {
            meta_char_flag = 0;
            redirecting_output = 0;

            // another one
            if (output_file) {
                free(output_file);
            }

            output_file = strdup(w);
            continue;
        }

        if (piping_flag) {
            if (second_word) {
                // only one
                bad_syntax = 1;
                break;
            }

            meta_char_flag = 0;
            piping_flag = 0;

            second_word = strdup(w);
            second_args[0] = strdup(w);
            second_args_num = 1;
            continue;
        }

        if (first_word == NULL) {
            // set first word
            first_word = strdup(w);
            first_args[0] = strdup(w);
            first_args_num = 1;
            continue;
        }

        /* args */
     /*   char** args;
        int* args_num;
     */ 
        if (second_word == NULL) {
            // args of first command
            args = (char**)first_args;
            args_num = &first_args_num;
        } else {
            // args of second command
            args = (char**)second_args;
            args_num = &second_args_num;
        }

        if (*args_num >= MAXITEM) {
            //  too many arguments
            fprintf(stderr, "too many arguments\n");
            bad_syntax = 1;
            break;
        }

        if (args[*args_num]) {
            free(args[*args_num]);
        }

        args[*args_num] = strdup(w);
        *args_num += 1;
    }
}

int run()
{
    // check redirected files
    if (input_file && (access(input_file, F_OK) != 0)) {
        // input file not exist
        fprintf(stderr, "input file doesnot exist\n");
        return 1;
    }

    if (output_file && (access(output_file, F_OK) == 0)) {
        // output file exists
        fprintf(stderr, "output file exists\n");
        return 1;
    }

    if (strcmp(first_word, "cd") == 0) {
        return run_cd();
    }

    if (strcmp(first_word, "ls-F") == 0) {
        return run_ls_F();
    }

    if (second_word == NULL) {
        return run_single_cmd();
    }

    return run_piped_cmds();
}

void run_test()
{
/* Declaration of Local*/
    int i;
    if (first_word) {
        printf("command: %s\n", first_word);
        if (first_args_num > 0) {
            printf("arguments:\n");
            for (i=0; i<first_args_num; ++i) {
                printf("    %s\n", first_args[i]);
            }
        }
    }

    if (second_word) {
        printf("piped command: %s\n", second_word);
        if (first_args_num > 0) {
            printf("arguments:\n");
            for (i=0; i<second_args_num; ++i) {
                printf("    %s\n", second_args[i]);
            }
        }
    }

    if (input_file) {
        printf("redirected input file: %s\n", input_file);
    }

    if (output_file) {
        printf("redirected output file: %s\n", output_file);
    }

    printf("run in background?: %s\n", (background_flag ? "true" : "false"));
}

int run_cd()
{
/* Declaration of Local*/
    char* path;
    if (background_flag) {
        background_flag = 0;
    }

    if (first_args_num > 2) {
        // error
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }

    if (first_args_num == 1) {
        // cd
        first_args_num += 1;
        first_args[1] = strdup("$HOME");
    }

    path = first_args[1];
    if (path[0] == '$') {
        // environment, skip $
        path = getenv(path + 1);
        if (path == NULL) {
            // error
            fprintf(stderr, "invalid environment variable\n");
            return 1;
        }
    }

    if (chdir(path)) {
        perror("chdir");
        return 1;
    }

    // printf("chdir success\n");
    return 0;
}


int run_ls_F()
{
/* Declaration of Locals*/    
    DIR *dirp;
    struct dirent *dp;

    if (background_flag) {
        background_flag = 0;
    }

    if (first_args_num == 1) {
        // ls-F
        first_args_num += 1;
        first_args[1] = strdup(".");
    }

    dirp = opendir(first_args[1]);
    if (dirp == NULL) {
       if (errno == EACCES) {
            fprintf(stderr, "%s unreadable\n", first_args[1]);
            return -1;
        }

        if (errno == ENOENT) {
            fprintf(stderr, "%s doesnot exist\n", first_args[1]);
            return -1;
        }

        if (errno == ENOTDIR) {
            // regular file
            printf("%s\n", first_args[1]);
        } else {
            perror("ls-F error");
        }


        // struct stat s;
        // if (stat(first_args[1], &s) < 0) {
        //     perror("ls-F stat");
        //     perror("opendir");
        //     return -1;
        // }


        return 0;
    }

    while(dirp) {
        if ((dp = readdir(dirp)) != NULL) {
            printf("%s\n", dp->d_name);
        } else {
            closedir(dirp);
            break;
        }
    }

    return 0;
}

int run_single_cmd()
{
/* Declaration of Local*/
    int pid;

    if ((pid = fork()) < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        // child
        int in_fd;
        int out_fd;
        if (input_file) {
            if ((in_fd = open(input_file, O_RDONLY)) < 0) {
                perror("open");
                _exit(-1);
            }

            if (dup2(in_fd, STDIN_FILENO) < 0) {
                close(in_fd);
                perror("dup2");
                _exit(-1);
            }

            close(in_fd);
        }

        if (output_file) {
            if ((out_fd = open(output_file, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) < 0) {
                perror("open");
                _exit(-1);
            }

            if (dup2(out_fd, STDOUT_FILENO) < 0) {
                perror("dup2");
                _exit(-1);
            }

            close(out_fd);
        }

        if (execvp(first_word, first_args) < 0) {
            perror("execvp failure");
            _exit(-1);
        }
    }

    // parent
    // printf("child: %d created\n", pid);
    if (background_flag) {
        // run in backgound
        printf("%s [%d]\n", first_word, pid);
        return 0;
    }

    // wait child
    for (;;) {
        int tpid = wait(NULL);
        if (tpid == pid || tpid < 0) {
            if (tpid < 0) {
                perror("wait");
            }
            break;
        }
    }

    // printf("wait child %d success\n", pid);

    return 0;
}


int run_piped_cmds()
{
/* Declaration of Locals*/
    int pid;
    int pid2;

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid > 0) {
        // parent
        for (;;) {
            int tpid = wait(NULL);
            if (tpid == pid || tpid < 0) {
                if (tpid < 0) {
                    perror("wait");
                }
                break;
            }
        }

        return 0;
    }

    // child
    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("pipe");
        _exit(-1);
    }

    pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        _exit(-1);
    }

    if (pid2 == 0) {
        // child-child
        int in_fd;
        // execute first command
        if (input_file) {
            if ((in_fd = open(input_file, O_RDONLY)) < 0) {
                perror("open");
                _exit(-1);
            }

            if (dup2(in_fd, STDIN_FILENO)) {
                perror("dup2");
                close(in_fd);
                _exit(-1);
            }

            close(in_fd);
        }

        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) < 0) {
            perror("dup2");
            _exit(-1);
        }


        close(pipefd[1]);
        if (execvp(first_word, first_args) < 0) {
            perror("execvp");
            _exit(-1);
        }
    }

    close(pipefd[1]);
    if (dup2(pipefd[0], STDIN_FILENO) < 0) {
        perror("dup2");
        _exit(-1);
    }

    int out_fd;
    if (output_file) {
        if ((out_fd = open(output_file, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) < 0) {
            perror("open");
            _exit(-1);
        }

        if (dup2(out_fd, STDOUT_FILENO) < 0) {
            perror("dup2");
            _exit(-1);
        }

        close(out_fd);
    }

    close(pipefd[0]);
    if (execvp(second_word, second_args) < 0) {
        perror("execvp");
        _exit(-1);
    }

    return -1;
}
