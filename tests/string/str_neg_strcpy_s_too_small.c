/* TC
 * id:    str_neg_strcpy_s_too_small
 * area:  string
 * funcs: strcpy_s
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcpy_s error conditions table: destination too small -> ERANGE, strDestination[0] set to 0
 */
#include "tc.h"
#define MINGW_HAS_SECURE_API 1
#include <string.h>
#include <errno.h>

static const char *volatile src;
static volatile size_t psz;

TC_CASE(size_equals_len)
{
    char b[8]; int r, i;
    for (i = 0; i < 8; i++) b[i] = 'x';
    errno = 0; tc_iph_count = 0; src = "hello"; psz = 5;
    r = strcpy_s(b, psz, src);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(size1_nonempty)
{
    char b[4]; int r, i;
    for (i = 0; i < 4; i++) b[i] = 'x';
    errno = 0; tc_iph_count = 0; src = "a"; psz = 1;
    r = strcpy_s(b, psz, src);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(control_len_plus_1)
{
    char b[8]; int r;
    errno = 0; tc_iph_count = 0; src = "hello"; psz = 6;
    r = strcpy_s(b, psz, src);
    CHECK_EQ_I(r, 0); CHECK_STR(b, "hello"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL setup no handler API\n"); return 2; }
    TC_RUN(size_equals_len); TC_RUN(size1_nonempty); TC_RUN(control_len_plus_1);
    return tc_end();
}
