/* TC
 * id:    pfv_neg_probe_trailing_percent
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.1 (UB); MS docs parameter validation
 * note:  OBSERVED Win10 22H2 19045 ucrtbase (run twice): sprintf(b,"abc%") does not invoke the handler: returns 3 with "abc" (the lone trailing % is dropped). Old guess (-1 + handler) was wrong.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(trailing_pct)
{
    char b[64]; volatile const char *f = "abc%"; int n; tc_iph_count = 0;
    n = sprintf(b, (const char *)f);
    CHECK_EQ_I(n, 3); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(trailing_pct); return tc_end(); }
