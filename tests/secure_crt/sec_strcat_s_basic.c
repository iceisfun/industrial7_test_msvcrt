/* TC
 * id:    sec_strcat_s_basic
 * area:  secure_crt
 * funcs: strcat_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strcat_s
 */
#include "tc.h"
#include <string.h>

TC_CASE(append) { char d[16] = "foo"; volatile const char *s = "bar"; CHECK_EQ_I(strcat_s(d, sizeof d, (const char *)s), 0); CHECK_STR(d, "foobar"); }
TC_CASE(append_empty) { char d[16] = "foo"; volatile const char *s = ""; CHECK_EQ_I(strcat_s(d, sizeof d, (const char *)s), 0); CHECK_STR(d, "foo"); }
TC_CASE(exact_fit) { char d[7] = "foo"; volatile const char *s = "bar"; CHECK_EQ_I(strcat_s(d, 7, (const char *)s), 0); CHECK_STR(d, "foobar"); }
TC_CASE(build_from_empty_dest) { char d[16]; volatile const char *s = "xyz"; d[0] = 0; CHECK_EQ_I(strcat_s(d, sizeof d, (const char *)s), 0); CHECK_STR(d, "xyz"); }
TC_CASE(chain_three) { char d[16]; volatile const char *a = "a", *b = "bb", *c = "ccc"; d[0] = 0;
    CHECK_EQ_I(strcat_s(d, sizeof d, (const char *)a), 0); CHECK_EQ_I(strcat_s(d, sizeof d, (const char *)b), 0); CHECK_EQ_I(strcat_s(d, sizeof d, (const char *)c), 0); CHECK_STR(d, "abbccc"); }

int main(void) { tc_begin(); TC_RUN(append); TC_RUN(append_empty); TC_RUN(exact_fit); TC_RUN(build_from_empty_dest); TC_RUN(chain_three); return tc_end(); }
