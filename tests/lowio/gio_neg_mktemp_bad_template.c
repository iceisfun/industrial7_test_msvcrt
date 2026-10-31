/* TC
 * id:    gio_neg_mktemp_bad_template
 * area:  lowio
 * funcs: _mktemp _wmktemp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mktemp: returns NULL if template is badly formed
 * note:  errno / invalid-parameter handler are not asserted here (see gio_neg_probe_mktemp_errno_iph).
 *        Handler is installed (where the CRT has the API) so validation CRTs do not terminate.
 *        Buffer state after failure is NOT asserted (see gio_neg_probe_mktemp_errno_iph).
 *        Templates are given leading pad bytes so old CRTs scanning backwards stay in bounds.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

TC_CASE(no_x)      { char b[16] = "zzzzabc"; CHECK(_mktemp(b + 4) == NULL); }
TC_CASE(five_x)    { char b[16] = "zzzzabcXXXXX"; CHECK(_mktemp(b + 4) == NULL); }
TC_CASE(empty_string) { char b[16] = "zzzz"; CHECK(_mktemp(b + 4) == NULL); }
TC_CASE(x_not_trailing) { char b[16] = "zzzzXXXXXXabc"; CHECK(_mktemp(b + 4) == NULL); }
TC_CASE(wide_variants)
{
    wchar_t a[16] = L"zzzzabc", b[16] = L"zzzzabcXXXXX", c[16] = L"zzzz", d[16] = L"zzzzXXXXXXabc";
    CHECK(_wmktemp(a + 4) == NULL);
    CHECK(_wmktemp(b + 4) == NULL);
    CHECK(_wmktemp(c + 4) == NULL);
    CHECK(_wmktemp(d + 4) == NULL);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(no_x); TC_RUN(five_x); TC_RUN(empty_string); TC_RUN(x_not_trailing);
    TC_RUN(wide_variants);
    return tc_end();
}
