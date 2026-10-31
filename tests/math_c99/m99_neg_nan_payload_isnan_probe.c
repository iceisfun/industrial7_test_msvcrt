/* TC
 * id:    m99_neg_nan_payload_isnan_probe
 * area:  math_c99
 * funcs: nan nanf
 * procs: _fpclass _isnan
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs nan, _fpclass (_FPCLASS_QNAN 0x0002)
 * note:  PROBE: bits of nan("") assumed 0x7ff8000000000000 (default quiet NaN, sign clear); nan("1"), nan("0x1"), nan("abc") bit patterns are only PRINTED (TC INFO), not asserted; errno untouched by nan(""). _fpclass(nan("")) == _FPCLASS_QNAN.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

typedef int (__cdecl *fpc_t)(double); typedef int (__cdecl *isn_t)(double);
static fpc_t p_fpclass; static isn_t p_isnan;

static void infob(const char *what, unsigned long long b)
{ char h[24]; tc_hex(b, h); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write(" bits="); tc_write(h); tc_write("\n"); }

TC_CASE(empty_bits) { volatile const char *t = ""; double r = nan((const char *)t); infob("nan(\"\")", tc_dbits(r)); CHECK_EQ_U(tc_dbits(r), 0x7ff8000000000000ULL); }
TC_CASE(signbit_clear) { volatile const char *t = ""; double r = nan((const char *)t); CHECK((tc_dbits(r) >> 63) == 0); }
TC_CASE(tags_recorded)
{
    volatile const char *a = "1", *b = "0x1", *c = "abc"; double r;
    r = nan((const char *)a); infob("nan(\"1\")", tc_dbits(r)); CHECK(r != r);
    r = nan((const char *)b); infob("nan(\"0x1\")", tc_dbits(r)); CHECK(r != r);
    r = nan((const char *)c); infob("nan(\"abc\")", tc_dbits(r)); CHECK(r != r);
    { volatile const char *e = ""; float f = nanf((const char *)e); infob("nanf(\"\")", tc_fbits(f)); CHECK(f != f); }
}
TC_CASE(fpclass_isnan)
{
    volatile const char *t = ""; double r = nan((const char *)t);
    if (!p_fpclass || !p_isnan) TC_SKIP("missing");
    CHECK_EQ_I(p_fpclass(r), 0x0002);
    CHECK(p_isnan(r) != 0);
}
TC_CASE(errno_untouched)
{
    volatile const char *t = ""; double r;
    errno = 9; r = nan((const char *)t); CHECK(r != r); CHECK_EQ_I(errno, 9);
}
int main(void)
{
    tc_begin();
    p_fpclass = (fpc_t)tc_crt_proc("_fpclass"); p_isnan = (isn_t)tc_crt_proc("_isnan");
    TC_RUN(empty_bits); TC_RUN(signbit_clear); TC_RUN(tags_recorded); TC_RUN(fpclass_isnan); TC_RUN(errno_untouched);
    return tc_end();
}
