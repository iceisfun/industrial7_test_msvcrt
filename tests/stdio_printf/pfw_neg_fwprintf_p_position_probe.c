/* TC
 * id:    pfw_neg_fwprintf_p_position_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _vfwprintf_p
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf_p positional parameters: positions must start at 1 with no gaps
 * note:  PROBE. A format using only %2$d (position 1 missing) is probed to fail with -1/EINVAL and write no bytes to the stream.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
static int sh_vf(FILE *s, const wchar_t *f, va_list ap) { return __stdio_common_vfwprintf_p(4, s, f, NULL, ap); }
#else
typedef int (__cdecl *vf_t)(FILE *, const wchar_t *, va_list);
static vf_t p_vf;
static int sh_vf(FILE *s, const wchar_t *f, va_list ap) { return p_vf ? p_vf(s, f, ap) : -99; }
#endif
static int fwp(FILE *s, const wchar_t *f, ...) { int r; va_list ap; va_start(ap, f); r = sh_vf(s, f, ap); va_end(ap); return r; }
static const wchar_t * volatile gfmt = L"%2$d";
static const char tmp[] = "tc_pfw_neg_fwprintf_p_position_probe.tmp";

TC_CASE(missing_position_one)
{
    FILE *fp = fopen(tmp, "w"); int r; long n = -1; FILE *g;
    CHECK(fp != NULL); if (!fp) return;
    tc_iph_count = 0; errno = 0; r = fwp(fp, gfmt, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL);
    fclose(fp);
    g = fopen(tmp, "rb"); if (g) { fseek(g, 0, SEEK_END); n = ftell(g); fclose(g); }
    CHECK_EQ_I(n, 0);
    remove(tmp);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
#if TC_CRT_VER < 140
    p_vf = (vf_t)tc_crt_proc("_vfwprintf_p"); CHECK(p_vf != NULL);
#endif
    TC_RUN(missing_position_one); return tc_end();
}
