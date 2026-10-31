/* TC
 * id:    ct_neg_isblank_not_exported_win7
 * area:  ctype
 * funcs:
 * procs: isblank iswblank _isblank_l _iswblank_l
 * crt:   msvcr90 msvcr80
 * tier:  p2
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs isblank, iswblank (C99 additions, VS2013+)
 * note:  VS2005/VS2008 CRTs predate C99 isblank; absence is inferred (docs do not state non-export). msvcr100 is in the probe test.
 */
#include "tc.h"

TC_CASE(isblank_null) { CHECK(tc_crt_proc("isblank") == NULL); }
TC_CASE(iswblank_null) { CHECK(tc_crt_proc("iswblank") == NULL); }
TC_CASE(_isblank_l_null) { CHECK(tc_crt_proc("_isblank_l") == NULL); }
TC_CASE(_iswblank_l_null) { CHECK(tc_crt_proc("_iswblank_l") == NULL); }

int main(void) { tc_begin(); TC_RUN(isblank_null); TC_RUN(iswblank_null); TC_RUN(_isblank_l_null); TC_RUN(_iswblank_l_null); return tc_end(); }
