#include <stdint.h>
#include <sys/types.h>

unsigned long get_stack_addr(pid_t pid);
unsigned long get_auxv_addr(pid_t pid);
int disable_vdso(pid_t pid);

void read_block(pid_t pid, void* src, void* dst, size_t len);
void write_block(pid_t pid, void* src, void* dst, size_t len);
