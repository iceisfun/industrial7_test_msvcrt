/* TC
 * id:    str_neg_strset_s_invalid
 * area:  string
 * funcs: _strset_s _strnset_s
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strset_s, _strnset_s (error conditions: NULL str / str not terminated within numberOfElements -> EINVAL)
 * note:  Only return code, errno and the invalid-parameter handler are asserted. Destination state after an error is probed in str_neg_strset_s_dest_state_probe.
 */
#include "tc.h"
#define MINGW_HAS_SECURE_API 1
#include <string.h>
#include <errno.h>

static char *volatile pd;
static volatile size_t psz;

TC_CASE(control_ok)
{
    char b[8] = "ab"; int r; tc_iph_count = 0; errno = 0;
    r = _strset_s(b, sizeof b, 'x');
    CHECK_EQ_I(r, 0); CHECK_STR(b, "xx"); CHECK_EQ_I(tc_iph_count, 0);
    r = _strnset_s(b, sizeof b, 'y', 1);
    CHECK_EQ_I(r, 0); CHECK_STR(b, "yx"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(null_dest)
{
    int r; tc_iph_count = 0; errno = 0; pd = NULL; psz = 10;
    r = _strset_s(pd, psz, 'x');
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0;
    r = _strnset_s(pd, psz, 'x', 2);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(unterminated)
{
    char b[8]; int r, i;
    for (i = 0; i < 8; i++) b[i] = 'q';
    tc_iph_count = 0; errno = 0; pd = b; psz = 4;      /* only the first 4 bytes are the buffer: no NUL in them */
    r = _strset_s(pd, psz, 'x');
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    for (i = 0; i < 8; i++) b[i] = 'q';
    tc_iph_count = 0; errno = 0;
    r = _strnset_s(pd, psz, 'x', 2);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK(b[4] == 'q' && b[7] == 'q');                 /* nothing written past the stated size */
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL setup no handler API\n"); return 2; }
    TC_RUN(control_ok); TC_RUN(null_dest); TC_RUN(unterminated);
    return tc_end();
}
