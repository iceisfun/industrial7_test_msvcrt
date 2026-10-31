/* TC
 * id:    mem_neg_memcpy_s_success_keeps_errno
 * area:  memory
 * funcs:
 * procs: memcpy_s memmove_s wmemcpy_s wmemmove_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs memcpy_s, memmove_s, wmemcpy_s, wmemmove_s (return 0 on success; errno documented only for the error cases)
 * note:  Guards against over-eager errno clearing/setting and spurious handler calls on valid edge inputs (count == destsz, count 0, overlap). The wide variants are skipped if the DLL lacks them.
 */
#include "tc.h"
#include <stddef.h>
#include <errno.h>

TC_DECL_PROC(int, memcpy_s, (void *, size_t, const void *, size_t));
TC_DECL_PROC(int, memmove_s, (void *, size_t, const void *, size_t));
TC_DECL_PROC(int, wmemcpy_s, (wchar_t *, size_t, const wchar_t *, size_t));
TC_DECL_PROC(int, wmemmove_s, (wchar_t *, size_t, const wchar_t *, size_t));

static volatile size_t v_n;

TC_CASE(memcpy_s_ok)
{
    char d[8] = "XXXXXXX"; int r;
    if (!tc_p_memcpy_s) TC_SKIP("missing");
    v_n = 4; tc_iph_count = 0; errno = 1234;
    r = tc_p_memcpy_s(d, 8, "abcd", v_n);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, 1234); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_MEM(d, "abcdXXX", 7);
}
TC_CASE(count_equals_destsz)
{
    char d[4]; int r;
    if (!tc_p_memcpy_s || !tc_p_memmove_s) TC_SKIP("missing");
    v_n = 4; tc_iph_count = 0; errno = 1234;
    r = tc_p_memcpy_s(d, 4, "wxyz", v_n);
    CHECK_EQ_I(r, 0); CHECK_MEM(d, "wxyz", 4);
    d[0] = d[1] = d[2] = d[3] = 0;
    r = tc_p_memmove_s(d, 4, "1234", v_n);
    CHECK_EQ_I(r, 0); CHECK_MEM(d, "1234", 4);
    CHECK_EQ_I(errno, 1234); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_zero)
{
    char d[4] = "abc"; int r;
    if (!tc_p_memcpy_s || !tc_p_memmove_s) TC_SKIP("missing");
    v_n = 0; tc_iph_count = 0; errno = 1234;
    r = tc_p_memcpy_s(d, 4, "xyz", v_n); CHECK_EQ_I(r, 0);
    r = tc_p_memmove_s(d, 4, "xyz", v_n); CHECK_EQ_I(r, 0);
    CHECK_MEM(d, "abc", 4);
    CHECK_EQ_I(errno, 1234); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(memmove_s_overlap)
{
    char b[8] = "abcdefg"; int r;
    if (!tc_p_memmove_s) TC_SKIP("missing");
    v_n = 5; tc_iph_count = 0; errno = 1234;
    r = tc_p_memmove_s(b + 2, 5, b, v_n);
    CHECK_EQ_I(r, 0); CHECK_MEM(b, "ababcde", 7);
    r = tc_p_memmove_s(b, 7, b + 2, v_n);
    CHECK_EQ_I(r, 0); CHECK_MEM(b, "abcde", 5);
    CHECK_EQ_I(errno, 1234); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(wide_ok)
{
    wchar_t d[4] = {0}, s[4] = {1, 2, 3, 4}; int r;
    if (!tc_p_wmemcpy_s || !tc_p_wmemmove_s) TC_SKIP("wide missing");
    v_n = 4; tc_iph_count = 0; errno = 1234;
    r = tc_p_wmemcpy_s(d, 4, s, v_n);
    CHECK_EQ_I(r, 0); CHECK(d[0] == 1 && d[3] == 4);
    d[0] = d[1] = d[2] = d[3] = 0;
    r = tc_p_wmemmove_s(d, 4, s, v_n);
    CHECK_EQ_I(r, 0); CHECK(d[0] == 1 && d[3] == 4);
    v_n = 0;
    r = tc_p_wmemcpy_s(d, 4, s, v_n); CHECK_EQ_I(r, 0);
    r = tc_p_wmemmove_s(d, 4, s, v_n); CHECK_EQ_I(r, 0);
    CHECK_EQ_I(errno, 1234); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(memcpy_s); TC_LOAD_PROC(memmove_s); TC_LOAD_PROC(wmemcpy_s); TC_LOAD_PROC(wmemmove_s);
    CHECK(tc_iph_install());
    TC_RUN(memcpy_s_ok); TC_RUN(count_equals_destsz); TC_RUN(count_zero); TC_RUN(memmove_s_overlap); TC_RUN(wide_ok);
    return tc_end();
}
