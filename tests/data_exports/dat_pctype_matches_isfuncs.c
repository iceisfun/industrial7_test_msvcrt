/* TC
 * id:    dat_pctype_matches_isfuncs
 * area:  data_exports
 * funcs: isalpha isdigit isspace isupper
 * procs: __pctype_func
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   ctype.h flag values; MS docs isalpha/isdigit/isspace/isupper (C locale, arguments -1..255)
 */
#include "tc.h"
#include <ctype.h>

typedef const unsigned short *(__cdecl *tabfn_t)(void);
static tabfn_t p_pctype_func;

static int bad(int (*f)(int), unsigned short mask)
{
    const unsigned short *t = p_pctype_func(); int c, n = 0;
    for (c = 0; c < 256; c++) if ((f(c) != 0) != ((t[c] & mask) != 0)) n++;
    if ((f(-1) != 0) != ((t[-1] & mask) != 0)) n++;
    return n;
}
TC_CASE(alpha_agrees_all_bytes) { CHECK_EQ_I(bad(isalpha, 0x103), 0); }
TC_CASE(digit_agrees)           { CHECK_EQ_I(bad(isdigit, 0x4), 0); }
TC_CASE(space_agrees)           { CHECK_EQ_I(bad(isspace, 0x8), 0); }
TC_CASE(upper_agrees)           { CHECK_EQ_I(bad(isupper, 0x1), 0); }

int main(void) { tc_begin(); p_pctype_func = (tabfn_t)tc_crt_proc("__pctype_func"); if (!p_pctype_func) { tc_write("TC FAIL dat_pctype_matches_isfuncs::__pctype_func not exported\n"); return 1; } TC_RUN(alpha_agrees_all_bytes); TC_RUN(digit_agrees); TC_RUN(space_agrees); TC_RUN(upper_agrees); return tc_end(); }
