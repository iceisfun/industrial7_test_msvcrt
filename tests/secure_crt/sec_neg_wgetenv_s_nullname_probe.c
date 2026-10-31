/* TC
 * id:    sec_neg_wgetenv_s_nullname_probe
 * area:  secure_crt
 * funcs: _wgetenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs getenv_s, _wgetenv_s
 * note:  PROBE: NULL variable name believed to return EINVAL and invoke the invalid-parameter handler (docs table does not list it explicitly)
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>
#include <errno.h>
static volatile size_t v_sz = 8;
TC_CASE(null_name_EINVAL) { wchar_t b[8]; size_t n = 5; tc_iph_install(); tc_iph_count = 0;
    CHECK_EQ_I(_wgetenv_s(&n, b, v_sz, NULL), EINVAL); CHECK(tc_iph_count >= 1); }
int main(void) { tc_begin(); TC_RUN(null_name_EINVAL); return tc_end(); }
