/* TC
 * id:    pfm_3_neg_swprintf_s_trunc_each_conv
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s swprintf_s _snprintf_s _snwprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s, swprintf_s, _snprintf_s, _snwprintf_s (buffer too small: handler invoked, buffer[0] = 0, errno ERANGE, return -1)
 * note:  Buffer too small for one conversion class at a time: %d %s %c %f (%a on ucrtbase only; the legacy CRTs have no %a).
 *        On ucrtbase these functions are SDK inlines; on older CRTs they are called through the DLL exports. Float output is small
 *        enough (1.5 -> "1.500000") that legacy and UCRT agree on the length. Buffers are pre-filled with 'x'.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#if TC_CRT_VER >= 140
#define P(n) n
#else
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
TC_DECL_PROC(int, swprintf_s, (wchar_t *, size_t, const wchar_t *, ...));
TC_DECL_PROC(int, _snprintf_s, (char *, size_t, size_t, const char *, ...));
TC_DECL_PROC(int, _snwprintf_s, (wchar_t *, size_t, size_t, const wchar_t *, ...));
#define P(n) tc_p_##n
#endif

static volatile int v12345 = 12345;
static volatile double d15 = 1.5;
static const char *volatile sabc = "abcdef";
static const wchar_t *volatile wabc = L"abcdef";

#define BEGIN tc_iph_count = 0; errno = 0
#define RANGE_FAIL(r, b0) do { CHECK_EQ_I(r, -1); CHECK_EQ_I(b0, 0); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); } while (0)
#define NOISE(b) do { size_t i_; for (i_ = 0; i_ < sizeof(b) / sizeof((b)[0]); i_++) (b)[i_] = 'x'; } while (0)

TC_CASE(sprintf_s_d)  { char b[8]; int r; NOISE(b); BEGIN; r = P(sprintf_s)(b, 4, "%d", v12345); RANGE_FAIL(r, b[0]); }
TC_CASE(sprintf_s_s)  { char b[8]; int r; NOISE(b); BEGIN; r = P(sprintf_s)(b, 4, "%s", sabc); RANGE_FAIL(r, b[0]); }
TC_CASE(sprintf_s_c)  { char b[8]; int r; NOISE(b); BEGIN; r = P(sprintf_s)(b, 1, "%c", 'q'); RANGE_FAIL(r, b[0]); }
TC_CASE(sprintf_s_f)  { char b[16]; int r; NOISE(b); BEGIN; r = P(sprintf_s)(b, 4, "%f", d15); RANGE_FAIL(r, b[0]); }
#if TC_CRT_VER >= 140
TC_CASE(sprintf_s_a)  { char b[16]; int r; NOISE(b); BEGIN; r = P(sprintf_s)(b, 4, "%a", d15); RANGE_FAIL(r, b[0]); }
#endif
TC_CASE(swprintf_s_each)
{
    wchar_t b[16]; int r;
    NOISE(b); BEGIN; r = P(swprintf_s)(b, 4, L"%d", v12345); RANGE_FAIL(r, b[0]);
    NOISE(b); BEGIN; r = P(swprintf_s)(b, 4, L"%ls", wabc); RANGE_FAIL(r, b[0]);
    NOISE(b); BEGIN; r = P(swprintf_s)(b, 1, L"%c", (wchar_t)L'q'); RANGE_FAIL(r, b[0]);
    NOISE(b); BEGIN; r = P(swprintf_s)(b, 4, L"%f", d15); RANGE_FAIL(r, b[0]);
}
TC_CASE(snprintf_s_size_too_small)
{
    char b[16]; int r;
    NOISE(b); BEGIN; r = P(_snprintf_s)(b, 4, 10, "%d", v12345); RANGE_FAIL(r, b[0]);
}
TC_CASE(snwprintf_s_size_too_small)
{
    wchar_t b[16]; int r;
    NOISE(b); BEGIN; r = P(_snwprintf_s)(b, 4, 10, L"%d", v12345); RANGE_FAIL(r, b[0]);
}
int main(void)
{
    tc_begin();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(sprintf_s); TC_LOAD_PROC(swprintf_s); TC_LOAD_PROC(_snprintf_s); TC_LOAD_PROC(_snwprintf_s);
    if (!tc_p_sprintf_s || !tc_p_swprintf_s || !tc_p__snprintf_s || !tc_p__snwprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__missing _s export\n"); return 2; }
#endif
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(sprintf_s_d); TC_RUN(sprintf_s_s); TC_RUN(sprintf_s_c); TC_RUN(sprintf_s_f);
#if TC_CRT_VER >= 140
    TC_RUN(sprintf_s_a);
#endif
    TC_RUN(swprintf_s_each); TC_RUN(snprintf_s_size_too_small); TC_RUN(snwprintf_s_size_too_small);
        return tc_end();
}
