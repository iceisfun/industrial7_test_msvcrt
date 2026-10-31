/* TC
 * id:    pfm_3_prec_minus_star_all_conv
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1p4 (negative precision argument is taken as if precision were omitted); MS docs printf precision
 * note:  star argument handling per C11; MS docs describe the * width/precision forms only loosely
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile double d = 1.5;
TC_CASE(ints)
{
    char b[64]; volatile int i = 42; volatile unsigned x = 255;
    sprintf(b, "[%.*d][%.*u][%.*x]", -3, i, -3, (unsigned)i, -3, x);
    CHECK_STR(b, "[42][42][ff]");
}
TC_CASE(str) { char b[64]; static const char s[] = "abc"; sprintf(b, "[%.*s]", -1, s); CHECK_STR(b, "[abc]"); }
TC_CASE(f) { char b[64]; sprintf(b, "[%.*f]", -2, d); CHECK_STR(b, "[1.500000]"); }
TC_CASE(e)
{
    char b[64]; sprintf(b, "[%.*e]", -1, d);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "[1.500000e+00]");
#else
    CHECK_STR(b, "[1.500000e+000]");
#endif
}
TC_CASE(g) { char b[64]; volatile double v = 1234567.0; sprintf(b, "[%.*g]", -1, v); 
#if TC_AT_LEAST(140)
    CHECK_STR(b, "[1.23457e+06]");
#else
    CHECK_STR(b, "[1.23457e+006]");
#endif
}
TC_CASE(c) { char b[64]; volatile int c = 'x'; sprintf(b, "[%.*c]", -1, c); CHECK_STR(b, "[x]"); }
int main(void) { tc_begin(); TC_RUN(ints); TC_RUN(str); TC_RUN(f); TC_RUN(e); TC_RUN(g); TC_RUN(c); return tc_end(); }
