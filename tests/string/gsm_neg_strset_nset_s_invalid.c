/* TC
 * id:    gsm_neg_strset_nset_s_invalid
 * area:  string
 * kind:  negative
 * funcs:
 * procs: _strset_s _strnset_s _wcsset_s _wcsnset_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _strset_s, _strnset_s, _wcsset_s, _wcsnset_s (EINVAL for NULL str, or str not terminated within numberOfElements)
 * note:  Resolved through procs to avoid header differences. Fill-value range and count > size are probed separately in gsm_neg_strset_s_fill_probe.
 */
#include "tc.h"
#include <string.h>
#include <wchar.h>
#define EINVAL_ 22
TC_DECL_PROC(int, _strset_s, (char *, size_t, int));
TC_DECL_PROC(int, _strnset_s, (char *, size_t, int, size_t));
TC_DECL_PROC(int, _wcsset_s, (wchar_t *, size_t, wchar_t));
TC_DECL_PROC(int, _wcsnset_s, (wchar_t *, size_t, wchar_t, size_t));

TC_CASE(exported) { CHECK(tc_p__strset_s && tc_p__strnset_s && tc_p__wcsset_s && tc_p__wcsnset_s); }
TC_CASE(null_dest)
{
    tc_iph_count = 0;
    CHECK_EQ_I(tc_p__strset_s(NULL, 8, 'x'), EINVAL_); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(tc_p__strnset_s(NULL, 8, 'x', 2), EINVAL_); CHECK_EQ_I(tc_iph_count, 2);
}
TC_CASE(size_zero)
{
    char b[8] = "abc";
    tc_iph_count = 0;
    CHECK_EQ_I(tc_p__strset_s(b, 0, 'x'), EINVAL_); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(tc_p__strnset_s(b, 0, 'x', 1), EINVAL_); CHECK_EQ_I(tc_iph_count, 2);
    CHECK_STR(b, "abc");
}
TC_CASE(unterminated)
{
    char b[8]; tc_iph_count = 0;
    memset(b, 'a', 8);
    CHECK_EQ_I(tc_p__strset_s(b, 4, 'x'), EINVAL_); CHECK_EQ_I(tc_iph_count, 1);
    memset(b, 'a', 8);
    CHECK_EQ_I(tc_p__strnset_s(b, 4, 'x', 2), EINVAL_); CHECK_EQ_I(tc_iph_count, 2);
}
TC_CASE(valid_fill)
{
    char b[8] = "abc";
    tc_iph_count = 0;
    CHECK_EQ_I(tc_p__strset_s(b, sizeof b, 'x'), 0); CHECK_STR(b, "xxx");
    CHECK_EQ_I(tc_p__strnset_s(b, sizeof b, 'y', 1), 0); CHECK_STR(b, "yxx");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wide)
{
    wchar_t b[8]; int i; tc_iph_count = 0;
    CHECK_EQ_I(tc_p__wcsset_s(NULL, 8, L'x'), EINVAL_); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(tc_p__wcsnset_s(NULL, 8, L'x', 1), EINVAL_); CHECK_EQ_I(tc_iph_count, 2);
    b[0] = L'a'; b[1] = 0;
    CHECK_EQ_I(tc_p__wcsset_s(b, 0, L'x'), EINVAL_); CHECK_EQ_I(tc_iph_count, 3);
    CHECK_EQ_I(tc_p__wcsnset_s(b, 0, L'x', 1), EINVAL_); CHECK_EQ_I(tc_iph_count, 4);
    for (i = 0; i < 8; i++) b[i] = L'a';
    CHECK_EQ_I(tc_p__wcsset_s(b, 4, L'x'), EINVAL_); CHECK_EQ_I(tc_iph_count, 5);
    for (i = 0; i < 8; i++) b[i] = L'a';
    CHECK_EQ_I(tc_p__wcsnset_s(b, 4, L'x', 2), EINVAL_); CHECK_EQ_I(tc_iph_count, 6);
    b[0] = L'a'; b[1] = L'b'; b[2] = 0;
    CHECK_EQ_I(tc_p__wcsset_s(b, 8, 0x20AC), 0); CHECK_EQ_U(b[0], 0x20AC); CHECK_EQ_U(b[1], 0x20AC); CHECK_EQ_U(b[2], 0);
    CHECK_EQ_I(tc_p__wcsnset_s(b, 8, L'q', 1), 0); CHECK_EQ_U(b[0], L'q'); CHECK_EQ_U(b[1], 0x20AC);
    CHECK_EQ_I(tc_iph_count, 6);
}
int main(void)
{
    tc_begin(); if (!tc_iph_install()) return 3;
    TC_LOAD_PROC(_strset_s); TC_LOAD_PROC(_strnset_s); TC_LOAD_PROC(_wcsset_s); TC_LOAD_PROC(_wcsnset_s);
    TC_RUN(exported);
    if (!(tc_p__strset_s && tc_p__strnset_s && tc_p__wcsset_s && tc_p__wcsnset_s)) return tc_end();
    TC_RUN(null_dest); TC_RUN(size_zero); TC_RUN(unterminated); TC_RUN(valid_fill); TC_RUN(wide);
    return tc_end();
}
