/* TC
 * id:    str_strcat_s_basic
 * area:  string
 * funcs: strcat_s
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strcat_s, wcscat_s, _mbscat_s (returns 0 on success)
 */
#include "tc.h"
#define MINGW_HAS_SECURE_API 1
#include <string.h>
#include <errno.h>

static const char *volatile src;

TC_CASE(fits_exactly)
{
    char b[6] = "ab"; int r; errno = 0; tc_iph_count = 0; src = "cde";
    r = strcat_s(b, 6, src);
    CHECK_EQ_I(r, 0); CHECK_STR(b, "abcde"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(append_empty)
{
    char b[8] = "ab"; int r; src = "";
    r = strcat_s(b, sizeof b, src);
    CHECK_EQ_I(r, 0); CHECK_STR(b, "ab");
}
TC_CASE(dest_empty)
{
    char b[8] = ""; int r; src = "xyz";
    r = strcat_s(b, sizeof b, src);
    CHECK_EQ_I(r, 0); CHECK_STR(b, "xyz");
}
TC_CASE(chained)
{
    char b[16] = ""; int r = 0;
    src = "one"; r |= strcat_s(b, sizeof b, src);
    src = "-";   r |= strcat_s(b, sizeof b, src);
    src = "two"; r |= strcat_s(b, sizeof b, src);
    CHECK_EQ_I(r, 0); CHECK_STR(b, "one-two");
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(fits_exactly); TC_RUN(append_empty); TC_RUN(dest_empty); TC_RUN(chained);
    return tc_end();
}
