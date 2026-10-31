/* TC
 * id:    scn_neg_sscanf_null_input
 * area:  scanf
 * funcs: sscanf swscanf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sscanf, swscanf (NULL string / format: invalid parameter handler invoked, return EOF, errno EINVAL)
 * note:  msvcrt.dll excluded: no invalid parameter handler. Destination stays unchanged; the function works afterwards.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(sscanf_null)
{
    int x = 99, r; const char *volatile s = NULL;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    tc_iph_count = 0; errno = 0;
    r = sscanf(s, "%d", &x);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(x, 99);
}
TC_CASE(swscanf_null)
{
    int x = 99, r; const wchar_t *volatile s = NULL;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    tc_iph_count = 0; errno = 0;
    r = swscanf(s, L"%d", &x);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(x, 99);
}
TC_CASE(swscanf_null_format)
{
    int r; const wchar_t *volatile f = NULL;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    tc_iph_count = 0; errno = 0;
    r = swscanf(L"1", f);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(usable_after)
{
    int x = 0, r; const char *volatile s = NULL;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    r = sscanf(s, "%d", &x);
    CHECK_EQ_I(r, EOF);
    r = sscanf("12", "%d", &x);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(x, 12);
}
int main(void)
{
    tc_begin();
    TC_RUN(sscanf_null); TC_RUN(swscanf_null); TC_RUN(swscanf_null_format); TC_RUN(usable_after);
    return tc_end();
}
