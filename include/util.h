#ifndef LIBBREAK_UTIL_H
#define LIBBREAK_UTIL_H
#include <stdio.h> /* fprintf */

// TODO: generate this MACRO by configure
#define LIBBREAK_CONFIG_VERBOSE 1

#ifdef LIBBREAK_CONFIG_VERBOSE
#    define LIBBREAK_VERBOSE_OUTPUT(fmt, ...) \
        fprintf(stderr, "[verbose] " fmt, __VA_ARGS__)
#else
#    define LIBBREAK_VERBOSE_OUTPUT(fmt, ...)
#endif
#endif
