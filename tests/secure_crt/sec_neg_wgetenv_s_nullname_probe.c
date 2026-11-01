/* TC
 * id:    sec_neg_wgetenv_s_nullname_probe
 * area:  secure_crt
 * funcs: _wgetenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs getenv_s, _wgetenv_s
 * note:  [observed on ucrtbase 19045 only; the same expectation on the legacy CRTs listed in crt: is [unverified]] OBSERVED on Win10 22H2 (19045): _wgetenv_s(&n,buf,8,NULL) returns 0, *retval 0 and does NOT call the invalid-parameter
 *        handler (NULL name == variable not found). The earlier EINVAL+handler expectation was wrong. Stable.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>
#include <errno.h>
static volatile size_t v_sz = 8;
TC_CASE(null_name_not_found) { wchar_t b[8]; size_t n = 5; tc_iph_install(); tc_iph_count = 0;
    CHECK_EQ_I(_wgetenv_s(&n, b, v_sz, NULL), 0); CHECK_EQ_I(n, 0); CHECK_EQ_I(tc_iph_count, 0); }   /* observed */
int main(void) { tc_begin(); TC_RUN(null_name_not_found); return tc_end(); }
