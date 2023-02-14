1://Name: Chonglin ZHang
2://RedID: 822942790
3://Class account: cssc0094
4://Course: CS480 Operating Systems
5://Instructor: John Carroll
6:
7:I wrote the p3helper.c code myself.
8:I believe everything works well for all conditions.
9:
10:how to compile:
11:make
12:p3 6 777 3
13:p3 5 777 4
14:p3 4 777 3
15:Algorithm:
16:Basically a single lock algorithm is used, every process will enter the critical region, check the current number of item has been placed, then calculate the geometry of the item that it will place.
17:If it is in line end, it print a 'N' then newline, if it is the last item, it print an 'F' then clean up.(remove common file and semaphore). After all these things done, process update the common file then
18:leave the critical region, let other process to run
19:Before starting, every process need to do some calculation, to get 'comman knowledge', (geometry), and initialize the mutex. The first process that successfully create the mutex is also responsible
20:to create the and initialize the common file, which is used to store current number of items has been placed. The mutex is initialize to value 0 not 1, because other process need to wait the common file to be
21:initialized. The first process will unlock the mutex (by sem_post) after all initialization done, then all things could actually start.
22:Every process will first try to create the semaphore, if it has been created by other process, then the process will try to open it.
23:Only the process successfully created the semaphore will try to create the common file. Only if the process has opened the sempahore and common file it will continue to print.


NOTE: Make sure your gradernotes documentation EXPLAINS your solution strategy.
If you use a common file to pass values between processes, you should explain
why your code makes it impossible for a robot to use printeger() until after
the common file has been initialized, etc.

A significant part of your grade depends on how quickly your writeup
leads me to an understanding of how your code works.
Be sure to discuss your file/semaphore cleanup strategy, too.

Make sure gradernotes also contains the other two REQUIRED statements, as well.

total 24
drwx------ 5 cssc0094   52 Aug 24 15:04 Maildir/
drwx------ 2 cssc0094  154 Sep 27 22:40 One/
-rw------- 1 cssc0094 1733 Aug 22 23:45 README
drwx------ 2 cssc0094 4096 Nov  7 17:43 Three/
drwx------ 2 cssc0094 4096 Oct  7 00:22 Two/
drwx------ 2 cssc0094 4096 Sep 25 20:27 Zero/
lrwxrwxrwx 1 cssc0094   31 Aug 27 23:55 calendar -> /home/cs/faculty/cs480/calendar
drwx------ 2 cssc0094   61 Aug 26 10:35 data/
drwx------ 2 cssc0094  129 Sep  6 00:15 data1/
drwx------ 2 cssc0094   64 Sep  6 00:34 testOne/
drwx------ 2 cssc0094 4096 Oct  5 12:42 testTwo/
-rw------- 1 cssc0094   29 Sep 29 16:52 your.outputc
Test 0... checking for the proper directory:
total 132
lrwxrwxrwx 1 cssc0094    34 Nov  3 14:46 CHK.h -> /home/cs/faculty/cs480/Three/CHK.h
-rw------- 1 cssc0094     0 Nov  3 16:12 GradeEarly
-rw------- 1 cssc0094  1595 Nov  7 17:42 gradernotes
lrwxrwxrwx 1 cssc0094    37 Nov  3 14:46 makefile -> /home/cs/faculty/cs480/Three/makefile
-rwx------ 1 cssc0094 19864 Nov  7 17:39 p3*
lrwxrwxrwx 1 cssc0094    33 Nov  3 14:46 p3.h -> /home/cs/faculty/cs480/Three/p3.h
-rw------- 1 cssc0094  7345 Nov  7 17:36 p3helper.c
-rw------- 1 cssc0094 25968 Nov  7 17:39 p3helper.o
lrwxrwxrwx 1 cssc0094    37 Nov  3 14:46 p3main.c -> /home/cs/faculty/cs480/Three/p3main.c
-rw------- 1 cssc0094 17168 Nov  7 17:39 p3main.o
-rwx------ 1 cssc0094 27136 Nov  7 17:39 p3robot*
lrwxrwxrwx 1 cssc0094    38 Nov  3 14:46 p3robot.c -> /home/cs/faculty/cs480/Three/p3robot.c
-rw------- 1 cssc0094 13688 Nov  7 17:39 p3robot.o
-rw-r--r-- 1 cssc0094  5715 Oct 18  2019 semex.c
Test 1... testing will proceed only if you have MODIFIED the proper file:
ls -lo ~/Three/; diff ~cssc0094/Three/p3helper.c ~cs480/Three/p3helper.c
total 132
lrwxrwxrwx 1 cssc0094    34 Nov  3 14:46 CHK.h -> /home/cs/faculty/cs480/Three/CHK.h
-rw------- 1 cssc0094     0 Nov  3 16:12 GradeEarly
-rw------- 1 cssc0094  1595 Nov  7 17:42 gradernotes
lrwxrwxrwx 1 cssc0094    37 Nov  3 14:46 makefile -> /home/cs/faculty/cs480/Three/makefile
-rwx------ 1 cssc0094 19864 Nov  7 17:39 p3
lrwxrwxrwx 1 cssc0094    33 Nov  3 14:46 p3.h -> /home/cs/faculty/cs480/Three/p3.h
-rw------- 1 cssc0094  7345 Nov  7 17:36 p3helper.c
-rw------- 1 cssc0094 25968 Nov  7 17:39 p3helper.o
lrwxrwxrwx 1 cssc0094    37 Nov  3 14:46 p3main.c -> /home/cs/faculty/cs480/Three/p3main.c
-rw------- 1 cssc0094 17168 Nov  7 17:39 p3main.o
-rwx------ 1 cssc0094 27136 Nov  7 17:39 p3robot
lrwxrwxrwx 1 cssc0094    38 Nov  3 14:46 p3robot.c -> /home/cs/faculty/cs480/Three/p3robot.c
-rw------- 1 cssc0094 13688 Nov  7 17:39 p3robot.o
-rw-r--r-- 1 cssc0094  5715 Oct 18  2019 semex.c
/* p3helper.c
   Program 3 CS480 SDSU Fall 2021

   This is the ONLY file you are allowed to change. (In fact, the other
   files should be symbolic links to
     ~cs480/Three/p3main.c
     ~cs480/Three/p3robot.c
     ~cs480/Three/p3.h
     ~cs480/Three/makefile
     ~cs480/Three/CHK.h    )
   */

