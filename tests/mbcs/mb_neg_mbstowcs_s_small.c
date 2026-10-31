/* TC
 * id:    mb_neg_mbstowcs_s_small
 * area:  mbcs
 * funcs: setlocale
 * procs: mbstowcs_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs mbstowcs_s (ERANGE, EINVAL, EILSEQ, STRUNCATE), Parameter validation
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>

TC_DECL_PROC(errno_t, mbstowcs_s, (size_t *, wchar_t *, size_t, const char *, size_t));

static int set932(void)
{
    if (setlocale(LC_ALL, "Japanese_Japan.932")) return 1;
    return setlocale(LC_ALL, ".932") != NULL;
}

#ifndef STRUNCATE
#define STRUNCATE 80
#endif
TC_CASE(too_small)
{
    wchar_t w[4]; size_t rv = 99; errno_t e; const char *volatile s = "abc";
    w[0] = 0x7777;
    tc_iph_count = 0;
    e = tc_p_mbstowcs_s(&rv, w, 2, s, 3);
    CHECK_EQ_I(e, ERANGE); CHECK(tc_iph_count >= 1);
}
TC_CASE(truncate)
{
    wchar_t w[4]; size_t rv = 99; errno_t e; const char *volatile s = "abc";
    w[0] = w[1] = w[2] = 0x7777;
    tc_iph_count = 0;
    e = tc_p_mbstowcs_s(&rv, w, 2, s, (size_t)-1);
    CHECK_EQ_I(e, STRUNCATE); CHECK_EQ_U(w[0], 'a'); CHECK_EQ_U(w[1], 0);
}
TC_CASE(null_src)
{
    wchar_t w[4]; size_t rv = 99; errno_t e;
    tc_iph_count = 0;
    e = tc_p_mbstowcs_s(&rv, w, 4, NULL, 3);
    CHECK_EQ_I(e, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(dst_size_zero)
{
    wchar_t w[4]; size_t rv = 99; errno_t e; const char *volatile s = "abc"; volatile size_t sz = 0;
    tc_iph_count = 0;
    e = tc_p_mbstowcs_s(&rv, w, sz, s, 3);
    CHECK_EQ_I(e, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(invalid_sequence)
{
    wchar_t w[8]; size_t rv = 99; errno_t e; const char s[] = { (char)0x82, 0x20, 0 };
    if (!set932()) { setlocale(LC_ALL, "C"); TC_SKIP("932 locale unavailable"); }
    w[0] = 0x7777;
    e = tc_p_mbstowcs_s(&rv, w, 8, s, 8);
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(e, EILSEQ);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(mbstowcs_s); CHECK(tc_p_mbstowcs_s);
    if (!tc_p_mbstowcs_s) return tc_end();
    tc_iph_install();
    TC_RUN(too_small); TC_RUN(truncate); TC_RUN(null_src); TC_RUN(dst_size_zero); TC_RUN(invalid_sequence);
    return tc_end();
}
