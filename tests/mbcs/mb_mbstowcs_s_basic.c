/* TC
 * id:    mb_mbstowcs_s_basic
 * area:  mbcs
 * funcs: 
 * procs: mbstowcs_s
 * crt:   >=80
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs mbstowcs_s, _mbstowcs_s_l
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

TC_CASE(exact_fit)
{
    wchar_t w[4]; size_t rv = 99; errno_t e; const char *volatile s = "abc";
    e = tc_p_mbstowcs_s(&rv, w, 4, s, 3);
    CHECK_EQ_I(e, 0); CHECK_EQ_U(rv, 4); CHECK_WSTR(w, L"abc");
}
TC_CASE(query_size)
{
    size_t rv = 99; errno_t e; const char *volatile s = "abcde";
    e = tc_p_mbstowcs_s(&rv, NULL, 0, s, 0);
    CHECK_EQ_I(e, 0); CHECK_EQ_U(rv, 6);
}
TC_CASE(truncate_exact)
{
    wchar_t w[4]; size_t rv = 99; errno_t e; const char *volatile s = "abc";
    e = tc_p_mbstowcs_s(&rv, w, 4, s, (size_t)-1);
    CHECK_EQ_I(e, 0); CHECK_EQ_U(rv, 4); CHECK_WSTR(w, L"abc");
}
TC_CASE(count_limits)
{
    wchar_t w[10]; size_t rv = 99; errno_t e; const char *volatile s = "abcdef";
    e = tc_p_mbstowcs_s(&rv, w, 10, s, 2);
    CHECK_EQ_I(e, 0); CHECK_EQ_U(rv, 3); CHECK_WSTR(w, L"ab");
}
TC_CASE(empty)
{
    wchar_t w[2]; size_t rv = 99; errno_t e; const char *volatile s = "";
    w[0] = 0x7777;
    e = tc_p_mbstowcs_s(&rv, w, 2, s, 1);
    CHECK_EQ_I(e, 0); CHECK_EQ_U(rv, 1); CHECK_EQ_U(w[0], 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(mbstowcs_s); CHECK(tc_p_mbstowcs_s);
    if (!tc_p_mbstowcs_s) return tc_end();
    tc_iph_install();
    TC_RUN(exact_fit); TC_RUN(query_size); TC_RUN(truncate_exact); TC_RUN(count_limits); TC_RUN(empty);
    return tc_end();
}
