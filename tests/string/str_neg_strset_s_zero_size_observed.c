/* TC
 * id:    str_neg_strset_s_zero_size_observed
 * area:  string
 * funcs: _strset_s _strnset_s
 * crt:   >=100
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _strset_s _strnset_s error table (numberOfElements 0 not clearly specified); shipped CRT _VALIDATE_STRING -> EINVAL
 * note:  Split from the documented tests: size 0 -> EINVAL (not ERANGE), handler called once, destination untouched.
 */
#include "tc.h"
#define MINGW_HAS_SECURE_API 1
#include <string.h>
#include <errno.h>

static char *volatile pd;
static const char *volatile ps;
static volatile size_t psz;

TC_CASE(size_zero)
{
    char b[8] = "ab"; int r; errno = 0; tc_iph_count = 0; pd = b; ps = "c"; psz = 0;
    r = _strset_s(pd, psz, 'x');
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0;
    r = _strnset_s(pd, psz, 'x', 1);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_STR(b, "ab");
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL setup no handler API\n"); return 2; }
    TC_RUN(size_zero);
    return tc_end();
}
