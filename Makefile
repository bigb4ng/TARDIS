all: tardis novdso.so

tardis: tardis.c
	gcc tardis.c ptrace.c -o tardis -lm -Wall -Ofast -std=c99 -DPID_MAX=$(shell cat /proc/sys/kernel/pid_max)

clean:
	rm -f tardis novdso.so
