/* TC
 * id:    str_neg_strncmp_zero_count
 * area:  string
 * funcs: strncmp _strnicmp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   C11 7.24.4.4; MS docs strncmp, _strnicmp (count 0 compares nothing)
 */
#include "tc.h"
#include <string.h>

static volatile const char *va, *vb;

TC_CASE(strncmp_zero) { va = "a"; vb = "b"; CHECK_EQ_I(strncmp((const char *)va, (const char *)vb, 0), 0); }
TC_CASE(strnicmp_zero) { va = "a"; vb = "b"; CHECK_EQ_I(_strnicmp((const char *)va, (const char *)vb, 0), 0); }
TC_CASE(differ_at_n_not_examined) { va = "abcX"; vb = "abcY"; CHECK_EQ_I(strncmp((const char *)va, (const char *)vb, 3), 0); CHECK_EQ_I(_strnicmp((const char *)va, (const char *)vb, 3), 0); }
TC_CASE(strncmp_zero_empty) { va = ""; vb = "zzz"; CHECK_EQ_I(strncmp((const char *)va, (const char *)vb, 0), 0); }

int main(void) { tc_begin(); TC_RUN(strncmp_zero); TC_RUN(strnicmp_zero); TC_RUN(differ_at_n_not_examined); TC_RUN(strncmp_zero_empty); return tc_end(); }
