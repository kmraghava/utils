
#include <assert.h>
#include "macro_args.h"
#include <stdio.h>
#include <string.h>

/* include your macro set here */

void test_macro_args (void)
{
    /* ---- NARGS ---- */
    assert(NARGS(a) == 1);
    assert(NARGS(a,b) == 2);
    assert(NARGS(a,b,c) == 3);
    assert(NARGS(a,b,c,d,e,f,g,h,i,j) == 10);

    /* ---- FIRST_ARG ---- */
    assert(strcmp(STRINGIZE(FIRST_ARG(a, b, c)), "a") == 0);

    /* ---- LAST_ARG ---- */
    assert(strcmp(STRINGIZE(LAST_ARG(a, b, c)), "c") == 0);
    assert(strcmp(STRINGIZE(LAST_ARG(x, y)), "y") == 0);

    /* ---- DOT_JOIN_ARGS ---- */
    /* This creates token sequences like a.b.c — we test via stringification */
    assert(strcmp(STRINGIZE(DOT_JOIN_ARGS(a)),       "a") == 0);
    assert(strcmp(STRINGIZE(DOT_JOIN_ARGS(a,b)),     "a.b") == 0);
    assert(strcmp(STRINGIZE(DOT_JOIN_ARGS(a,b,c,d)), "a.b.c.d") == 0);

    printf("All macro tests passed!\n");
}

