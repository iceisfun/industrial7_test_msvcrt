/* TC
 * id:    mb_neg_mblen_zero_n
 * area:  mbcs
 * funcs: mblen mbtowc
 * kind:  negative
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.22.7.1 says -1 for n==0 with non-empty s; MS implementation returns 0 (mbtowc: if s==NULL or n==0 return 0)
 * note:  PROBE: mblen("a",0) and mbtowc(&wc,"a",0) return 0 on Windows (differs from C11, which would give -1).
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(mblen_zero) { volatile const char *a = "a"; CHECK_EQ_I(mblen((const char *)a, 0), 0); }
TC_CASE(mbtowc_zero) { volatile const char *a = "a"; wchar_t wc = 0; CHECK_EQ_I(mbtowc(&wc, (const char *)a, 0), 0); }

int main(void) { tc_begin(); TC_RUN(mblen_zero); TC_RUN(mbtowc_zero); return tc_end(); }
