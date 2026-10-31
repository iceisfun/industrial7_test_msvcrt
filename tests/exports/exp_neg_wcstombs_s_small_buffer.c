/* TC
 * id:    exp_neg_wcstombs_s_small_buffer
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: wcstombs_s mbstowcs_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs wcstombs_s / mbstowcs_s: ERANGE if destination too small; with _TRUNCATE STRUNCATE; NULL dest with nonzero size EINVAL
 * note:  ASCII-only input to stay locale independent. iph count not asserted for ERANGE.
 */
#include "tc.h"
#include <errno.h>

#ifndef STRUNCATE
#define STRUNCATE 80
#endif
#define TRUNC ((size_t)-1)
TC_DECL_PROC(errno_t, wcstombs_s, (size_t *, char *, size_t, const wchar_t *, size_t));
TC_DECL_PROC(errno_t, mbstowcs_s, (size_t *, wchar_t *, size_t, const char *, size_t));

TC_CASE(wcstombs_s_erange) { size_t r = 99; char d[3] = {1,1,1}; CHECK_EQ_I(tc_p_wcstombs_s(&r, d, 3, L"hello", 5), ERANGE); }
TC_CASE(wcstombs_s_truncate_strunc)
{
    size_t r = 0; char d[4] = {1,1,1,1};
    CHECK_EQ_I(tc_p_wcstombs_s(&r, d, 4, L"hello", TRUNC), STRUNCATE);
    CHECK_EQ_U(r, 4); CHECK_STR(d, "hel");
}
TC_CASE(mbstowcs_s_erange) { size_t r = 99; wchar_t d[3] = {1,1,1}; CHECK_EQ_I(tc_p_mbstowcs_s(&r, d, 3, "hello", 5), ERANGE); }
TC_CASE(null_dest_nonzero_size_einval)
{
    size_t r = 0;
    tc_iph_count = 0; CHECK_EQ_I(tc_p_wcstombs_s(&r, NULL, 5, L"hi", 2), EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; CHECK_EQ_I(tc_p_mbstowcs_s(&r, NULL, 5, "hi", 2), EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin(); TC_LOAD_PROC(wcstombs_s); TC_LOAD_PROC(mbstowcs_s); tc_iph_install();
    if (!tc_p_wcstombs_s || !tc_p_mbstowcs_s) { tc_write("TC FAIL " TC_TEST_ID "::exported\n"); return 1; }
    TC_RUN(wcstombs_s_erange); TC_RUN(wcstombs_s_truncate_strunc); TC_RUN(mbstowcs_s_erange); TC_RUN(null_dest_nonzero_size_einval);
    return tc_end();
}
