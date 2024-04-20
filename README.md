# TARDIS

Trace And Rewrite Delays In Syscalls: Hooking time-related Linux syscalls to warp a process's perspective of time.

This code is rather buggy, mainly due to my lack of understanding of the ptrace API.
You probably shouldn't use it for anything serious, although it could be useful for
testing/debugging certain applications.

## Things to try:

```
$ ./tardis 10000 10000 xclock
$ ./tardis 1 3 glxgears
$ ./tardis 1 -1 glxgears
$ ./tardis 10 10 firefox
$ ./tardis 10 10 /bin/sh
```

![xclock demo](https://i.imgur.com/UnFYuLs.gif)

## Notes:

- Currently only x86_64 Linux is supported. It should be possible to port to i386 with fairly minimal effort.

- In order to prevent vDSO functions from being used instead of syscalls, the auxiliary vector is patched to ignore vDSO pointer entry.

- Certain simple programs, like `glxgears`, don't mind being run with time flowing in reverse! Most programs don't however, and of course there's no way to have a negative delay.

- There are many more syscalls that I still need to handle.

Currently handled syscalls:

- `nanosleep`
- `clock_nanosleep`
- `select`
- `poll`
- `gettimeofday`
- `clock_gettime`
- `time`
