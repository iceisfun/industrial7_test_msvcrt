/* TC
 * id:    str_neg_strcpy_s_null_src
 * area:  string
 * funcs: strcpy_s
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcpy_s error conditions table: strSource NULL -> EINVAL, strDestination[0] set to 0
 */
#include "tc.h"
#define MINGW_HAS_SECURE_API 1
#include <string.h>
#include <errno.h>

static const char *volatile ps;

TC_CASE(null_src)
{
    char b[8]; int r, i;
    for (i = 0; i < 8; i++) b[i] = 'x';
    errno = 0; tc_iph_count = 0; ps = NULL;
    r = strcpy_s(b, sizeof b, ps);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL setup no handler API\n"); return 2; }
    TC_RUN(null_src);
    return tc_end();
}
