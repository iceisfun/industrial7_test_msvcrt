/* TC
 * id:    pfw_neg_vscwprintf_invalid_conv
 * area:  stdio_printf
 * kind:  negative
 * funcs: _scwprintf
 * crt:   >=140
 * tier:  p0
 * conf:  probe
 * ref:   UCRT printf validation: unknown conversion specifier / trailing '%' -> invalid parameter handler, -1, EINVAL
 * note:  PROBE. Probes that the length-only (non-_s) wide formatter rejects an unknown conversion ('%y', '%5.3k') and a trailing '%'
 *        with -1/EINVAL via the invalid parameter handler on ucrtbase. Legacy (msvcr80-120) non-secure tables may instead print the
 *        character literally, so this is restricted to ucrtbase. The _l variant is called through __stdio_common_vswprintf.
 *        Control cases around it are certain.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

static int scwl(const wchar_t *f, _locale_t l, ...) { int r; va_list ap; va_start(ap, l); r = __stdio_common_vswprintf(6, NULL, 0, f, l, ap); va_end(ap); return r; }
static const wchar_t * volatile f_y = L"ab%y";
static const wchar_t * volatile f_pct = L"%";
static const wchar_t * volatile f_k = L"%5.3k";
static const wchar_t * volatile f_ok = L"ab%dcd";

TC_CASE(control) { CHECK_EQ_I(_scwprintf(f_ok, 42), 6); }
TC_CASE(unknown_y) { int r; tc_iph_count = 0; errno = 0; r = _scwprintf(f_y); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(trailing_percent) { int r; tc_iph_count = 0; errno = 0; r = _scwprintf(f_pct); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(unknown_k_with_width) { int r; tc_iph_count = 0; errno = 0; r = _scwprintf(f_k, 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(locale_variant_unknown) { int r; tc_iph_count = 0; errno = 0; r = scwl(f_y, NULL); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(recovers) { tc_iph_count = 0; (void)_scwprintf(f_y); CHECK_EQ_I(_scwprintf(f_ok, 42), 6); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(control); TC_RUN(unknown_y); TC_RUN(trailing_percent); TC_RUN(unknown_k_with_width); TC_RUN(locale_variant_unknown); TC_RUN(recovers);
    return tc_end();
}
