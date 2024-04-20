all: tardis novdso.so

tests: tests/*
	(cd tests; find . -name '*.c' | xargs -I@ echo @ | rev | cut -d '.' -f 2- | rev | xargs -I@ gcc -std=c99 -Wall -o ./bin/@ @.c)

.PHONY: test
test: tests tardis
	python3 tests/run_tests.py

tardis: tardis.c
	gcc tardis.c -o tardis -lm -Wall -Ofast -std=c99 -DPID_MAX=$(shell cat /proc/sys/kernel/pid_max)

novdso.so: novdso.c
	gcc -std=c99 -Wall -fPIC -shared -o novdso.so novdso.c

clean:
	rm -f tests/bin/*
	rm -f tardis novdso.so
