#include "common.h"
#include "matmult.h"

static void fill(T *dst, int size) {
  for (int i = 0; i < size; i++)
      dst[i] = 1;
}

static matmult_t matmult[4] = {matmult0, matmult1, matmult2, matmult3};

int main(int argc, char **argv) {
  int opt, variant = -1, n = 0;
  pmc_evset_t evset = PMC_NONE;
  bool err = false;

  while ((opt = getopt(argc, argv, "p:n:v:")) != -1) {
    if (opt == 'p') {
      if (strcmp(optarg, "ipc") == 0)
        evset = PMC_IPC;
      else if (strcmp(optarg, "branch") == 0)
        evset = PMC_BRANCH;
      else if (strcmp(optarg, "memory") == 0)
        evset = PMC_MEMORY;
    } else if (opt == 'n')
      n = atoi(optarg);
    else if (opt == 'v')
      variant = atoi(optarg);
    else
      err = true;
  }

  if (err || n == 0 || variant < 0 || variant >= 4)
    die("Usage: %s -p [ipc|branch|memory] -n size -v variant\n", argv[0]);

  pmc_init(evset);

  if (n % BLOCK)
    die("Matrix size (%d) must be divisible by %d!\n", n, BLOCK);

  size_t size = n * n * sizeof(T);
  size_t pagesize = getpagesize();

  /* Allocate space for each matrix plus extra space (of page size). */
  T *a = malloc_page_aligned(size + pagesize);
  T *b = malloc_page_aligned(size + pagesize);
  T *c = malloc_page_aligned(size + pagesize);

  printf("Generate 2 matrices %d x %d (%ld KiB each)\n", n, n, size >> 10);

  fill(a, n * n + NITEMS(pagesize, T));
  fill(b, n * n + NITEMS(pagesize, T));
  bzero(c, size + pagesize);
  flush_cache();

  printf("Performing matrix multiplication.\n");

  _timer_t timer;
  timer_reset(&timer);
  timer_start(&timer);
  pmc_start();
  matmult[variant](n, a + A_OFFSET, b + B_OFFSET, c + C_OFFSET);
  pmc_stop();
  timer_stop(&timer);
  timer_print(&timer);

  pmc_print();

  free(a);
  free(b);
  free(c);

  pmc_kill();

  return 0;
}
