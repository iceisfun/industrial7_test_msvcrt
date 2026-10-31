/* TC
 * id:    scn_neg_sscanf_s_too_small_ret_probe
 * area:  scanf
 * kind:  negative
 * funcs: sscanf_s swscanf_s sscanf
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs scanf_s (return value after an invalid-parameter failure is not precisely documented)
 * note:  PROBES, with a returning invalid-parameter handler: (1) sscanf_s %5c into size 3, %[a-z] into size 4 and swscanf_s %ls into
 *        size 4 each return 0 (could instead be EOF); (2) after sscanf_s("%s", buf, 4) overflows, buf[0] == 0 (empty string);
 *        (3) lone sign "+"/"-" for %d and lone "." for %f leave the target untouched and return 0 or EOF.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const char *volatile vin; static const wchar_t *volatile vwin;
TC_CASE(c_ret0) { char a[8] = "ZZZZZZZ"; int n; CHECK(tc_iph_install()); vin = "abcdefg"; n = sscanf_s((const char *)vin, "%5c", a, (unsigned)3); CHECK_EQ_I(n, 0); }
TC_CASE(set_ret0) { char a[8] = "ZZZZZZZ"; int n; CHECK(tc_iph_install()); vin = "abcdefgh"; n = sscanf_s((const char *)vin, "%[a-z]", a, (unsigned)4); CHECK_EQ_I(n, 0); }
TC_CASE(ls_ret0) { wchar_t a[8] = L"ZZZZZZZ"; int n; CHECK(tc_iph_install()); vwin = L"abcdef"; n = swscanf_s((const wchar_t *)vwin, L"%ls", a, (unsigned)4); CHECK_EQ_I(n, 0); }
TC_CASE(s_emptied) { char a[8] = "ZZZZZZZ"; CHECK(tc_iph_install()); vin = "abcdefgh"; (void)sscanf_s((const char *)vin, "%s", a, (unsigned)4); CHECK_EQ_I(a[0], 0); }
TC_CASE(sign_alone)
{
    int d = 5, n;
    vin = "+"; n = sscanf((const char *)vin, "%d", &d); CHECK(n == 0 || n == EOF); CHECK_EQ_I(d, 5);
    vin = "-"; n = sscanf((const char *)vin, "%d", &d); CHECK(n == 0 || n == EOF); CHECK_EQ_I(d, 5);
}
TC_CASE(dot_alone) { float f = 2.5f; int n; vin = "."; n = sscanf((const char *)vin, "%f", &f); CHECK(n == 0 || n == EOF); CHECK_BITS_F(f, 2.5f); }
int main(void) { tc_begin(); TC_RUN(c_ret0); TC_RUN(set_ret0); TC_RUN(ls_ret0); TC_RUN(s_emptied); TC_RUN(sign_alone); TC_RUN(dot_alone); return tc_end(); }
