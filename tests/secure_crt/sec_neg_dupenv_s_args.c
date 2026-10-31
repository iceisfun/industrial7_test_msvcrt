/* TC
 * id:    sec_neg_dupenv_s_args
 * area:  secure_crt
 * funcs: _dupenv_s _wdupenv_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _dupenv_s, _wdupenv_s
 * note:  buffer-set-to-NULL on invalid varname is in sec_neg_dupenv_s_nullvar_probe
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <locale.h>
TC_CASE(null_buffer_ptr_EINVAL) { size_t n = 7; tc_iph_install(); tc_iph_count = 0;
    CHECK_EQ_I(_dupenv_s(NULL, &n, "PATH"), EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(null_varname_EINVAL) { char *p = NULL; size_t n = 7; tc_iph_install(); tc_iph_count = 0;
    CHECK_EQ_I(_dupenv_s(&p, &n, NULL), EINVAL); CHECK(tc_iph_count >= 1); free(p); }
TC_CASE(wide_null_buffer_ptr_EINVAL) { size_t n = 7; tc_iph_install(); tc_iph_count = 0;
    CHECK_EQ_I(_wdupenv_s(NULL, &n, L"PATH"), EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(absent_var_not_an_error_no_handler) { char *p = (char *)1; size_t n = 7; wchar_t *w = (wchar_t *)1; size_t m = 7;
    tc_iph_install(); tc_iph_count = 0;
    CHECK_EQ_I(_dupenv_s(&p, &n, "TC_SEC_NEG_ABSENT_VAR"), 0); CHECK(p == NULL); CHECK_EQ_U(n, 0);
    CHECK_EQ_I(_wdupenv_s(&w, &m, L"TC_SEC_NEG_ABSENT_VAR"), 0); CHECK(w == NULL); CHECK_EQ_U(m, 0);
    CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(handler_counts) { char *p = NULL; size_t n = 0; tc_iph_install(); tc_iph_count = 0;
    _dupenv_s(NULL, &n, "X"); CHECK(tc_iph_count >= 1); _dupenv_s(&p, &n, NULL); CHECK(tc_iph_count >= 2); free(p); }
int main(void) { tc_begin(); TC_RUN(null_buffer_ptr_EINVAL); TC_RUN(null_varname_EINVAL); TC_RUN(wide_null_buffer_ptr_EINVAL);
    TC_RUN(absent_var_not_an_error_no_handler); TC_RUN(handler_counts); return tc_end(); }
