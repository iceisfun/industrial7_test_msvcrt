/* TC
 * id:    ucr_neg_stdio_vswprintf_mbs_eilseq_probe
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vswprintf
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf type field (%hs in wide functions), mbrtowc
 * note:  PROBES: wide vswprintf with %hs given the single byte 0xE9 in the "C" locale. In the C locale bytes 0x80-0xFF
 *        either map straight through (success) or fail with EILSEQ. Only the invariant "ret == -1 implies errno == EILSEQ"
 *        and "ret >= 0 implies the output is NUL terminated" is asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

typedef int (__cdecl *vws_t)(unsigned long long, wchar_t *, size_t, const wchar_t *, void *, va_list);
static vws_t p_vws;
static int callw(wchar_t *b, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vws(0, b, 32, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(hs_highbyte)
{
    static const char s[] = { (char)0xE9, 0 };
    wchar_t b[32]; int r;
    if (!p_vws) TC_SKIP("not exported");
    b[0] = 0x7777; errno = 0; tc_iph_count = 0;
    r = callw(b, L"%hs", (const char *)s);
    if (r == -1) CHECK_EQ_I(errno, EILSEQ);
    else { CHECK(r >= 0); CHECK_EQ_I(r, 1); CHECK_EQ_I(b[1], 0); }
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vws = (vws_t)tc_crt_proc("__stdio_common_vswprintf");
    TC_RUN(hs_highbyte);
    return tc_end();
}
