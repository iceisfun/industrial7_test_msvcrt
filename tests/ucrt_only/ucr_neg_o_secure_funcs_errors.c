/* TC
 * id:    ucr_neg_o_secure_funcs_errors
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: _o_strcpy_s _o_wcscpy_s _o_memcpy_s _o__itoa_s
 * crt:   win10
 * tier:  p1
 * conf:  documented
 * ref:   MS docs strcpy_s/wcscpy_s (EINVAL for NULL, ERANGE for small dest, dest[0]=0), memcpy_s, _itoa_s (ERANGE); _o_ aliases are the same functions
 * note:  Buffer state after _itoa_s failure and memcpy_s dest zeroing are not asserted here (see ucr_neg_o_memcpy_s_zeroes_probe).
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *strcpy_s_t)(char *, size_t, const char *);
typedef int (__cdecl *wcscpy_s_t)(wchar_t *, size_t, const wchar_t *);
typedef int (__cdecl *memcpy_s_t)(void *, size_t, const void *, size_t);
typedef int (__cdecl *itoa_s_t)(int, char *, size_t, int);
static strcpy_s_t p_strcpy_s; static wcscpy_s_t p_wcscpy_s; static memcpy_s_t p_memcpy_s; static itoa_s_t p_itoa_s;

TC_CASE(strcpy_null_dest) { tc_iph_count = 0; CHECK_EQ_I(p_strcpy_s(NULL, 8, "abc"), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(strcpy_small)
{
    char d[3] = {'x', 'x', 'x'}; tc_iph_count = 0;
    CHECK_EQ_I(p_strcpy_s(d, sizeof d, "abcdef"), ERANGE); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wcscpy_small)
{
    wchar_t d[3] = {L'x', L'x', L'x'}; tc_iph_count = 0;
    CHECK_EQ_I(p_wcscpy_s(d, 3, L"abcdef"), ERANGE); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(memcpy_small)
{
    char d[2] = {'x', 'x'}; static const char s[8] = "abcdefg"; tc_iph_count = 0;
    CHECK_EQ_I(p_memcpy_s(d, sizeof d, s, 8), ERANGE); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(itoa_small)
{
    char b[2] = {'x', 'x'}; tc_iph_count = 0;
    CHECK_EQ_I(p_itoa_s(12345, b, 2, 10), ERANGE); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    p_strcpy_s = (strcpy_s_t)tc_crt_proc("_o_strcpy_s"); p_wcscpy_s = (wcscpy_s_t)tc_crt_proc("_o_wcscpy_s");
    p_memcpy_s = (memcpy_s_t)tc_crt_proc("_o_memcpy_s"); p_itoa_s = (itoa_s_t)tc_crt_proc("_o__itoa_s");
    if (!p_strcpy_s || !p_wcscpy_s || !p_memcpy_s || !p_itoa_s || !tc_iph_install()) { tc_write("TC FAIL missing proc\n"); return 1; }
    TC_RUN(strcpy_null_dest); TC_RUN(strcpy_small); TC_RUN(wcscpy_small); TC_RUN(memcpy_small); TC_RUN(itoa_small);
    return tc_end();
}
