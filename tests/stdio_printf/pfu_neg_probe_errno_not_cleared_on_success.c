/* TC
 * id:    pfu_neg_probe_errno_not_cleared_on_success
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: sprintf_s _snprintf_s fprintf_s fopen fclose
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.5p3 (no library function sets errno to zero); MS docs sprintf_s
 * note:  PROBE: successful secure printf calls leave errno untouched (preset 123 stays 123), and a _snprintf_s truncation
 *        (_TRUNCATE) followed by a successful call keeps the errno of the earlier failure (the errno value left by the truncation is not
 *        assumed; only that the later success does not change it).
 */
#include "tc.h"
#include <errno.h>

#define TMP "tc_pfu_errno.tmp"
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
TC_DECL_PROC(int, _snprintf_s, (char *, size_t, size_t, const char *, ...));
TC_DECL_PROC(int, fprintf_s, (void *, const char *, ...));
TC_DECL_PROC(void *, fopen, (const char *, const char *));
TC_DECL_PROC(int, fclose, (void *));

TC_CASE(sprintf_s_success_keeps_errno)
{
    char b[16]; int n;
    if (!tc_p_sprintf_s) TC_SKIP("sprintf_s not exported");
    errno = 123;
    n = tc_p_sprintf_s(b, sizeof b, "%d", 42);
    CHECK_EQ_I(n, 2); CHECK_STR(b, "42"); CHECK_EQ_I(errno, 123);
}
TC_CASE(snprintf_s_truncate_then_success)
{
    char b[4]; char c[16]; int n, e;
    if (!tc_p__snprintf_s) TC_SKIP("_snprintf_s not exported");
    tc_iph_install();
    errno = 0;
    n = tc_p__snprintf_s(b, sizeof b, (size_t)-1, "abcdefgh");
    CHECK_EQ_I(n, -1); e = errno;
    n = tc_p__snprintf_s(c, sizeof c, (size_t)-1, "ok");
    CHECK_EQ_I(n, 2); CHECK_EQ_I(errno, e);
    errno = 123;
    n = tc_p__snprintf_s(c, sizeof c, (size_t)-1, "ok");
    CHECK_EQ_I(n, 2); CHECK_EQ_I(errno, 123);
}
TC_CASE(fprintf_s_success_keeps_errno)
{
    void *f; int n;
    if (!tc_p_fprintf_s || !tc_p_fopen || !tc_p_fclose) TC_SKIP("missing proc");
    f = tc_p_fopen(TMP, "w"); CHECK(f != NULL); if (!f) return;
    errno = 123;
    n = tc_p_fprintf_s(f, "x%d", 1);
    CHECK_EQ_I(n, 2); CHECK_EQ_I(errno, 123);
    tc_p_fclose(f); DeleteFileA(TMP);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(sprintf_s); TC_LOAD_PROC(_snprintf_s); TC_LOAD_PROC(fprintf_s); TC_LOAD_PROC(fopen); TC_LOAD_PROC(fclose);
    TC_RUN(sprintf_s_success_keeps_errno); TC_RUN(snprintf_s_truncate_then_success); TC_RUN(fprintf_s_success_keeps_errno);
    return tc_end();
}
