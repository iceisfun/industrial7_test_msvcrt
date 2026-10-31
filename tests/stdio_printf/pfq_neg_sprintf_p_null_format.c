/* TC
 * id:    pfq_neg_sprintf_p_null_format
 * area:  stdio_printf
 * kind:  negative
 * funcs: _sprintf_p _swprintf_p _fprintf_p
 * procs: _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_p, fprintf_p, swprintf_p: NULL format -> invalid parameter handler, return -1, errno EINVAL
 * note:  The handler may be reached more than once per call on some builds, so only >= 1 is asserted. The destination buffer contents after failure are deliberately not asserted.
 */
#include "tc.h"
#include "pfq_shim.h"
#include <errno.h>

#define XS_(x) #x
#define XS(x) XS_(x)
#define FN "tc_pfq_neg_sprintf_p_null_format_" XS(TC_CRT_VER) ".tmp"

TC_CASE(sprintf_p)
{
    char b[10]; const char * volatile f = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = pq_sprintf_p(b, sizeof b, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(swprintf_p)
{
    wchar_t b[10]; const wchar_t * volatile f = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = pq_swprintf_p(b, 10, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(fprintf_p)
{
    FILE *fp = fopen(FN, "wb"); const char * volatile f = NULL; int r;
    CHECK(fp != NULL); if (!fp) return;
    tc_iph_count = 0; errno = 0;
    r = pq_fprintf_p(fp, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
    fclose(fp);
    CHECK_EQ_I(remove(FN), 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(sprintf_p); TC_RUN(swprintf_p); TC_RUN(fprintf_p);
    return tc_end();
}
