/* An example of dup2(), useful in programming a shell.
 * This code illustrates how to do some things correctly... and how to do
 * many things INcorrectly
 */

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#define MAXNAMELENGTH 50

main() {
  int output_fd;

  int flags;
  flags = O_APPEND | O_WRONLY;
/* The above flags are NOT the right combination for output redirection.  Try:
 * man 2 open
 * to discover what the constants mean, and what options are available.
 * On some systems, the actual bitmasks are in /usr/include/sys/fcntl.h,
 * which is then included by /usr/include/fcntl.h
 */

  char filename[MAXNAMELENGTH] = "testfile";
/* Using malloc() to create a string of the correct length would be infinitely
 * preferable to limiting filenames to a fixed length (50 in this example)...
 * and of course, a shell is unlikely to hardwire some filename into the code
 */

  if ((output_fd=open(filename, flags, S_IRUSR | S_IWUSR)) < 0) {
	perror("oops, open failed!");
	exit(1);
  }

/* With the given flags, open() will fail if the file does not already exist.
 * That is NOT the behavior your shell should have!
 */

/* /usr/include/unistd.h contains:
 * #define   STDOUT_FILENO   1
 * which specifies the file descriptor for stdout.  For readability, good
 * programming practice requires the use of this mnemonic rather than just a 1
 */

  dup2(output_fd,STDOUT_FILENO);
/* Not checking the return status of dup2() indicates poor programming habits */

/* At this point, a file has been opened for output, and this file is
 * pointed to by the original file descriptor and now also by the file
 * descriptor that specifies stdout.  Therefore, if the child writes to
 * stdout, the output will automatically go into the file that this
 * file descriptor points to.
 *
 * We really don't want (or need) to have both file descriptors open, so:
 */

  close(output_fd);
/* Hmm, this naughty programmer is ignoring whether close() failed or succeeded */

/* Which process (parent or child) should have the output redirected?
 * Should the setup for redirection happen before or after the exec()?
 */

}
