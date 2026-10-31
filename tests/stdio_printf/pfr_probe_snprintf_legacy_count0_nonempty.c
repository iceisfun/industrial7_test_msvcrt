/* TC
 * id:    pfr_probe_snprintf_legacy_count0_nonempty
 * area:  stdio_printf
 * funcs:
 * procs: _snprintf
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _snprintf
 * note:  PROBING, one per case, whether _snprintf(buf,0,"abc") (a) returns -1, (b) leaves buf untouched, (c) does NOT invoke the
 *        invalid-parameter handler (iph count 0).
 */
#include "tc.h"

typedef int (__cdecl *snprintf_t)(char *, size_t, const char *, ...);
static snprintf_t p_snprintf;

TC_CASE(ret_minus1)
{
    char b[4]; int r;
    if (!p_snprintf) TC_SKIP("_snprintf not exported");
    b[0] = 'Q';
    r = p_snprintf(b, 0, "%s", "abc");
    CHECK_EQ_I(r, -1);
}
TC_CASE(buffer_untouched)
{
    char b[4];
    if (!p_snprintf) TC_SKIP("_snprintf not exported");
    b[0] = 'Q'; b[1] = 'R';
    p_snprintf(b, 0, "%s", "abc");
    CHECK_EQ_I(b[0], 'Q'); CHECK_EQ_I(b[1], 'R');
}
TC_CASE(no_handler)
{
    char b[4];
    if (!p_snprintf) TC_SKIP("_snprintf not exported");
    tc_iph_count = 0;
    p_snprintf(b, 0, "%s", "abc");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_snprintf = (snprintf_t)tc_crt_proc("_snprintf");
    TC_RUN(ret_minus1); TC_RUN(buffer_untouched); TC_RUN(no_handler);
    return tc_end();
}
