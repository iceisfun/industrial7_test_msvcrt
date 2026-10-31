/* TC
 * id:    str_neg_strxfrm_invalid_probe
 * area:  string
 * funcs: strxfrm _strxfrm_l
 * crt:   >=100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   none
 * note:  PROBE: strxfrm/_strxfrm_l with NULL dest and count>0, or NULL src, validate params: errno=EINVAL, handler called once, return is an error value (INT_MAX or (size_t)-1; the exact value is probed in str_neg_strxfrm_invalid_retval_probe).
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <limits.h>

static volatile const char *vs;
static char *volatile vd;

TC_CASE(null_dest_count)
{
    size_t r; tc_iph_count = 0; errno = 0; vs = "a"; vd = NULL;
    r = strxfrm(vd, (const char *)vs, 5);
    CHECK(r == (size_t)INT_MAX || r == (size_t)-1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_src)
{
    char d[8]; size_t r; tc_iph_count = 0; errno = 0; vs = NULL;
    r = strxfrm(d, (const char *)vs, sizeof d);
    CHECK(r == (size_t)INT_MAX || r == (size_t)-1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(l_null_dest_count)
{
    size_t r; tc_iph_count = 0; errno = 0; vs = "a"; vd = NULL;
    r = _strxfrm_l(vd, (const char *)vs, 5, NULL);
    CHECK(r == (size_t)INT_MAX || r == (size_t)-1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(null_dest_count); TC_RUN(null_src); TC_RUN(l_null_dest_count);
    return tc_end();
}
