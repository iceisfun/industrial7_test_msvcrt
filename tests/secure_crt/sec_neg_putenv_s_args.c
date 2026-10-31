/* TC
 * id:    sec_neg_putenv_s_args
 * area:  secure_crt
 * funcs: _putenv_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _putenv_s, _wputenv_s (EINVAL for NULL name/value)
 * note:  name with '=' and empty name are in sec_neg_putenv_s_name_probe
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <locale.h>
TC_CASE(null_name_EINVAL) { tc_iph_install(); tc_iph_count = 0; CHECK_EQ_I(_putenv_s(NULL, "v"), EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(null_value_EINVAL) { tc_iph_install(); tc_iph_count = 0; CHECK_EQ_I(_putenv_s("TC_SEC_NEG3", NULL), EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(handler_counts) { tc_iph_install(); tc_iph_count = 0; _putenv_s(NULL, "v"); CHECK(tc_iph_count >= 1); _putenv_s("TC_SEC_NEG3", NULL); CHECK(tc_iph_count >= 2); }
int main(void) { tc_begin(); TC_RUN(null_name_EINVAL); TC_RUN(null_value_EINVAL); TC_RUN(handler_counts); return tc_end(); }
