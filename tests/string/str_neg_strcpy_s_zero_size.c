/* TC
 * id:    str_neg_strcpy_s_zero_size
 * area:  string
 * funcs: strcpy_s
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs strcpy_s error table (lists numberOfElements 0 together with 'too small' -> ERANGE; shipped CRT validates size>0 first -> EINVAL)
 * note:  The docs table is ambiguous/contradicts the shipped CRT (_VALIDATE_STRING rejects size 0 with EINVAL and leaves dest untouched).
 *        Kept as observed; if real Windows disagrees, that is a finding.
 */
#include "tc.h"
#define MINGW_HAS_SECURE_API 1
#include <string.h>
#include <errno.h>

static char *volatile pd;
static volatile size_t psz;

TC_CASE(zero_size)
{
    char b[4] = { 'x', 'y', 'z', 0 }; int r;
    errno = 0; tc_iph_count = 0; pd = b; psz = 0;
    r = strcpy_s(pd, psz, "a");
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_STR(b, "xyz");
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL setup no handler API\n"); return 2; }
    TC_RUN(zero_size);
    return tc_end();
}
