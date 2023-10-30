#include <sys/sysctl.h>

int main() {
  int mib[] = { CTL_HW, HW_PHYSMEM };
  size_t memsize;
  size_t len = sizeof(memsize);
  sysctl(mib, 2, &memsize, &len, NULL, 0);
  printf("The total physical memory is %zu bytes\n", memsize);
  return 0;
}