//Name: Chonglin ZHang
////RedID: 822942790
////Class account: cssc0094
////Course: CS480 Operating Systems
////Instructor: John Carroll
//
//how to compile:
//make
//p3 6 777 3
//p3 5 777 4
//p3 4 777 3
//Algorithm:
//Basically a single lock algorithm is used, every process will enter the critical region, check the current number of item has been placed, then calculate the geometry of the item that it will place.
//If it is in line end, it print a 'N' then newline, if it is the last item, it print an 'F' then clean up.(remove common file and semaphore). After all these things done, process update the common file then
//leave the critical region, let other process to run
//Before starting, every process need to do some calculation, to get 'comman knowledge', (geometry), and initialize the mutex. The first process that successfully create the mutex is also responsible
//to create the and initialize the common file, which is used to store current number of items has been placed. The mutex is initialize to value 0 not 1, because other process need to wait the common file to be
//initialized. The first process will unlock the mutex (by sem_post) after all initialization done, then all things could actually start.
//Every process will first try to create the semaphore, if it has been created by other process, then the process will try to open it.
//Only the process successfully created the semaphore will try to create the common file. Only if the process has opened the sempahore and common file it will continue to print.
#include "p3.h"

/* You may put declarations/definitions here.
   In particular, you will probably want access to information
   about the job (for details see the assignment and the documentation
   in p3robot.c):
     */
extern int nrRobots;
extern int quota;
extern int seed;

// debug macros
#define DEBUG 0
#if(DEBUG)
    #define D(x) x
#else
    #define D(x)
#endif

#define COMMONFILENAMESIZE 128
#define INVALID_FD (-1)
#define INVALID_LINENO (-1)

sem_t *pmutx;
int self_placed;
int* pnewline;
char semaphoreMutx[SEMNAMESIZE];
char commonFilePath[COMMONFILENAMESIZE];
int fd_count = INVALID_FD;

// calculate the newline position
// aka. the shape of triangle
// return value is that the number of
// items that should newline happens
// example of 12 items
// **
// ****
// ***
// **
// *
// the first line, 2 items to newline
// the second line, 6 items to newline (item number is accumulated)
// the third line, 9 items, ...
// so the return value is an array of {2,6,9,11,12,0}
static int* do_math(int total);
// calculate current line number
// by how many item has been placed.
static int calc_lineno(int placed);

/* General documentation for the following functions is in p3.h
   Here you supply the code, and internal documentation:
   */
