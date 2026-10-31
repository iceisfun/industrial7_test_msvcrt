/* TC
 * id:    pfo_neg_vsprintf_huge_precision_overflow
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf __stdio_common_vsprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vsprintf_s, vsnprintf (count limits writes; legacy returns -1 on overflow; C99 mode returns required length); C11 7.21.6.5
 * note:  The raw workers may return any negative value on overflow (the SDK inline wrappers map it to -1), so only r < 0 is asserted.
 *        "%.1000f" of 1.0 is "1." plus 1000 zeros = 1002 chars. No write beyond count is allowed in any mode (guard bytes).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

int __cdecl __stdio_common_vsprintf_s(unsigned __int64, char *, size_t, const char *, _locale_t, va_list);
#define G 0x5A

static int cv(unsigned __int64 o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static int cs(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_s(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int guard_ok(const char *b, int from, int to)
{ int i; for (i = from; i < to; i++) if ((unsigned char)b[i] != G) return 0; return 1; }
static const char * volatile fmt = "%.1000f";
static volatile double one = 1.0;

TC_CASE(opts0_no_overflow)
{
    char b[64]; int r; memset(b, G, sizeof b);
    r = cv(0, b, 32, fmt, one);
    CHECK(r < 0); CHECK(guard_ok(b, 32, 64));
}
TC_CASE(snprintf_mode)
{
    char b[64]; int r; memset(b, G, sizeof b);
    r = cv(2, b, 32, fmt, one);
    CHECK_EQ_I(r, 1002); CHECK(guard_ok(b, 32, 64)); CHECK_EQ_I(b[31], 0);
    CHECK_MEM(b, "1.000000000000000000000000000000", 31);
}
TC_CASE(size_query)
{
    int r = cv(2, NULL, 0, fmt, one);
    CHECK_EQ_I(r, 1002);
}
TC_CASE(secure_variant)
{
    char b[64]; int r; memset(b, G, sizeof b);
    tc_iph_count = 0;
    r = cs(b, 32, fmt, one);
    CHECK(r < 0); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 1); CHECK(guard_ok(b, 32, 64));
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(opts0_no_overflow); TC_RUN(snprintf_mode); TC_RUN(size_query); TC_RUN(secure_variant);
    return tc_end();
}
