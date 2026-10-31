/* TC
 * id:    sec_neg_strcpy_s_range
 * area:  secure_crt
 * funcs: strcpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcpy_s (ERANGE, dest[0] set to 0)
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static volatile const char *v_s = "abcdef";
static char *volatile v_null;
static const char *volatile v_nulls;

#define INIT(d) memset(d, 'x', sizeof d)
static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }

TC_CASE(size_equals_strlen_ERANGE) { char d[6]; INIT(d); setup(); CHECK_EQ_I(strcpy_s(d, 6, (const char *)v_s), ERANGE); }
TC_CASE(size_one_nonempty_src_ERANGE) { char d[6]; INIT(d); setup(); CHECK_EQ_I(strcpy_s(d, 1, (const char *)v_s), ERANGE); CHECK_EQ_I(d[0], 0); }
TC_CASE(dest_first_byte_zeroed) { char d[6]; INIT(d); setup(); strcpy_s(d, 3, (const char *)v_s); CHECK_EQ_I(d[0], 0); }
TC_CASE(errno_ERANGE) { char d[6]; INIT(d); setup(); strcpy_s(d, 3, (const char *)v_s); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(handler_count_one) { char d[6]; INIT(d); setup(); strcpy_s(d, 3, (const char *)v_s); CHECK_EQ_I(tc_iph_count, 1); }

int main(void) { tc_begin(); TC_RUN(size_equals_strlen_ERANGE); TC_RUN(size_one_nonempty_src_ERANGE); TC_RUN(dest_first_byte_zeroed); TC_RUN(errno_ERANGE); TC_RUN(handler_count_one); return tc_end(); }
