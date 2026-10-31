/* TC
 * id:    pfr_neg_snprintf_s_state_not_poisoned
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf _scprintf
 * procs: sprintf_s _vsnprintf_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs sprintf_s, _snprintf_s, _snprintf, _scprintf (parameter validation, errno)
 * note:  Failing calls (NULL format, buffer too small, count >= size) with a returning handler must not leave sticky state:
 *        later valid calls return correct lengths, do not call the handler, and do not clear errno.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#if TC_CRT_VER < 140
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
#define sprintf_s tc_p_sprintf_s
#endif
TC_DECL_PROC(int, _vsnprintf_s, (char *, size_t, size_t, const char *, va_list));

static int w_snprintf_s(char *b, size_t n, size_t c, const char *f, ...)
{
    va_list ap; int r; va_start(ap, f);
#if TC_CRT_VER >= 140
    r = _vsnprintf_s(b, n, c, f, ap);
#else
    r = tc_p__vsnprintf_s(b, n, c, f, ap);
#endif
    va_end(ap); return r;
}
static const char * volatile v_null = NULL;
static const char * volatile v_abc = "abc";

TC_CASE(alternate)
{
    char b[16]; int r;
    tc_iph_count = 0;
    errno = 0; r = sprintf_s(b, sizeof b, v_null);          CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    r = sprintf_s(b, sizeof b, "%s", v_abc);                CHECK_EQ_I(r, 3); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 1);
    r = sprintf_s(b, 3, "%s", v_abc);                       CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 2);
    r = _snprintf(b, sizeof b, "%s-%d", v_abc, 12);         CHECK_EQ_I(r, 6); CHECK_STR(b, "abc-12"); CHECK_EQ_I(tc_iph_count, 2);
    r = w_snprintf_s(b, 4, 8, "%s", "abcdef");              CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 3);
    r = w_snprintf_s(b, sizeof b, 8, "%s", v_abc);          CHECK_EQ_I(r, 3); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 3);
    r = _scprintf(v_null);                                  CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 4);
    r = _scprintf("%s%d", v_abc, 100);                      CHECK_EQ_I(r, 6); CHECK_EQ_I(tc_iph_count, 4);
    r = sprintf_s(b, sizeof b, "%d", 12345);                CHECK_EQ_I(r, 5); CHECK_STR(b, "12345"); CHECK_EQ_I(tc_iph_count, 4);
}
TC_CASE(errno_kept)
{
    char b[16]; int r;
    tc_iph_count = 0;
    errno = 0; r = sprintf_s(b, 2, "%s", v_abc);            CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE);
    r = sprintf_s(b, sizeof b, "%s", v_abc);                CHECK_EQ_I(r, 3);
    CHECK_EQ_I(errno, ERANGE);
    r = _snprintf(b, sizeof b, "%s", v_abc);                CHECK_EQ_I(r, 3);
    CHECK_EQ_I(errno, ERANGE);
    r = _scprintf("%s", v_abc);                             CHECK_EQ_I(r, 3);
    CHECK_EQ_I(errno, ERANGE);
}
int main(void)
{
    tc_begin();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(sprintf_s);
    TC_LOAD_PROC(_vsnprintf_s);
    if (!tc_p_sprintf_s || !tc_p__vsnprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 2; }
#endif
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(alternate); TC_RUN(errno_kept);
    return tc_end();
}
