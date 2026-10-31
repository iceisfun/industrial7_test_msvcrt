/* TC
 * id:    ucr_stdio_vsscanf_basic
 * area:  ucrt_only
 * funcs: __stdio_common_vsscanf
 * crt:   win10
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2; MS docs sscanf (options 0 = non-secure, %s needs no size)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <errno.h>

static int vsc(unsigned long long opts, const char *in, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vsscanf(opts, in, (size_t)-1, f, NULL, ap); va_end(ap);
    return r;
}

TC_CASE(three) { int d = 0; char s[16] = ""; float f = 0; int r = vsc(0, "12 abc 3.5", "%d %s %f", &d, s, &f);
    CHECK_EQ_I(r, 3); CHECK_EQ_I(d, 12); CHECK_STR(s, "abc"); CHECK(f == 3.5f); }
TC_CASE(c_reads_space) { char c = 'x'; int r = vsc(0, " y", "%c", &c); CHECK_EQ_I(r, 1); CHECK_EQ_I(c, ' '); }
TC_CASE(space_then_c) { char c = 'x'; int r = vsc(0, " y", " %c", &c); CHECK_EQ_I(r, 1); CHECK_EQ_I(c, 'y'); }
TC_CASE(counts_assignments) { int a = 0, b = 0, c = 77; int r = vsc(0, "1 2", "%d %d %d", &a, &b, &c);
    CHECK_EQ_I(r, 2); CHECK_EQ_I(a, 1); CHECK_EQ_I(b, 2); CHECK_EQ_I(c, 77); }

int main(void)
{
    tc_begin();
    TC_RUN(three);
    TC_RUN(c_reads_space);
    TC_RUN(space_then_c);
    TC_RUN(counts_assignments);
    return tc_end();
}
