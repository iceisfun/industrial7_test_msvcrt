/* TC
 * id:    str_neg_strcpy_s_null_dest
 * area:  string
 * funcs: strcpy_s
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcpy_s error conditions table: strDestination NULL -> EINVAL, invalid parameter handler invoked
 */
#include "tc.h"
#define MINGW_HAS_SECURE_API 1
#include <string.h>
#include <errno.h>

static char *volatile pd;
static volatile size_t psz;

TC_CASE(null_dest)
{
    int r; errno = 0; tc_iph_count = 0; pd = NULL; psz = 10;
    r = strcpy_s(pd, psz, "a");
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_dest_size0)
{
    int r; errno = 0; tc_iph_count = 0; pd = NULL; psz = 0;
    r = strcpy_s(pd, psz, "a");
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL setup no handler API\n"); return 2; }
    TC_RUN(null_dest); TC_RUN(null_dest_size0);
    return tc_end();
}
