/* TC
 * id:    pfo_neg_invalid_format_specifier
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   MS docs printf type field (docs also say a % followed by a meaningless char is copied literally - conflicts with UCRT validation)
 * note:  OBSERVED: __stdio_common_vsprintf(0) copies unknown conversion chars (%y, %Q) literally (without the '%'), as the printf docs say,
 *        with no -1/EINVAL/handler. valid_still_works is the only documented case.
 *        Only unknown type characters are asserted here. End-of-string and size-prefix oddities are in pfo_neg_invalid_format_probe.
 *        Buffer content after failure is not asserted.
 *        Observed on Win10 22H2 19045 ucrtbase: the non-_s printf family does not validate format strings (no handler call, no errno); the bad conversion char is copied through.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int cv(char *b, size_t n, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vsprintf(0, b, n, f, NULL, ap); va_end(ap);
    return r;
}
TC_CASE(unknown_y)
{
    char b[32]; const char * volatile f = "a%yb"; int r;
    tc_iph_count = 0; errno = 0;
    r = cv(b, sizeof b, f, 1);
    CHECK_EQ_I(r, 3); CHECK_STR(b, "ayb");
    CHECK_EQ_I(errno, 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(unknown_Q)
{
    char b[32]; const char * volatile f = "%Q"; int r;
    tc_iph_count = 0; errno = 0;
    r = cv(b, sizeof b, f, 1);
    CHECK_EQ_I(r, 1); CHECK_STR(b, "Q");
    CHECK_EQ_I(errno, 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(valid_still_works)
{
    char b[32]; int r;
    tc_iph_count = 0;
    r = cv(b, sizeof b, "%d%%", 5);
    CHECK_EQ_I(r, 2);
    CHECK_STR(b, "5%");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(unknown_y); TC_RUN(unknown_Q); TC_RUN(valid_still_works);
    return tc_end();
}
