# Libbreak

A Linux library for inserting/removing self software breakpoints at runtime.

## Usage

### Compilation

```shell
./configure
make
# make test # only build test
# make clean # clean every built files
# make clean-local # clean only local built files, not clean submodules' built files
```

### Use Libbreak in your C code

* Include the `include/break.h` in your C code,
* Libbreak linkage arguments: `-Lpath/to/libbreak -lbreak -Lpath/to/libbreak/capstone -lcapstone`

### API docs

`include/break.h`

### Examples

`test/break.c`

## Motivation

When smartly dealing with the Linux SIGSEGV signal, the programmer hopes to check whether the SIGSEGV is an actual error. If not, the program needs to change the behavior of memory access instruction, which triggers the SIGSEGV signal, in 2 ways:

1. interpret the SIGSEGV-triggered instruction, like how Linux kernel solving unaligned memory access;
2. temporarily change the memory protection;

The 1st solution seems cool and fast (only produce one signal), but its interpretation is lack of generality. Here generality means programmers firstly need to disassemble the SIGSEGV-triggered instruction and then to decide how to interpret it, which heavily depends on the goal of the program.

The 2nd solution has great generality, but for memory security, it is not easy to restore memory protection. The Libbreak project tries to alleviate this problem by presenting the capability to insert/remove soft breakpoints into the program at runtime. Here is a Libbreak workflow example of the improved 2nd solution,

1. memory access triggers a SIGSEGV signal;
2. in SIGSEGV handler,
   1. change the memory protection,
   2. insert a break next to the SIGSEGV-triggered instruction;
3. rerun the SIGSEGV-triggered instruction;
4. run the next instruction, which is a break instruction;
5. in break handler,
   1. restore the memory protection,
   2. remove the break at the current instruction, which is the next instruction of SIGSEGV-triggered instruction;
6. continue execute the following instructions

The 2nd solution with Libbreak needs to produce two signals, therefore runs a little bit slower than the 1st solution; while the 2nd solution with Libbreak has greater generality.

## TODO

* add x86 support