/* TC
 * id:    pfm_1_neg_probe_unknown_conv_legacy
 * area:  stdio_printf
 * funcs: sprintf _snprintf
 * procs: sprintf_s
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf Format specification / Parameter validation: an invalid format string invokes the invalid parameter handler and the function returns -1 with errno EINVAL
 * note:  PROBES that VS2005-2013 CRTs (not just UCRT) reject unknown conversions with -1/EINVAL/handler call (expected, from the _VALIDATE_RETURN in the VC8+ printf engine). Cases: %k %y %q, a lone trailing '%', an incomplete "%*"; buffer contents after failure are not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
static int r_sprintf(const char *volatile f)   { char b[32]; tc_iph_count = 0; errno = 0; return sprintf(b, f, 1); }
static int r_snprintf(const char *volatile f)  { char b[32]; tc_iph_count = 0; errno = 0; return _snprintf(b, sizeof b, f, 1); }
static int r_sprintf_s(const char *volatile f) { char b[32]; tc_iph_count = 0; errno = 0; return tc_p_sprintf_s(b, sizeof b, f, 1); }
#define BAD(r) do { CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); } while (0)

TC_CASE(sprintf_k)      { int r = r_sprintf("%k");  BAD(r); }
TC_CASE(sprintf_q)      { int r = r_sprintf("a%qb"); BAD(r); }
TC_CASE(sprintf_trail)  { int r = r_sprintf("abc%");  BAD(r); }
TC_CASE(sprintf_star)   { int r = r_sprintf("%*");    BAD(r); }
TC_CASE(snprintf_y)     { int r = r_snprintf("%y");   BAD(r); }
TC_CASE(sprintf_s_y)    { int r = r_sprintf_s("%y");  BAD(r); }
TC_CASE(sprintf_s_pct)  { int r = r_sprintf_s("%");   BAD(r); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_LOAD_PROC(sprintf_s); if (!tc_p_sprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__sprintf_s missing\n"); return 2; }
    TC_RUN(sprintf_k); TC_RUN(sprintf_q); TC_RUN(sprintf_trail); TC_RUN(sprintf_star);
    TC_RUN(snprintf_y); TC_RUN(sprintf_s_y); TC_RUN(sprintf_s_pct);
    return tc_end();
}
