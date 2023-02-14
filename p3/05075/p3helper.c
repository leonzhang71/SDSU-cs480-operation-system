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

sem_t *pmutx;
int self_placed;
int* pnewline;
char semaphoreMutx[SEMNAMESIZE];
int fd_count = -1;

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
    sprintf(semaphoreMutx,"/%s%ldmutx1",COURSEID,(long)getuid());
    // try to create semaphore
    pmutx = sem_open(semaphoreMutx, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
    D(printf("Total is %d, my quota is %d\n", nrRobots * quota, quota);)
    D(printf("Trying to open mutex\n");)
    if (pmutx) {
        count = 0;
        // the first process to run is also responsible to
        // create the count file and initialize it
        CHK((fd_count = open("countfile" ,O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR)));
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
        CHK((fd_count = open("countfile", O_RDWR, S_IRUSR|S_IWUSR)));
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
        CHK(unlink("countfile"));
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
    return -1;
}
