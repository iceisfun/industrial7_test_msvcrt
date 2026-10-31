/* TC
 * id:    pfw_swprintf_p_star_probe
 * area:  stdio_printf
 * funcs: 
 * procs: _swprintf_p
 * crt:   >=80
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf_p Positional Parameters
 * note:  PROBE: whether width given as positional star (%1$*2$d: value is arg 1, width is arg 2) is accepted and honoured by _swprintf_p. Believed supported per docs examples but not certain; assertions kept isolated. msvcrt.dll excluded.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

typedef int (__cdecl *sp_t)(wchar_t *, size_t, const wchar_t *, ...);
static sp_t p_sp;
#if TC_CRT_VER >= 140
static int __cdecl shim_sp(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf_p(_CRT_INTERNAL_LOCAL_PRINTF_OPTIONS, b, n, f, NULL, ap); va_end(ap); return r; }
#endif
static void fill(wchar_t *b, int n) { int i; for (i = 0; i < n; i++) b[i] = L'Z'; }

TC_CASE(star_width_positional)
{
    wchar_t b[32]; int n; if (!p_sp) TC_SKIP("not exported");
    fill(b, 32); n = p_sp(b, 32, L"[%1$*2$d]", 42, 6);
    CHECK_WSTR(b, L"[    42]"); CHECK_EQ_I(n, 8);
}

int main(void)
{
    tc_begin(); tc_iph_install(); /* unsupported syntax must not abort the process on ucrt */
#if TC_CRT_VER >= 140
    p_sp = shim_sp;
#else
    p_sp = (sp_t)tc_crt_proc("_swprintf_p");
#endif
    TC_RUN(star_width_positional);
    return tc_end();
}
