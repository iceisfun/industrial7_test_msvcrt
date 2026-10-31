/* TC
 * id:    mb_wcstombs_s_family
 * area:  mbcs
 * funcs: 
 * procs: wcstombs_s
 * crt:   >=80
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs wcstombs_s, _wcstombs_s_l
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>

TC_DECL_PROC(errno_t, wcstombs_s, (size_t *, char *, size_t, const wchar_t *, size_t));

static int set932(void)
{
    if (setlocale(LC_ALL, "Japanese_Japan.932")) return 1;
    return setlocale(LC_ALL, ".932") != NULL;
}

#ifndef STRUNCATE
#define STRUNCATE 80
#endif
TC_CASE(success)
{
    char b[8]; size_t rv = 99; errno_t e; const wchar_t *volatile s = L"abc";
    e = tc_p_wcstombs_s(&rv, b, 8, s, 8);
    CHECK_EQ_I(e, 0); CHECK_EQ_U(rv, 4); CHECK_STR(b, "abc");
}
TC_CASE(query_size)
{
    size_t rv = 99; errno_t e; const wchar_t *volatile s = L"abcde";
    e = tc_p_wcstombs_s(&rv, NULL, 0, s, 0);
    CHECK_EQ_I(e, 0); CHECK_EQ_U(rv, 6);
}
TC_CASE(truncate)
{
    char b[8]; size_t rv = 99; errno_t e; const wchar_t *volatile s = L"abcdef";
    e = tc_p_wcstombs_s(&rv, b, 4, s, (size_t)-1);
    CHECK_EQ_I(e, STRUNCATE); CHECK_STR(b, "abc");
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(wcstombs_s); CHECK(tc_p_wcstombs_s);
    if (!tc_p_wcstombs_s) return tc_end();
    tc_iph_install();
    TC_RUN(success); TC_RUN(query_size); TC_RUN(truncate);
    return tc_end();
}
