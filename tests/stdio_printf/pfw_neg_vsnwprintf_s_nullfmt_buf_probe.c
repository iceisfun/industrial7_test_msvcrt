/* TC
 * id:    pfw_neg_vsnwprintf_s_nullfmt_buf_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _vsnwprintf_s
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _vsnwprintf_s (buffer state after failure)
 * note:  PROBE. With a NULL format and a valid buffer, probes that the secure formatter writes an empty string (buf[0]==0)
 *        before returning -1. The -1/EINVAL part is asserted in pfw_neg_vsnwprintf_s_null_args.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
extern int __cdecl __stdio_common_vsnwprintf_s(unsigned __int64, wchar_t *, size_t, size_t, const wchar_t *, _locale_t, va_list);
static int sh_vsnws(wchar_t *b, size_t sz, size_t cnt, const wchar_t *f, va_list ap) { return __stdio_common_vsnwprintf_s(4, b, sz, cnt, f, NULL, ap); }
#else
typedef int (__cdecl *vsnws_t)(wchar_t *, size_t, size_t, const wchar_t *, va_list);
static vsnws_t p_vsnwprintf_s;
static int sh_vsnws(wchar_t *b, size_t sz, size_t cnt, const wchar_t *f, va_list ap) { return p_vsnwprintf_s ? p_vsnwprintf_s(b, sz, cnt, f, ap) : -99; }
#endif
static int vsnws(wchar_t *b, size_t sz, size_t cnt, const wchar_t *f, ...)
{ int r; va_list ap; va_start(ap, f); r = sh_vsnws(b, sz, cnt, f, ap); va_end(ap); return r; }
static const wchar_t * volatile nfmt;

TC_CASE(buf0_cleared) { wchar_t b[8] = { L'Q', L'Q', 0 }; int r; r = vsnws(b, 8, 8, nfmt); CHECK_EQ_I(r, -1); CHECK_EQ_I(b[0], 0); }
TC_CASE(buf0_cleared_truncate) { wchar_t b[8] = { L'Q', L'Q', 0 }; int r; r = vsnws(b, 8, _TRUNCATE, nfmt); CHECK_EQ_I(r, -1); CHECK_EQ_I(b[0], 0); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
#if TC_CRT_VER < 140
    p_vsnwprintf_s = (vsnws_t)tc_crt_proc("_vsnwprintf_s");
    CHECK(p_vsnwprintf_s != NULL);
#endif
    TC_RUN(buf0_cleared); TC_RUN(buf0_cleared_truncate); return tc_end();
}
