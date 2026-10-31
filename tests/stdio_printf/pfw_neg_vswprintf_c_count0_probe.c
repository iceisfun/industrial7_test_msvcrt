/* TC
 * id:    pfw_neg_vswprintf_c_count0_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _vswprintf_c
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _swprintf_c: count is the maximum number of characters (including terminator) to store
 * note:  PROBE. count==0 with a non-NULL buffer: probes that the call returns a negative value and writes nothing.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
static int sh_vc(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return __stdio_common_vswprintf(4, b, n, f, NULL, ap); }
#else
typedef int (__cdecl *vc_t)(wchar_t *, size_t, const wchar_t *, va_list);
static vc_t p_vc;
static int sh_vc(wchar_t *b, size_t n, const wchar_t *f, va_list ap) { return p_vc ? p_vc(b, n, f, ap) : -99; }
#endif
static int vswc(wchar_t *b, size_t n, const wchar_t *f, ...) { int r; va_list ap; va_start(ap, f); r = sh_vc(b, n, f, ap); va_end(ap); return r; }

TC_CASE(count_zero) { wchar_t a[6] = { L'Q', L'Q', L'Q', L'Q', L'Q', 0 }; wchar_t *b = a + 1; int r; r = vswc(b, 0, L"a"); CHECK(r < 0); CHECK_EQ_I(b[0], L'Q'); CHECK_EQ_I(b[1], L'Q'); }  /* padded: an implementation that writes b[count-1] must not hit memory outside the array */
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
#if TC_CRT_VER < 140
    p_vc = (vc_t)tc_crt_proc("_vswprintf_c"); CHECK(p_vc != NULL);
#endif
    TC_RUN(count_zero); return tc_end();
}
