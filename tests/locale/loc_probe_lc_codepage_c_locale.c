/* TC
 * id:    loc_probe_lc_codepage_c_locale
 * area:  locale
 * funcs: ___lc_codepage_func ___lc_collate_cp_func
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   none (internal helpers)
 * note:  PROBE: in the startup C locale the LC_CTYPE and LC_COLLATE code page values are 0 (the C locale has no code page).
 */
#include "tc.h"
#include <locale.h>

int __cdecl ___lc_collate_cp_func(void);

TC_CASE(codepage_zero) { CHECK_EQ_U(___lc_codepage_func(), 0); }
TC_CASE(collate_cp_zero) { CHECK_EQ_I(___lc_collate_cp_func(), 0); }
int main(void) { tc_begin(); TC_RUN(codepage_zero); TC_RUN(collate_cp_zero); return tc_end(); }
