/* TC
 * id:    pfr_neg_snprintf_legacy_truncate
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _snprintf, _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf, _snwprintf, _vsnprintf: if len > count, count chars are stored (no NUL) and -1 is returned
 * note:  the legacy functions never write beyond count elements; sentinel past count must stay intact.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_DECL_PROC(int, _snprintf, (char *, size_t, const char *, ...));
TC_DECL_PROC(int, _snwprintf, (wchar_t *, size_t, const wchar_t *, ...));
#ifdef _UCRT
#define LOADP(n) (tc_p_##n = (tc_pt_##n)(n))
#else
#define LOADP(n) TC_LOAD_PROC(n)
#endif

TC_CASE(exported) { CHECK(tc_p__snprintf != NULL); CHECK(tc_p__snwprintf != NULL); }
TC_CASE(narrow_truncated)
{
    char b[12]; volatile size_t n = 3; int i, r;
    for (i = 0; i < 12; i++) b[i] = 'Z';
    r = tc_p__snprintf(b, n, "%s", "abcdef");
    CHECK_EQ_I(r, -1);
    CHECK_MEM(b, "abc", 3);
    for (i = 3; i < 12; i++) CHECK_EQ_I(b[i], 'Z');
}
TC_CASE(narrow_one_over)
{
    char b[12]; volatile size_t n = 5; int i, r;
    for (i = 0; i < 12; i++) b[i] = 'Z';
    r = tc_p__snprintf(b, n, "%s", "abcdef");
    CHECK_EQ_I(r, -1);
    CHECK_MEM(b, "abcde", 5);
    CHECK_EQ_I(b[5], 'Z');
}
TC_CASE(wide_truncated)
{
    wchar_t b[12]; volatile size_t n = 3; int i, r;
    for (i = 0; i < 12; i++) b[i] = L'Z';
    r = tc_p__snwprintf(b, n, L"%ls", L"abcdef");
    CHECK_EQ_I(r, -1);
    CHECK(b[0] == L'a' && b[1] == L'b' && b[2] == L'c');
    for (i = 3; i < 12; i++) CHECK(b[i] == L'Z');
}

int main(void) { tc_begin(); LOADP(_snprintf); LOADP(_snwprintf); TC_RUN(exported); TC_RUN(narrow_truncated); TC_RUN(narrow_one_over); TC_RUN(wide_truncated); return tc_end(); }
