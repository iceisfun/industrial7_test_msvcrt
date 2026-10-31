/* TC
 * id:    sec_core_mem_wide_via_procs
 * area:  secure_crt
 * funcs:
 * procs: memcpy_s memmove_s wcscpy_s wcscat_s _itoa_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs memcpy_s, memmove_s, wcscpy_s, wcscat_s, _itoa_s
 * note:  Called via GetProcAddress so msvcr80 (missing from mingw import lib) is covered.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, memcpy_s, (void *, size_t, const void *, size_t));
TC_DECL_PROC(int, memmove_s, (void *, size_t, const void *, size_t));
TC_DECL_PROC(int, wcscpy_s, (wchar_t *, size_t, const wchar_t *));
TC_DECL_PROC(int, wcscat_s, (wchar_t *, size_t, const wchar_t *));
TC_DECL_PROC(int, _itoa_s, (int, char *, size_t, int));

TC_CASE(memcpy_s_ok) { char d[4] = {0}; CHECK_EQ_I(tc_p_memcpy_s(d, 4, "xyz", 3), 0); CHECK_MEM(d, "xyz", 3); }
TC_CASE(memmove_s_overlap) { char d[8] = "abcdef"; CHECK_EQ_I(tc_p_memmove_s(d + 2, 6, d, 4), 0); CHECK_STR(d, "ababcd"); }
TC_CASE(wcscpy_s_ok) { wchar_t d[8]; CHECK_EQ_I(tc_p_wcscpy_s(d, 8, L"abc"), 0); CHECK_WSTR(d, L"abc"); }
TC_CASE(wcscat_s_ok) { wchar_t d[8] = L"ab"; CHECK_EQ_I(tc_p_wcscat_s(d, 8, L"cd"), 0); CHECK_WSTR(d, L"abcd"); }
TC_CASE(itoa_s_radix16) { char d[16]; CHECK_EQ_I(tc_p__itoa_s(255, d, 16, 16), 0); CHECK_STR(d, "ff"); }
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(memcpy_s); TC_LOAD_PROC(memmove_s); TC_LOAD_PROC(wcscpy_s); TC_LOAD_PROC(wcscat_s); TC_LOAD_PROC(_itoa_s);
    if (!(tc_p_memcpy_s && tc_p_memmove_s && tc_p_wcscpy_s && tc_p_wcscat_s && tc_p__itoa_s)) { tc_write("TC FAIL " TC_TEST_ID "::load missing export\n"); return 1; }
    TC_RUN(memcpy_s_ok); TC_RUN(memmove_s_overlap); TC_RUN(wcscpy_s_ok); TC_RUN(wcscat_s_ok); TC_RUN(itoa_s_radix16);
    return tc_end();
}
