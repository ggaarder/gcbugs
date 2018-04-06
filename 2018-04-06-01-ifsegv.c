#if 0

Original Project: nnmnist

Original GDB Output:

Program received signal SIGSEGV, Segmentation fault.
Cannot remove breakpoints because program is no longer writable.
Further execution is probably impossible.
main () at nnmnist.c:144
144       if (ntwkfd)

What is much more strange is that sometimes there is a SIGSEGV and sometimes not:

$ tcc 2018-04-06-01-ifsegv.c -lm
$ ./a.out 
$ ./a.out 
Segmentation fault
$ ./a.out 
$ ./a.out 
Segmentation fault
$ ./a.out 
Segmentation fault
$ ./a.out 
Segmentation fault
$ ./a.out 
$ ./a.out 
$ ./a.out 
$ ./a.out 
Segmentation fault
$ ./a.out 
Segmentation fault
$ ./a.out 
$ ./a.out 
Segmentation fault
$ ./a.out 
$ ./a.out 
$ ./a.out 
Segmentation fault
$ ./a.out 
$ ./a.out 
Segmentation fault
$ ./a.out 
$ ./a.out 
$ ./a.out 
Segmentation fault
$ ./a.out 
$ ./a.out 
$ ./a.out 
$ ./a.out 
$ ./a.out 
$ ./a.out 
$ ./a.out 
$ ./a.out 
$ ./a.out 
$ ./a.out 
Segmentation fault
$ ./a.out 

Finally I found that what caused SIGSEGV is not that "if" ..........

Program received signal SIGSEGV, Segmentation fault.
0x000000000040096d in main () at 2018-04-06-01-ifsegv.c:121
121	  munmap(lbl, lbllen);
(gdb) p lbl
$1 = 0x0
(gdb) p lbllen
$2 = 4196736

WTF?????????? So why did GDB say that it is the `if`?
#endif

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NTWKFN "foo"

char *rawntwk = 0;
int L;
char **layers = 0; // for convenience

#define LBL "train-labels.idx1-ubyte"
#define IMG "train-images.idx3-ubyte"
char *lbl = 0, *img = 0;
int imgsiz;

int main() {
  int i, wcnt, ncnt, l, n;
  char *p;
  float v;
  int imgfd = 0, lblfd = 0, ntwkfd = 0,
    imglen, lbllen, xcnt, ntwklen, imgno;

  
  remove(NTWKFN);
    ntwkfd = open(NTWKFN, O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR);

    L = 4+2;
    write(ntwkfd, &L, sizeof(int));
    ncnt = imgsiz;
    write(ntwkfd, &ncnt, sizeof(int)); // input layer
    srand(time(0));
    
    for (l = 1; l < L-1; ++l) { // hidden layers
      wcnt = ncnt;
      ncnt = 20;
      write(ntwkfd, &ncnt, sizeof(int));
      for (n = 0; n < ncnt; ++n)
        for (i = 0; i <= wcnt; ++i) { // together with bias
          v = rand()/RAND_MAX;
          write(ntwkfd, &v, sizeof(float));
        }
    }

    // output layer
    wcnt = ncnt;
    ncnt = 10; // recognizing 10 digits
    write(ntwkfd, &ncnt, sizeof(int));
    for (n = 0; n < ncnt; ++n)
      for (i = 0; i <= wcnt; ++i) { // together with bias
        v = rand()/RAND_MAX;
        write(ntwkfd, &v, sizeof(float));
      }    
        
  if (ntwkfd)
    close(ntwkfd);
  munmap(lbl, lbllen);
}
