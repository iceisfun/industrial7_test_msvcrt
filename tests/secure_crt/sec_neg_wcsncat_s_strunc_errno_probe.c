/* TC
 * id:    sec_neg_wcsncat_s_strunc_errno_probe
 * area:  secure_crt
 * funcs:
 * procs: wcsncat_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wcsncat_s (STRUNCATE); errno behaviour on truncation is not documented
 * note:  PROBE: errno remains 0 (untouched) after wcsncat_s returns STRUNCATE with _TRUNCATE; handler not invoked.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(errno_t, wcsncat_s, (wchar_t*,size_t,const wchar_t*,size_t));
#ifndef STRUNCATE
#define STRUNCATE 80
#endif
#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif
TC_CASE(strunc_errno_untouched) { wchar_t d[4] = L"ab"; int r; tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = tc_p_wcsncat_s(d, 4, L"cdef", _TRUNCATE); CHECK_EQ_I(r, 80); CHECK_WSTR(d, L"abc"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }
int main(void) { tc_begin(); TC_LOAD_PROC(wcsncat_s); TC_RUN(strunc_errno_untouched); return tc_end(); }
