/* TC
 * id:    str_neg_strcat_s_invalid
 * area:  string
 * funcs: strcat_s
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcat_s error conditions table: NULL dest / dest not terminated -> EINVAL; NULL src -> EINVAL and dest[0]=0
 */
#include "tc.h"
#define MINGW_HAS_SECURE_API 1
#include <string.h>
#include <errno.h>

static char *volatile pd;
static const char *volatile ps;
static volatile size_t psz;

TC_CASE(null_dest)
{
    int r; errno = 0; tc_iph_count = 0; pd = NULL; ps = "a"; psz = 10;
    r = strcat_s(pd, psz, ps);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_src)
{
    char b[8] = "ab"; int r; errno = 0; tc_iph_count = 0; ps = NULL;
    r = strcat_s(b, sizeof b, ps);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(dest_unterminated)
{
    char b[8]; int r, i;
    for (i = 0; i < 8; i++) b[i] = 'x';
    errno = 0; tc_iph_count = 0; pd = b; ps = "c"; psz = 4;   /* no NUL within the first 4 bytes */
    r = strcat_s(pd, psz, ps);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK(b[4] == 'x' && b[7] == 'x');
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL setup no handler API\n"); return 2; }
    TC_RUN(null_dest); TC_RUN(null_src); TC_RUN(dest_unterminated);
    return tc_end();
}
