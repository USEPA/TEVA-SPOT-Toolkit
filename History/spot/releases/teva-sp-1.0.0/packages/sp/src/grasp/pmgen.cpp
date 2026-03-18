#include <stdio.h>
#include <stdlib.h>
#include "bossa_random.h"


void showUsage(char **argv) {
  fprintf (stderr, "Usage: %s <users> <facilities> <seed>\n", argv[0]);
  exit (-1);
}


int main (int argc, char *argv[]) {
  if (argc!=4) showUsage(argv);
  int n = atoi (argv[1]);
  int m = atoi (argv[2]);
  int s = atoi (argv[3]);

  BossaRandom::randomize(s);
  fprintf (stdout, "p %d %d\n", n, m);
  fprintf (stdout, "s %d\n", s);
  for (int i=1; i<=n; i++) {
    for (int j=1; j<=n; j++) {
      fprintf (stdout, "a %d %d %d\n", i, j, BossaRandom::getInteger(1, n));
    }
  }
  return 0;
}
