/* TC
 * id:    pfw_probe_snwprintf_s_count_lt_len
 * area:  stdio_printf
 * funcs:
 * procs: _snwprintf_s
 * crt:   >=80
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _snprintf_s, _snwprintf_s
 * note:  PROBE: with count < len but size large enough, output is cut to count chars, NUL-terminated, returns -1 with no handler call (docs: "If count is less than len, the string is truncated... returns -1"). Believed true; isolated.
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

TC_CASE(count_lt_len)
{
    wchar_t b[16]; int n, i; if (!p_sn) TC_SKIP("not exported");
    for (i = 0; i < 16; i++) b[i] = L'Z';
    tc_iph_count = 0;
    n = p_sn(b, 16, 3, L"%ls", L"abcdefgh");
    CHECK_EQ_I(n, -1); CHECK_WSTR(b, L"abc"); CHECK_EQ_I(tc_iph_count, 0);
}

int main(void)
{
    tc_begin(); tc_iph_install();
#if TC_CRT_VER >= 140
    p_sn = shim_sn;
#else
    p_sn = (sn_t)tc_crt_proc("_snwprintf_s");
#endif
    TC_RUN(count_lt_len);
    return tc_end();
}