void initStudentStuff(void) {
    int count;
    // calculate common knowledge
    pnewline = do_math(nrRobots * quota);
    // make semaphore name
    sprintf(semaphoreMutx,"/%s%ldmutx",COURSEID,(long)getuid());
    sprintf(commonFilePath, "/tmp/%s-p3-count-%ld", COURSEID, (long)getuid());
    // try to create semaphore
    pmutx = sem_open(semaphoreMutx, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
    D(printf("Total is %d, my quota is %d\n", nrRobots * quota, quota);)
    D(printf("Trying to open mutex\n");)
    if (pmutx) {
        count = 0;
        // the first process to run is also responsible to
        // create the count file and initialize it
        CHK((fd_count = open(commonFilePath , O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)));
        CHK(lseek(fd_count, 0, SEEK_SET));
        assert(sizeof(count) == write(fd_count, &count, sizeof(count)));
        // after the countfile has been created and initialized,
        // then the other process is allowed to run
        sem_post(pmutx);
        D(printf("I am master\n");)
    } else if (errno == EEXIST) {
        // other process to run open the semaphore..
        D(printf("I am slave\n");)
        pCHK(SEM_FAILED == (pmutx = sem_open(semaphoreMutx, O_RDWR | O_EXCL, S_IRUSR | S_IWUSR)));
    } else {
        assert(0);
    }
}

/* In this braindamaged version of placeWidget, the widget builders don't
   coordinate at all, and merely print a random pattern. You should replace
   this code with something that fully follows the p3 specification. */
void placeWidget(int n) {
    // line number, items placed
    int lineno, placed;

    // should process do cleanup job
    int clean_self, clean_all;

    clean_self = 0;
    clean_all = 0;

    // enter critical region
    sem_wait(pmutx);
    // read how many placed

    if (fd_count < 0) {
        CHK((fd_count = open(commonFilePath, O_RDWR, S_IRUSR | S_IWUSR)));
    }
    CHK(lseek(fd_count, 0, SEEK_SET));
    CHK(read(fd_count,&placed, sizeof(placed)));
    // calculate line number
    lineno = calc_lineno(placed);
    // place
    printeger(n);
    D(printf("Placing %dth item\n", placed);)

    placed += 1;
    self_placed += 1;

    if (self_placed == quota) {
        clean_self = 1;
    }
    if (placed == nrRobots * quota) {
        // finish
        D(printf("Should finish now\n");)
        printf("F\n");
        clean_all = 1;
    } else if (placed == pnewline[lineno]) {
        // newline
        printf("N\n");
    }
    fflush(stdout);

    if (!clean_all) {
        // continue, update countfile
        CHK(lseek(fd_count, 0, SEEK_SET));
        assert(sizeof(placed) == write(fd_count,&placed, sizeof(placed)));
    }
    sem_post(pmutx);
    if (clean_self) {
        // clean up self
        CHK(sem_close(pmutx));
        CHK(close(fd_count));
        free(pnewline);
    }
    if (clean_all) {
        // clean all
        CHK(sem_unlink(semaphoreMutx));
        CHK(unlink(commonFilePath));
    }
}

/* If you feel the need to create any additional functions, please
   write them below here, with appropriate documentation:
   */
static int* do_math(int total) {
    int* ret;
    int i, left, nitem, lineno, nline;
    left = total;
    // firstly, calculate how many lines in total
    for (nitem = 1, lineno = 0; nitem <= left; ++nitem, ++lineno) {
        left -= nitem;
    }
    if (left) {
        lineno += 1;
    }

    // got the number of total lines
    nline = lineno;
    // allocate
    ret = malloc((lineno + 1) * sizeof(int));

    // next, fill the result
    left = total;
    nitem = 1;
    // it is easier to firstly
    // calculate number of items in each line
    // the last line has 1 item, so it is
    // easier to calculate in reverse order
    for (lineno = nline - 1; lineno >= 0; --lineno) {
        ret[lineno] = nitem;
        left -= nitem;
        nitem += 1;
        if (nitem > left) {
            nitem = left;
        }
    }

    for (i = 1; i < nline; ++i) {
        ret[i] += ret[i - 1];
    }
    ret[i] = 0;
    return ret;
}

static int calc_lineno(int placed) {
    int i;
    for (i = 0; pnewline[i] != 0; ++i) {
        if (placed < pnewline[i]) {
            return i;
       }
    }
    return INVALID_LINENO;
}
