/* TC
 * id:    pfm_3_neg_snprintf_s_truncate_each_conv
 * area:  stdio_printf
 * funcs:
 * procs: _snprintf_s _snwprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf_s, _snwprintf_s (count == _TRUNCATE: write as much as fits, null-terminate, return -1, no invalid-parameter call; count smaller than needed: output cut to count, return -1)
 * note:  Companion of pfm_3_neg_swprintf_s_trunc_each_conv (hard failure with ERANGE). errno after a _TRUNCATE truncation is not asserted.
 *        Buffers are pre-filled with 'x'. Older CRTs call the DLL exports through procs; ucrtbase uses the SDK inlines.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#if TC_CRT_VER >= 140
#define P(n) n
#else
TC_DECL_PROC(int, _snprintf_s, (char *, size_t, size_t, const char *, ...));
TC_DECL_PROC(int, _snwprintf_s, (wchar_t *, size_t, size_t, const wchar_t *, ...));
#define P(n) tc_p_##n
#endif

static volatile int v12345 = 12345;
static volatile double d15 = 1.5;
static const char *volatile sabc = "abcdef";
static const wchar_t *volatile wabc = L"abcdef";



#define BEGIN tc_iph_count = 0; errno = 0
#define NOISE(b) do { size_t i_; for (i_ = 0; i_ < sizeof(b) / sizeof((b)[0]); i_++) (b)[i_] = 'x'; } while (0)

TC_CASE(snprintf_s_truncate_each)
{
    char b[16]; int r;
    NOISE(b); BEGIN; r = P(_snprintf_s)(b, 4, _TRUNCATE, "%d", v12345);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(b, "123");
    NOISE(b); BEGIN; r = P(_snprintf_s)(b, 4, _TRUNCATE, "%s", sabc);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(b, "abc");
    NOISE(b); BEGIN; r = P(_snprintf_s)(b, 4, _TRUNCATE, "%f", d15);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(b, "1.5");
}
TC_CASE(snwprintf_s_truncate_each)
{
    wchar_t b[16]; int r;
    NOISE(b); BEGIN; r = P(_snwprintf_s)(b, 4, _TRUNCATE, L"%d", v12345);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0); CHECK_WSTR(b, L"123");
    NOISE(b); BEGIN; r = P(_snwprintf_s)(b, 4, _TRUNCATE, L"%ls", wabc);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0); CHECK_WSTR(b, L"abc");
}
TC_CASE(snprintf_s_count_cut)
{
    char b[16]; int r;
    NOISE(b); BEGIN; r = P(_snprintf_s)(b, 10, 3, "%d", v12345);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(b, "123");
}

int main(void)
{
    tc_begin();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(_snprintf_s); TC_LOAD_PROC(_snwprintf_s);
    if (!tc_p__snprintf_s || !tc_p__snwprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__missing _s export\n"); return 2; }
#endif
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(snprintf_s_truncate_each); TC_RUN(snwprintf_s_truncate_each); TC_RUN(snprintf_s_count_cut);
    return tc_end();
}
