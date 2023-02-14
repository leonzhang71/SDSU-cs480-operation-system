/* see
 * http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/apis/dup2.htm
 * for a discussion of dup2() and this sample program
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

void print_file_id(int file_descriptor) {
  struct stat info;
  if (fstat(file_descriptor, &info) != 0)
    fprintf(stderr, "stat() error for file_descriptor %d: %s\n",
            strerror(errno));
  else
    printf("The file id of file_descriptor %d is %d\n", file_descriptor,
           (int) info.st_ino);
}

main() {
  int file_descriptor, file_descriptor2;
  char fn[] = "original.file";
  char fn2[] = "dup2.file";

  /* create original file */
  if((file_descriptor = creat(fn, S_IRUSR | S_IWUSR)) < 0)
    perror("creat() error");
  /* create file to dup to */
  else if((file_descriptor2 = creat(fn2, S_IWUSR)) < 0)
    perror("creat()error");
  /* dup file_descriptor to file_descriptor2; print results */
  else {
    print_file_id(file_descriptor);
    print_file_id(file_descriptor2);
    if ((file_descriptor2 = dup2(file_descriptor, file_descriptor2)) < 0)
      perror("dup2() error");
    else {
      puts("After dup2()...");
      print_file_id(file_descriptor);
      print_file_id(file_descriptor2);
      puts("The file descriptors are different but they");
      puts("point to the same file, which is different than");
      puts("the file that the second file_descriptor originally pointed to.");
      close(file_descriptor);
      close(file_descriptor2);
    }
    unlink(fn);
    unlink(fn2);
  }
}
