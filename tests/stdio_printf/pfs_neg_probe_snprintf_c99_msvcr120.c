/* TC
 * id:    pfs_neg_probe_snprintf_c99_msvcr120
 * area:  stdio_printf
 * funcs:
 * procs: snprintf

 * crt:   msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * note:  PROBE: whether msvcr120.dll exports C99-conforming snprintf (VS2013 CRT); if not exported the 'exported' case fails and others skip.
 * ref:   C11 7.21.6.5 snprintf: returns the length that would have been written; output truncated and NUL-terminated

 */
#include "tc.h"
#include <stdio.h>
#include <stddef.h>

TC_DECL_PROC(int, snprintf, (char *, size_t, const char *, ...));
#define snprintf tc_p_snprintf

static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 'Z'; }
static const char *volatile s_abcdef = "abcdef";
static const char *volatile f_s = "%s";
static const char *volatile f_ls = "%ls";
static const wchar_t *volatile w_abcdef = L"abcdef";
TC_CASE(exported) { CHECK(tc_p_snprintf != NULL); }
TC_CASE(trunc4)
{
    char b[16]; int n; fill(b); if (!tc_p_snprintf) TC_SKIP("no snprintf");
    n = snprintf(b, 4, f_s, s_abcdef);
    CHECK_EQ_I(n, 6);
    CHECK_STR(b, "abc");
    CHECK(b[4] == 'Z');
}
TC_CASE(size1)
{
    char b[16]; int n; fill(b); if (!tc_p_snprintf) TC_SKIP("no snprintf");
    n = snprintf(b, 1, f_s, "abc");
    CHECK_EQ_I(n, 3);
    CHECK(b[0] == 0);
    CHECK(b[1] == 'Z');
}
TC_CASE(size0)
{
    char b[16]; int n; fill(b); if (!tc_p_snprintf) TC_SKIP("no snprintf");
    n = snprintf(b, 0, f_s, "abc");
    CHECK_EQ_I(n, 3);
    CHECK(b[0] == 'Z');
}
TC_CASE(wide_source)
{
    char b[16]; int n; fill(b); if (!tc_p_snprintf) TC_SKIP("no snprintf");
    n = snprintf(b, 4, f_ls, w_abcdef);
    CHECK_EQ_I(n, 6);
    CHECK_STR(b, "abc");
    fill(b);
    n = snprintf(b, 0, f_ls, w_abcdef);
    CHECK_EQ_I(n, 6);
    CHECK(b[0] == 'Z');
}
int main(void) { tc_begin(); TC_LOAD_PROC(snprintf); TC_RUN(exported); TC_RUN(trunc4); TC_RUN(size1); TC_RUN(size0); TC_RUN(wide_source); return tc_end(); }
