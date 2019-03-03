/*
 * A slowed down cat for ascii animation.  Also useful for rate-limiting
 * a transfer over a narrow channel, e.g., sending data over a dialup
 * link so as to not overly interfere with interactive use.
 *
 * bsy@cs.cmu.edu
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void  usage() {
  fprintf(stderr,"Usage: sleepycat [-c count] [-s milliseconds] [files]...\n");
}

void  sleepycat(FILE *fp, int c, int s) {
  int i, ch;

  i = 0;
  s *= 1000;
  while ((ch = getc(fp)) != EOF) {
    putchar(ch);
    if (++i == c) {
      fflush(stdout);
      usleep(s);
      i = 0;
    }
  }
}

int main(int ac, char **av) {
  int   snooze, count, ch;
  extern char *optarg;
  extern int  optind, getopt();
  FILE    *fp, *fopen();

  count = 1;
  snooze = 1;
  while ((ch = getopt(ac,av,"c:s:")) != EOF) switch (ch) {
  case 'c': count = atoi(optarg); break;
  case 's': snooze = atoi(optarg);  break;
  default:  usage(); exit(1);
  }
  if (count <= 0) {
    fprintf(stderr,"sleepycat: bogus count!\n");
    count = 1;
  }
  if (optind == ac) sleepycat(stdin,count,snooze);
  else for (; optind < ac; optind++) {
    if ((fp = fopen(av[optind],"r"))) {
      sleepycat(fp,count,snooze);
      (void) fclose(fp);
    } else {
      perror("sleepycat");
      fprintf(stderr,"sleepycat: can not open %s\n",av[optind]);
    }
  }
}
