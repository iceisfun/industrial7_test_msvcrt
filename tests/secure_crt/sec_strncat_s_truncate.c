/* TC
 * id:    sec_strncat_s_truncate
 * area:  secure_crt
 * funcs: strncat_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strncat_s (_TRUNCATE)
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

static volatile const char *v_s = "XYZ";

TC_CASE(fits_returns_0) { char d[16] = "ab"; CHECK_EQ_I(strncat_s(d, sizeof d, (const char *)v_s, (size_t)-1), 0); CHECK_STR(d, "abXYZ"); }
TC_CASE(too_long_returns_STRUNCATE) { char d[5] = "ab"; CHECK_EQ_I(strncat_s(d, sizeof d, (const char *)v_s, (size_t)-1), STRUNCATE); }
TC_CASE(dest_terminated_and_prefix_kept) { char d[5] = "ab"; strncat_s(d, sizeof d, (const char *)v_s, (size_t)-1); CHECK_STR(d, "abXY"); CHECK_EQ_I(d[4], 0); }

int main(void) { tc_begin(); TC_RUN(fits_returns_0); TC_RUN(too_long_returns_STRUNCATE); TC_RUN(dest_terminated_and_prefix_kept); return tc_end(); }
