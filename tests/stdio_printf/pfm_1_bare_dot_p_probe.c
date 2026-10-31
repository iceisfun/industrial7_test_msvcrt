/* TC
 * id:    pfm_1_bare_dot_p_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.6.1 (precision with p is undefined by C); MS docs printf type field p
 * note:  PROBE: the CRT ignores precision for %p (prints the fixed-width uppercase pointer form), so %.p, %.0p equal plain %p.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(prec_ignored) {
    char a[64], b[64]; void * volatile p = (void *)(uintptr_t)0x1234; void * volatile z = NULL;
    sprintf(a, "%p", p); sprintf(b, "%.p", p); CHECK_STR(a, b);
    sprintf(b, "%.0p", p); CHECK_STR(a, b);
    sprintf(a, "%p", z); sprintf(b, "%.0p", z); CHECK_STR(a, b);
    sprintf(b, "%.p", z); CHECK_STR(a, b);
}
int main(void) { tc_begin(); TC_RUN(prec_ignored); return tc_end(); }
