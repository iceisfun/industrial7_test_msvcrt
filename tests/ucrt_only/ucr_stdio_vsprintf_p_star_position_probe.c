/* TC
 * id:    ucr_stdio_vsprintf_p_star_position_probe
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsprintf_p
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf_p positional parameters
 * note:  PROBE: whether width may be supplied positionally as '*2$' in the _p variants. Expected (most likely): value arg 1 with width arg 2.
 */
#include "tc.h"
#include <stdarg.h>

typedef int (__cdecl *vsp_t)(unsigned long long, char *, size_t, const char *, void *, va_list);
static vsp_t p_vsp;
static int call(char *b, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsp(0, b, 64, f, NULL, ap); va_end(ap); return r; }

TC_CASE(star_n)
{
    char b[64]; int r;
    if (!p_vsp) TC_SKIP("not exported");
    r = call(b, "[%1$*2$d]", 42, 6);
    CHECK_STR(b, "[    42]"); CHECK_EQ_I(r, 8);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vsp = (vsp_t)tc_crt_proc("__stdio_common_vsprintf_p");
    TC_RUN(star_n);
    return tc_end();
}
