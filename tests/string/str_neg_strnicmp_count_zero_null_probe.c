/* TC
 * id:    str_neg_strnicmp_count_zero_null_probe
 * area:  string
 * funcs: _strnicmp strncmp
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _strnicmp, _strnicmp_l, strncmp (count == 0 not discussed for NULL pointers)
 * note:  PROBE: with count == 0 the comparison functions return 0 without touching the pointers; the UCRT _strnicmp returns 0 early for count 0 before validating pointers, and strncmp never dereferences. Separate cases: count 0 with NULL pointers for _strnicmp and strncmp (strncmp with NULL is formally UB in ISO C; probed only because count 0 reads nothing). The non-zero-count NULL error path is in str_neg_strnicmp_null_retval_errno.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
static const char *volatile pn = NULL;
static const char *volatile pa = "a";

TC_CASE(strnicmp_count0_both_null)
{ errno = 0; tc_iph_count = 0; CHECK_EQ_I(_strnicmp(pn, pn, 0), 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(strnicmp_count0_first_null)
{ errno = 0; tc_iph_count = 0; CHECK_EQ_I(_strnicmp(pn, pa, 0), 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(strncmp_count0_null)
{ CHECK_EQ_I(strncmp(pn, pn, 0), 0); CHECK_EQ_I(strncmp(pn, pa, 0), 0); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(strnicmp_count0_both_null); TC_RUN(strnicmp_count0_first_null);
    TC_RUN(strncmp_count0_null);
    return tc_end();
}
