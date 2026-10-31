/* TC
 * id:    ucr_neg_stdio_vsscanf_match_fail
 * area:  ucrt_only
 * kind:  negative
 * funcs: __stdio_common_vsscanf
 * crt:   win10
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2 paragraph 16 (return value: EOF on input failure before first conversion, else number assigned); MS docs sscanf return value
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

TC_CASE(no_digits) { int v = 99; CHECK_EQ_I(vsc(0, "abc", "%d", &v), 0); CHECK_EQ_I(v, 99); }
TC_CASE(second_fails) { int a = 0, b = 55; CHECK_EQ_I(vsc(0, "1 x", "%d %d", &a, &b), 1); CHECK_EQ_I(a, 1); CHECK_EQ_I(b, 55); }
TC_CASE(empty_input) { int v = 99; CHECK_EQ_I(vsc(0, "", "%d", &v), EOF); CHECK_EQ_I(v, 99); }
TC_CASE(whitespace_only) { int v = 99; CHECK_EQ_I(vsc(0, "   ", "%d", &v), EOF); CHECK_EQ_I(v, 99); }
TC_CASE(literal_mismatch) { int v = 99; CHECK_EQ_I(vsc(0, "a1", "b%d", &v), 0); CHECK_EQ_I(v, 99); }
TC_CASE(scanset_no_match) { char s[8] = "zz"; CHECK_EQ_I(vsc(0, "xyz", "%[a-c]", s), 0); CHECK_STR(s, "zz"); }

int main(void)
{
    tc_begin();
    TC_RUN(no_digits);
    TC_RUN(second_fails);
    TC_RUN(empty_input);
    TC_RUN(whitespace_only);
    TC_RUN(literal_mismatch);
    TC_RUN(scanset_no_match);
    return tc_end();
}
