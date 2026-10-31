/* TC
 * id:    sec_neg_strcat_s_range
 * area:  secure_crt
 * funcs: strcat_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcat_s (ERANGE, dest[0] set to 0)
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static volatile const char *v_s = "abcdef";
static char *volatile v_null;
static const char *volatile v_nulls;

static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }

TC_CASE(one_byte_short_ERANGE) { char d[6] = "ab"; setup(); CHECK_EQ_I(strcat_s(d, 6, "cdef"), ERANGE); } /* needs 7 */
TC_CASE(dest_first_byte_zeroed) { char d[6] = "ab"; setup(); strcat_s(d, 6, "cdef"); CHECK_EQ_I(d[0], 0); }
TC_CASE(errno_ERANGE) { char d[6] = "ab"; setup(); strcat_s(d, 6, "cdef"); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(handler_count_one) { char d[6] = "ab"; setup(); strcat_s(d, 6, "cdef"); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(dest_already_full_ERANGE) { char d[5] = "abcd"; setup(); CHECK_EQ_I(strcat_s(d, 5, (const char *)v_s), ERANGE); CHECK_EQ_I(tc_iph_count, 1); }

int main(void) { tc_begin(); TC_RUN(one_byte_short_ERANGE); TC_RUN(dest_first_byte_zeroed); TC_RUN(errno_ERANGE); TC_RUN(handler_count_one); TC_RUN(dest_already_full_ERANGE); return tc_end(); }
