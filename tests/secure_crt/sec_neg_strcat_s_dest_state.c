/* TC
 * id:    sec_neg_strcat_s_dest_state
 * area:  secure_crt
 * funcs:
 * procs: strcat_s strncat_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcat_s / strncat_s error-condition tables (src NULL: EINVAL, dest[0] set to 0)
 * note:  unterminated-dest and size-0 cases moved to sec_neg_strcat_s_dest_probe (docs say dest not modified there; unsure)
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(errno_t, strcat_s, (char*,size_t,const char*));
TC_DECL_PROC(errno_t, strncat_s, (char*,size_t,const char*,size_t));
#ifndef STRUNCATE
#define STRUNCATE 80
#endif
#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif
static const char *volatile v_null;
static volatile size_t v_zero;
static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }

TC_CASE(cat_null_src) { char d[8] = "ab"; setup(); CHECK_EQ_I(tc_p_strcat_s(d, sizeof d, v_null), EINVAL); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ncat_null_src) { char d[8] = "ab"; setup(); CHECK_EQ_I(tc_p_strncat_s(d, sizeof d, v_null, 3), EINVAL); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 1); }

int main(void) { tc_begin(); TC_LOAD_PROC(strcat_s); TC_LOAD_PROC(strncat_s); TC_RUN(cat_null_src); TC_RUN(ncat_null_src); return tc_end(); }
