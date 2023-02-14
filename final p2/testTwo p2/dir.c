/*
 * extract filenames and index-node numbers from a directory
 *
 * This program produces results similar to 'ls -i', but 'ls -i' puts the
 * inode numbers in front of the names, sorts the names alphabetically, and
 * [if the results are going to a terminal] arranges them in multiple columns.
 * 'man dirent.h' explains how to access the four fields of the directory entry.
 * Adapted from http://www.linuxmisc.com/9-unix-questions/ca06d90f2028b36e.htm
 */
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
     DIR *dirp;    
     struct dirent *dp;
     int i = 0;

     dirp = opendir(".");
/* hmm, this poorly-written program plows ahead even if the system call fails,
 * to die a horrible death.  Failure could occur if the directory were
 * unreadable, or if we tried to open something that was neither a directory
 * nor a [soft] link to a directory.  The man page outlines what return value
 * should have been checked for. */

     while (dirp) {
         if ((dp = readdir(dirp)) != NULL) {
/* readdir() is a system call that reads one directory entry at a time.
 * You give readdir() the current position within an opened directory, and
 * it returns to you a pointer to a structure it has filled with the values
 * of the four fields for this entry.  We save this pointer (in dp) so that
 * we can examine that entry.  */
             printf("Entry%3d: %s %d\n", ++i, dp->d_name, dp->d_ino);
/* /usr/include/dirent.h #include's /usr/include/sys/dirent.h, which
 * defines the various fields in a dirent (directory entry) structure.
 * not surprisingly, d_name refers to the [human-readable] name of the entry. */
         }
         else {
/* each directory entry points to the next entry; the last one returns NULL,
 * at which point we know we have seen all the entries.  The entries are NOT
 * stored alphabetically; new entries are appended to the end of the list
 * (so they are [mostly] TIME-ordered, unless an entry has been deleted, and
 * there is 'room' to insert a new name where the deleted entry used to be). */
             closedir(dirp);
             break;
         }
     }
     return (0);
}
