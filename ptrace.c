#include <stdbool.h>
#include <stdio.h>
#include <sys/auxv.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>

void read_block(pid_t pid, void *dst, void *src, size_t len) {
	for (size_t i = 0; i < len; i += sizeof(void *)) {
		*(void **)(dst + i) = (void *)ptrace(PTRACE_PEEKDATA, pid, src + i, NULL);
#ifdef DEBUG_READ
		fprintf(stderr, "read_block: read 0x%lx at 0x%lx\n", *(long *)(dst + i), (long)(src + i));
#endif
	}
}

void write_block(pid_t pid, void *src, void *dst, size_t len) {
	for (size_t i = 0; i < len; i += sizeof(void *)) {
		ptrace(PTRACE_POKEDATA, pid, dst + i, *(void **)(src + i));
#ifdef DEBUG_WRITE
		fprintf(stderr, "write_block: write 0x%lx at 0x%lx\n", *(long *)(src + i), (long)(dst + i));
#endif
	}
}
unsigned long get_stack_addr(pid_t pid) {
	struct user_regs_struct uregs;
	ptrace(PTRACE_GETREGS, pid, 0, &uregs);
	return uregs.rsp;
}

unsigned long get_auxv_addr(pid_t pid) {
	unsigned long addr = get_stack_addr(pid);
#ifdef DEBUG
	fprintf(stderr, "get_auxv_addr: stack addr = 0x%lx\n", addr);
#endif

	long argc;
	read_block(pid, &argc, (void *)addr, sizeof(argc));
#ifdef DEBUG
	fprintf(stderr, "get_auxv_addr: argc = %ld\n", argc);
#endif

	// skip argc & argv[]
	addr += sizeof(long);
	addr += (argc + 1) * sizeof(long);

	// skip envp[]
	long data;
	read_block(pid, &data, (void *)addr, sizeof(data));
	while (data != 0) {
		read_block(pid, &data, (void *)addr, sizeof(data));
		addr += sizeof(long);
	}

	return addr;
}

int disable_vdso(pid_t pid) {
	long cur = get_auxv_addr(pid);
	if (cur == -1) {
		fprintf(stderr, "disable_vdso: error: auxv address not found\n");
		return -1;
	}

	Elf64_auxv_t auxv;
	read_block(pid, &auxv, (void *)cur, sizeof(auxv.a_type));

	while (auxv.a_type != AT_NULL) {
		if (auxv.a_type == AT_SYSINFO_EHDR) {
#ifdef DEBUG
			fprintf(stderr, "disable_vdso: found AT_SYSINFO_EHDR tag at %lx. setting type = AT_IGNORE\n", cur);
#endif

			auxv.a_type = AT_IGNORE;
			write_block(pid, &auxv, (void *)cur, sizeof(auxv.a_type));
			return 0;
		}

		cur += sizeof(auxv);
		read_block(pid, &auxv, (void *)cur, sizeof(auxv.a_type));
	}

	fprintf(stderr, "disable_vdso: error: AT_SYSINFO_EHDR tag not found\n");
	return -1;
}