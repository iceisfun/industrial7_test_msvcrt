/* TC
 * id:    dat_pwctype_eof_probe
 * area:  data_exports
 * funcs:
 * procs: __pwctype_func
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs iswalpha (WEOF is a valid argument)
 * note:  Probe: the wide ctype table is readable at index -1 and holds 0 (entry for EOF).
 */
#include "tc.h"
#include <ctype.h>

typedef const unsigned short *(__cdecl *tabfn_t)(void);

TC_CASE(pwctype_eof_zero) { tabfn_t f = (tabfn_t)tc_crt_proc("__pwctype_func"); const unsigned short *t; if (!f) TC_SKIP("not exported"); t = f(); volatile int c = -1; CHECK_EQ_U(t[c], 0); }
int main(void) { tc_begin(); TC_RUN(pwctype_eof_zero); return tc_end(); }
