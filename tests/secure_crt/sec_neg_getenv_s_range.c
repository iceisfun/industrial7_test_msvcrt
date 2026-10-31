/* TC
 * id:    sec_neg_getenv_s_range
 * area:  secure_crt
 * funcs: getenv_s _putenv_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs getenv_s (ERANGE, pReturnValue = required size incl NUL)
 * note:  variable TC_SEC_NEG1; buffer content after ERANGE is not asserted; invalid-parameter handler count for ERANGE is not asserted
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <locale.h>
static volatile size_t v_sz;
static void setv(void) { _putenv_s("TC_SEC_NEG1", "hello world"); }
TC_CASE(small_buffer_ERANGE) { char b[4]; size_t n = 0; int r; setv(); tc_iph_install(); v_sz = sizeof b; tc_iph_count = 0;
    r = getenv_s(&n, b, v_sz, "TC_SEC_NEG1"); CHECK_EQ_I(r, ERANGE); }
TC_CASE(retsize_gives_required_size_incl_nul) { char b[4]; size_t n = 0; setv(); tc_iph_install(); v_sz = sizeof b;
    getenv_s(&n, b, v_sz, "TC_SEC_NEG1"); CHECK_EQ_U(n, 12); }
TC_CASE(retry_with_retsize_succeeds) { char b[4], big[12]; size_t n = 0, m = 0; setv(); tc_iph_install(); v_sz = sizeof b;
    CHECK_EQ_I(getenv_s(&n, b, v_sz, "TC_SEC_NEG1"), ERANGE); v_sz = n; CHECK_EQ_U(n, sizeof big);
    CHECK_EQ_I(getenv_s(&m, big, v_sz, "TC_SEC_NEG1"), 0); CHECK_STR(big, "hello world"); CHECK_EQ_U(m, 12); }
int main(void) { tc_begin(); TC_RUN(small_buffer_ERANGE); TC_RUN(retsize_gives_required_size_incl_nul); TC_RUN(retry_with_retsize_succeeds); _putenv_s("TC_SEC_NEG1", ""); return tc_end(); }
