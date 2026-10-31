/* TC
 * id:    ucr_stdio_vsprintf_star_width_prec
 * area:  ucrt_only
 * funcs: __stdio_common_vsprintf
 * crt:   win10
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 paragraphs 5-6 (* width/precision; negative width = '-' flag; negative precision = omitted)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

static int vs(unsigned long long opts, char *b, size_t n, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vsprintf(opts, b, n, f, NULL, ap); va_end(ap);
    return r;
}
static double mk(unsigned long long bits) { union { double d; unsigned long long u; } x; x.u = bits; return x.d; }
static volatile unsigned long long v_bits;
static double dv(unsigned long long bits) { v_bits = bits; return mk(v_bits); }

TC_CASE(width) { char b[64]; vs(0, b, sizeof b, "[%*d]", 6, 42); CHECK_STR(b, "[    42]"); }
TC_CASE(left_flag) { char b[64]; vs(0, b, sizeof b, "[%-*d]", 6, 42); CHECK_STR(b, "[42    ]"); }
TC_CASE(negative_width) { char b[64]; vs(0, b, sizeof b, "[%*d]", -6, 42); CHECK_STR(b, "[42    ]"); }
TC_CASE(str_prec) { char b[64]; vs(0, b, sizeof b, "[%.*s]", 3, "abcdef"); CHECK_STR(b, "[abc]"); }
TC_CASE(width_and_prec) { char b[64]; vs(0, b, sizeof b, "[%*.*s]", 6, 2, "abcdef"); CHECK_STR(b, "[    ab]"); }
TC_CASE(negative_prec) { char b[64]; vs(0, b, sizeof b, "[%.*d][%.*s]", -1, 42, -1, "abcdef"); CHECK_STR(b, "[42][abcdef]"); }

int main(void)
{
    tc_begin();
    TC_RUN(width);
    TC_RUN(left_flag);
    TC_RUN(negative_width);
    TC_RUN(str_prec);
    TC_RUN(width_and_prec);
    TC_RUN(negative_prec);
    return tc_end();
}
