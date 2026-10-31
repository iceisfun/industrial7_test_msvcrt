/* TC
 * id:    pfo_neg_vfprintf_s_n_specifier
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vfprintf_s __stdio_common_vfprintf __stdio_common_vfwprintf_s fopen fclose
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf type field characters (%n: disabled by default, invalid parameter handler); _set_printf_count_output; fprintf_s
 * note:  The target int must remain at its sentinel value; the return value is negative with the handler invoked once.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

int __cdecl __stdio_common_vfprintf_s(unsigned __int64, FILE *, const char *, _locale_t, va_list);
int __cdecl __stdio_common_vfwprintf_s(unsigned __int64, FILE *, const wchar_t *, _locale_t, va_list);
#define FN "tc_pfo_neg_vfprintf_s_n_specifier_" TC_CRT_DLL ".tmp"
#define SENT 0x5A5A5A5A

static int cs(FILE *f, const char *fm, ...)
{ va_list ap; int r; va_start(ap, fm); r = __stdio_common_vfprintf_s(0, f, fm, NULL, ap); va_end(ap); return r; }
static int cn(FILE *f, const char *fm, ...)
{ va_list ap; int r; va_start(ap, fm); r = __stdio_common_vfprintf(0, f, fm, NULL, ap); va_end(ap); return r; }
static int cw(FILE *f, const wchar_t *fm, ...)
{ va_list ap; int r; va_start(ap, fm); r = __stdio_common_vfwprintf_s(0, f, fm, NULL, ap); va_end(ap); return r; }

TC_CASE(narrow_s)
{
    FILE *f = fopen(FN, "w"); volatile int n = SENT; int r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = cs(f, "ab%n", (int *)&n);
    CHECK(r < 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, SENT);
    fclose(f); remove(FN);
}
TC_CASE(wide_s)
{
    FILE *f = fopen(FN, "w"); volatile int n = SENT; int r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = cw(f, L"ab%n", (int *)&n);
    CHECK(r < 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, SENT);
    fclose(f); remove(FN);
}
TC_CASE(plain_default_disabled)
{
    FILE *f = fopen(FN, "w"); volatile int n = SENT; int r; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; r = cn(f, "ab%n", (int *)&n);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, SENT); CHECK(r < 0);
    fclose(f); remove(FN);
}
int main(void)
{
    tc_begin(); remove(FN);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(narrow_s); TC_RUN(wide_s); TC_RUN(plain_default_disabled);
    return tc_end();
}
