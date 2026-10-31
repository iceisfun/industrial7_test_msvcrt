/* TC
 * id:    pfu_sprintf_s_basic
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s
 * note:  msvcrt.dll: libmingwex would supply a static helper, so sprintf_s is resolved from the DLL there (TC_CRT_VER < 80); ucrtbase has no sprintf_s export (header inline over __stdio_common_vsprintf_s).
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s, _sprintf_s_l: returns number of bytes stored excluding the terminating NUL
 */
#include "tc.h"
#include <stdio.h>

typedef int (__cdecl *sprintf_s_t)(char *, size_t, const char *, ...);
static sprintf_s_t sprintf_s_p;
#if TC_CRT_VER < 80
#define sprintf_s sprintf_s_p
#endif

TC_CASE(retval)
{
    char b[32]; volatile int a = 42; volatile const char *s = "ab"; int n;
    n = sprintf_s(b, sizeof b, "%d-%s", a, (const char *)s);
    CHECK_EQ_I(n, 5); CHECK_STR(b, "42-ab");
}
TC_CASE(empty_format)
{
    char b[8]; int n; volatile const char *f = ""; b[0] = 'x';
    n = sprintf_s(b, sizeof b, (const char *)f);
    CHECK_EQ_I(n, 0); CHECK_EQ_I(b[0], 0);
}
TC_CASE(exact_fit)
{
    char b[6]; volatile const char *s = "abcde"; int n;
    n = sprintf_s(b, 6, "%s", (const char *)s);
    CHECK_EQ_I(n, 5); CHECK_STR(b, "abcde"); CHECK_EQ_I(b[5], 0);
}
TC_CASE(size_one_empty)
{
    char b[1]; volatile const char *f = ""; int n; b[0] = 'x';
    n = sprintf_s(b, 1, (const char *)f);
    CHECK_EQ_I(n, 0); CHECK_EQ_I(b[0], 0);
}
TC_CASE(mixed)
{
    char b[96]; volatile int c = 'Q'; volatile unsigned x = 0xbeef; volatile double d = 3.14159; volatile long long ll = -1234567890123LL; int n;
    n = sprintf_s(b, sizeof b, "%c|%x|%5.2f|%I64d", c, x, d, ll);
    CHECK_STR(b, "Q|beef| 3.14|-1234567890123"); CHECK_EQ_I(n, 27);
}
TC_CASE(long_string)
{
    static char src[4001], b[4096]; int i, n;
    for (i = 0; i < 4000; i++) src[i] = (char)('a' + i % 26);
    n = sprintf_s(b, sizeof b, "%s", src);
    CHECK_EQ_I(n, 4000); CHECK_EQ_I(b[4000], 0); CHECK_MEM(b, src, 4000);
}
int main(void)
{
    tc_begin();
    sprintf_s_p = (sprintf_s_t)tc_crt_proc("sprintf_s");
    if (TC_CRT_VER < 80 && !sprintf_s_p) { tc_write("TC FAIL " TC_TEST_ID "::__sprintf_s not exported\n"); return 2; }
    TC_RUN(retval); TC_RUN(empty_format); TC_RUN(exact_fit); TC_RUN(size_one_empty); TC_RUN(mixed); TC_RUN(long_string);
    return tc_end();
}
