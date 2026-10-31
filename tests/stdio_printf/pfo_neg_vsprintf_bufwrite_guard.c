/* TC
 * id:    pfo_neg_vsprintf_bufwrite_guard
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf_s __stdio_common_vsprintf __stdio_common_vswprintf
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs snprintf/_snprintf/sprintf_s/swprintf (output never exceeds the stated buffer count)
 * note:  __stdio_common_* options values are an implementation detail (0 = legacy, 0x2 = C99 snprintf); r<0 for options 0 is inferred, the guard-byte checks are the core claim.
 *        Output must NOT write beyond the buffer count. Guard bytes live inside a bigger array after the declared count.
 *        Contents of the in-range bytes after an overflow (e.g. debug 0xFE fill of _s buffers) are deliberately NOT asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

#define G 0x5A
static int calls(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static int calls_s(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_s(o, b, n, f, NULL, ap); va_end(ap); return r; }
static int callw(unsigned long long o, wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static void fill(char *b) { int i; for (i = 0; i < 64; i++) b[i] = G; }
static int guard_ok(const char *b, int from) { int i; for (i = from; i < 64; i++) if ((unsigned char)b[i] != G) return 0; return 1; }

TC_CASE(opts2_snprintf)
{
    char b[64]; volatile size_t n = 5; int r; fill(b);
    r = calls(0x2, b, n, "%s", "0123456789abcdefghijklmnopqrstuvwxyz");
    CHECK_EQ_I(r, 36);               /* C99 snprintf: would-be length */
    CHECK(guard_ok(b, 5));
}
TC_CASE(opts0_legacy)
{
    char b[64]; volatile size_t n = 5; int r; fill(b);
    r = calls(0, b, n, "%s", "0123456789abcdefghijklmnopqrstuvwxyz");
    CHECK(r < 0);
    CHECK(guard_ok(b, 5));
}
TC_CASE(vsprintf_s_overflow)
{
    char b[64]; volatile size_t n = 5; int r; fill(b); tc_iph_count = 0;
    r = calls_s(0, b, n, "%s", "0123456789abcdefghijklmnopqrstuvwxyz");
    CHECK(r < 0);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK(guard_ok(b, 5));
}
TC_CASE(wide_guard)
{
    wchar_t b[64]; volatile size_t n = 5; int i, r, ok = 1;
    for (i = 0; i < 64; i++) b[i] = 0x5A5A;
    r = callw(0, b, n, L"%ls", L"0123456789abcdefghijklmnopqrstuvwxyz");
    CHECK(r < 0);
    for (i = 5; i < 64; i++) if (b[i] != 0x5A5A) ok = 0;
    CHECK(ok);
    for (i = 0; i < 64; i++) b[i] = 0x5A5A;
    r = callw(0x2, b, n, L"%ls", L"0123456789abcdefghijklmnopqrstuvwxyz");
    CHECK_EQ_I(r, 36);
    ok = 1; for (i = 5; i < 64; i++) if (b[i] != 0x5A5A) ok = 0;
    CHECK(ok);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(opts2_snprintf); TC_RUN(opts0_legacy); TC_RUN(vsprintf_s_overflow); TC_RUN(wide_guard);
    return tc_end();
}
