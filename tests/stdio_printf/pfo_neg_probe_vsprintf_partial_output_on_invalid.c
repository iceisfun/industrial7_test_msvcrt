/* TC
 * id:    pfo_neg_probe_vsprintf_partial_output_on_invalid
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf __stdio_common_vsprintf_s __stdio_common_vsnprintf_s
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf type field characters (unknown conversion -> invalid parameter handler)
 * note:  PROBES: after the invalid conversion in "abc%y" the call returns a negative value, sets EINVAL, and bytes at index >= 4 of
 *        a 16 byte 'Z'-prefilled buffer are never touched. The contents of buf[0..3] are deliberately not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

int __cdecl __stdio_common_vsprintf_s(unsigned __int64, char *, size_t, const char *, _locale_t, va_list);
int __cdecl __stdio_common_vsnprintf_s(unsigned __int64, char *, size_t, size_t, const char *, _locale_t, va_list);
#define G 0x5A

static int cv(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int cs(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_s(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int cn(char *b, size_t n, size_t c, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsnprintf_s(0, b, n, c, f, NULL, ap); va_end(ap); return r; }
static int tail_ok(const char *b) { int i; for (i = 4; i < 16; i++) if ((unsigned char)b[i] != G) return 0; return 1; }
static const char * volatile fmt = "abc%y";

TC_CASE(s_variant) { char b[16]; int r; memset(b, G, 16); tc_iph_count = 0; errno = 0; r = cs(b, 16, fmt); CHECK(r < 0); CHECK_EQ_I(errno, EINVAL); CHECK(tail_ok(b)); }
TC_CASE(plain) { char b[16]; int r; memset(b, G, 16); errno = 0; r = cv(b, 16, fmt); CHECK(r < 0); CHECK_EQ_I(errno, EINVAL); CHECK(tail_ok(b)); }
TC_CASE(snprintf_s_truncate) { char b[16]; int r; memset(b, G, 16); errno = 0; r = cn(b, 16, (size_t)-1, fmt); CHECK(r < 0); CHECK_EQ_I(errno, EINVAL); CHECK(tail_ok(b)); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(s_variant); TC_RUN(plain); TC_RUN(snprintf_s_truncate);
    return tc_end();
}
