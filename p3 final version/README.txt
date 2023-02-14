how to compile:
make
how to run:
export PATH=$PATH:`pwd`
./p3 10
# note: can be any integers from 1 to 10

Algorithm:
Basically a single lock algorithm is used, every process will enter the critical region, check the current number of item has been placed, then calculate the geometry of the item that it will place.
If it is in line end, it print a 'N' then newline, if it is the last item, it print an 'F' then clean up.(remove common file and semaphore). After all these things done, process update the common file then
leave the critical region, let other process to run
Before starting, every process need to do some calculation, to get 'comman knowledge', (geometry), and initialize the mutex. The first process that successfully create the mutex is also responsible
to create the and initialize the common file, which is used to store current number of items has been placed. The mutex is initialize to value 0 not 1, because other process need to wait the common file to be
initialized. The first process will unlock the mutex (by sem_post) after all initialization done, then all things could actually start. 
Every process will first try to create the semaphore, if it has been created by other process, then the process will try to open it. 
Only the process successfully created the semaphore will try to create the common file. Only if the process has opened the sempahore and common file it will continue to print.

If code not work, pls check the autograde file.


