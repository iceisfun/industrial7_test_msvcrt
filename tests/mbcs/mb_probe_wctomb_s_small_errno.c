/* TC
 * id:    mb_probe_wctomb_s_small_errno
 * area:  mbcs
 * funcs: setlocale
 * procs: wctomb_s
 * crt:   >=80
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs wctomb_s
 * note:  PROBE: what wctomb_s returns when the destination (size 1) is too small for a 2-byte CP932 char (U+3042).
 *        Expect EINVAL (22), *pRetValue unchanged-or-(-1), buffer[0] untouched (the CRT may instead zero it, as the *_s string functions do), iph count >= 1. Each guess is its own case. Alternative: ERANGE (34). Measured on real Windows.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>

TC_DECL_PROC(errno_t, wctomb_s, (int *, char *, size_t, wchar_t));

static int set932(void)
{
    if (setlocale(LC_ALL, "Japanese_Japan.932")) return 1;
    return setlocale(LC_ALL, ".932") != NULL;
}

static errno_t small_call(int *rv, char *b)
{
    volatile size_t sz = 1; errno_t e;
    b[0] = 0x55; tc_iph_count = 0;
    e = tc_p_wctomb_s(rv, b, sz, 0x3042);
    setlocale(LC_ALL, "C");
    return e;
}
TC_CASE(small_errno_einval)
{
    char b[4]; int rv = 99;
    if (!set932()) { setlocale(LC_ALL, "C"); TC_SKIP("932 locale unavailable"); }
    CHECK_EQ_I(small_call(&rv, b), EINVAL);
}
TC_CASE(small_iph)
{
    char b[4]; int rv = 99;
    if (!set932()) { setlocale(LC_ALL, "C"); TC_SKIP("932 locale unavailable"); }
    small_call(&rv, b);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(small_buf_untouched)
{
    char b[4]; int rv = 99;
    if (!set932()) { setlocale(LC_ALL, "C"); TC_SKIP("932 locale unavailable"); }
    small_call(&rv, b);
    CHECK_EQ_U((unsigned char)b[0], 0x55);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(wctomb_s); CHECK(tc_p_wctomb_s);
    if (!tc_p_wctomb_s) return tc_end();
    tc_iph_install();
    TC_RUN(small_errno_einval); TC_RUN(small_iph); TC_RUN(small_buf_untouched);
    return tc_end();
}
