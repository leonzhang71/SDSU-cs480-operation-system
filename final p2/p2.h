#include <stdio.h>
#include "getword.h"
#include <stdio.h>
#include "getword.h"
#include <stdbool.h>
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

#define MAXITEM 100 /* max number of words per line */
#define PROMPT_STRING ":480:"
