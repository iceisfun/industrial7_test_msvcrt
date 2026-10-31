/* TC
 * id:    pfw_neg_probe_snwprintf_s_null_buf
 * area:  stdio_printf
 * funcs:
 * procs: _snwprintf_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _snprintf_s, _snwprintf_s
 * note:  PROBE: NULL buffer with size 4 -> handler called once, returns -1 (generic _s validation; not stated per-function in docs).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

typedef int (__cdecl *sn_t)(wchar_t *, size_t, size_t, const wchar_t *, ...);
static sn_t p_sn;
#if TC_CRT_VER >= 140
static int __cdecl shim_sn(wchar_t *b, size_t sz, size_t c, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnwprintf_s(b, sz, c, f, ap); va_end(ap); return r; }
#endif

static wchar_t * volatile vnull_buf;
TC_CASE(null_buffer)
{
    int n; if (!p_sn) TC_SKIP("not exported");
    tc_iph_count = 0;
    n = p_sn(vnull_buf, 4, 3, L"%d", 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin(); tc_iph_install();
#if TC_CRT_VER >= 140
    p_sn = shim_sn;
#else
    p_sn = (sn_t)tc_crt_proc("_snwprintf_s");
#endif
    TC_RUN(null_buffer);
    return tc_end();
}
