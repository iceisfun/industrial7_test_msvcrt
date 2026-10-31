/* TC
 * id:    str_neg_strxfrm_invalid_retval_probe
 * area:  string
 * funcs: strxfrm _strxfrm_l
 * crt:   >=100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   none
 * note:  PROBE: the exact error return of strxfrm/_strxfrm_l on invalid parameters is INT_MAX (as size_t), not (size_t)-1 as MS docs state. Most likely value is INT_MAX (_VALIDATE_RETURN(..., EINVAL, INT_MAX)).
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
    CHECK_EQ_U(r, INT_MAX); 
}
TC_CASE(null_src)
{
    char d[8]; size_t r; tc_iph_count = 0; errno = 0; vs = NULL;
    r = strxfrm(d, (const char *)vs, sizeof d);
    CHECK_EQ_U(r, INT_MAX); 
}
TC_CASE(l_null_dest_count)
{
    size_t r; tc_iph_count = 0; errno = 0; vs = "a"; vd = NULL;
    r = _strxfrm_l(vd, (const char *)vs, 5, NULL);
    CHECK_EQ_U(r, INT_MAX); 
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(null_dest_count); TC_RUN(null_src); TC_RUN(l_null_dest_count);
    return tc_end();
}
