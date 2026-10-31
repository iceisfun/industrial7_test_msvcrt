/* TC
 * id:    mb_neg_wcstombs_s_small
 * area:  mbcs
 * funcs: setlocale
 * procs: wcstombs_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcstombs_s (ERANGE, EINVAL, EILSEQ), Parameter validation
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

TC_CASE(too_small)
{
    char b[8]; size_t rv = 99; errno_t e; const wchar_t *volatile s = L"abc";
    b[0] = 0x55;
    tc_iph_count = 0;
    e = tc_p_wcstombs_s(&rv, b, 3, s, 3);
    CHECK_EQ_I(e, ERANGE); CHECK(tc_iph_count >= 1);
}
TC_CASE(null_src)
{
    char b[8]; size_t rv = 99; errno_t e;
    tc_iph_count = 0;
    e = tc_p_wcstombs_s(&rv, b, 8, NULL, 3);
    CHECK_EQ_I(e, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(dst_null_size_nonzero)
{
    size_t rv = 99; errno_t e; const wchar_t *volatile s = L"abc"; char *volatile d = NULL;
    tc_iph_count = 0;
    e = tc_p_wcstombs_s(&rv, d, 8, s, 3);
    CHECK_EQ_I(e, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(unmappable)
{
    char b[8]; size_t rv = 99; errno_t e; const wchar_t s[] = { L'a', 0x4E00, 0 };
    setlocale(LC_ALL, "C");
    e = tc_p_wcstombs_s(&rv, b, 8, s, 8);
    CHECK_EQ_I(e, EILSEQ);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(wcstombs_s); CHECK(tc_p_wcstombs_s);
    if (!tc_p_wcstombs_s) return tc_end();
    tc_iph_install();
    TC_RUN(too_small); TC_RUN(null_src); TC_RUN(dst_null_size_nonzero); TC_RUN(unmappable);
    return tc_end();
}
