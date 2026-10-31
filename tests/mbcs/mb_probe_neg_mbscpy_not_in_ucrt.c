/* TC
 * id:    mb_probe_neg_mbscpy_not_in_ucrt
 * area:  mbcs
 * funcs: 
 * procs: _mbscpy _mbscat _mbsnbcpy
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mbscpy (not in UCRT); UCRT export table
 * note:  PROBE: ucrtbase.dll does not export _mbscpy/_mbscat (only the _s variants) but does export _mbsnbcpy.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>

TC_CASE(cpy_absent) { CHECK(tc_crt_proc("_mbscpy") == NULL); }
TC_CASE(cat_absent) { CHECK(tc_crt_proc("_mbscat") == NULL); }
TC_CASE(nbcpy_present) { CHECK(tc_crt_proc("_mbsnbcpy") != NULL); }

int main(void) { tc_begin(); TC_RUN(cpy_absent); TC_RUN(cat_absent); TC_RUN(nbcpy_present); return tc_end(); }
