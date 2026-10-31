/* TC
 * id:    pfm_3_width_minus_star_p_pct_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf width, p, %%
 * note:  PROBING: %*p with width -20 left-justifies the pointer text (pointer digits then spaces). %*% consumes the width int and prints a single '%' with no padding (trailing %d proves consumption).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile uintptr_t pv = 0xABCDEF;
TC_CASE(star_p)
{
    char b[128];
    sprintf(b, "[%*p]", -20, (void *)pv);
#ifdef _WIN64
    CHECK_STR(b, "[0000000000ABCDEF    ]");
#else
    CHECK_STR(b, "[00ABCDEF            ]");
#endif
}
TC_CASE(star_pct) { char b[64]; b[0] = 0; tc_iph_install(); sprintf(b, "%*%|%d", -5, 9); CHECK_STR(b, "%|9"); }
int main(void) { tc_begin(); TC_RUN(star_p); TC_RUN(star_pct); return tc_end(); }
