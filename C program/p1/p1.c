/* ~cs480/One/getword.h provides a good example of the level of documentation
 * appropriate for this course. This p1.c provides an outstandingly BAD example.
 * ~cs480/One/makefile also provides a LOT of critically useful information!
 */

#include "getword.h"

int main()

{
int c;
char s[STORAGE];

for(;;) {
	(void) printf("n=%d, s=[%s]\n", c = getword(s), s);
	if (c == -1) break;
	}
}
