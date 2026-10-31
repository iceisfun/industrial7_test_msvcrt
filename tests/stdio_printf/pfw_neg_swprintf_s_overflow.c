/* TC
 * id:    pfw_neg_swprintf_s_overflow
 * area:  stdio_printf
 * funcs:
 * procs: swprintf_s _snwprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s, swprintf_s: too-small buffer -> invalid parameter handler, returns -1, buffer[0] set to L'\0', errno ERANGE; _snwprintf_s with _TRUNCATE truncates and returns -1 with STRUNCATE not an error
 */
#include "tc.h"
#include <errno.h>
#include <wchar.h>

TC_DECL_PROC(int, swprintf_s, (wchar_t *, size_t, const wchar_t *, ...));
TC_DECL_PROC(int, _snwprintf_s, (wchar_t *, size_t, size_t, const wchar_t *, ...));
TC_CASE(overflow_empties_buffer)
{
    wchar_t b[8]; int n; int i;
    if (!tc_p_swprintf_s || !tc_iph_install()) TC_SKIP("missing");
    for (i = 0; i < 8; i++) b[i] = L'Z';
    tc_iph_count = 0; errno = 0;
    n = tc_p_swprintf_s(b, 4, L"%ls", L"abcdef");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, ERANGE); CHECK(b[0] == 0);
}
TC_CASE(null_buffer)
{
    int n;
    if (!tc_p_swprintf_s || !tc_iph_install()) TC_SKIP("missing");
    tc_iph_count = 0; errno = 0;
    n = tc_p_swprintf_s(NULL, 4, L"x");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(truncate_returns_minus1_no_handler)
{
    wchar_t b[8]; int n;
    if (!tc_p__snwprintf_s || !tc_iph_install()) TC_SKIP("missing");
    tc_iph_count = 0;
    n = tc_p__snwprintf_s(b, 4, (size_t)-1, L"%ls", L"abcdef");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 0);
    CHECK(b[0] == L'a' && b[1] == L'b' && b[2] == L'c' && b[3] == 0);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(swprintf_s); TC_LOAD_PROC(_snwprintf_s);
    TC_RUN(overflow_empties_buffer); TC_RUN(null_buffer); TC_RUN(truncate_returns_minus1_no_handler); return tc_end();
}
