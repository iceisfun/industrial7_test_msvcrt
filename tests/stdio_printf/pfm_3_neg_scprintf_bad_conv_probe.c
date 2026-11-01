/* TC
 * id:    pfm_3_neg_scprintf_bad_conv_probe
 * area:  stdio_printf
 * funcs: _scprintf _vscprintf _scwprintf _vscwprintf
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs printf format specification (an invalid type character is an invalid parameter since VS2005)
 * note:  OBSERVED: "ab%ycd" is counted as 5 (the bad conversion character is copied through) and "ab%" as 2; no errno, no handler call.
 *        Observed on Win10 22H2 19045 ucrtbase: the non-_s printf family does not validate format strings (no handler call, no errno); the bad conversion char is copied through.
 *        A (test bug): scprintf/vscprintf/scwprintf/vscwprintf("ab%ycd") -> 5, ("ab%") -> 2.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

static const char *volatile bad = "ab%ycd", *volatile trail = "ab%";
static const wchar_t *volatile wbad = L"ab%ycd", *volatile wtrail = L"ab%";
static int vsc(const char *f, ...) { int r; va_list ap; va_start(ap, f); r = _vscprintf(f, ap); va_end(ap); return r; }
static int vscw(const wchar_t *f, ...) { int r; va_list ap; va_start(ap, f); r = _vscwprintf(f, ap); va_end(ap); return r; }

#define BEGIN tc_iph_count = 0; errno = 0
#define EXPECT_LEN(r, n) do { CHECK_EQ_I(r, n); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); } while (0)

TC_CASE(scprintf_y)   { int r; BEGIN; r = _scprintf(bad, 1); EXPECT_LEN(r, 5); }
TC_CASE(vscprintf_y)  { int r; BEGIN; r = vsc(bad, 1); EXPECT_LEN(r, 5); }
TC_CASE(scwprintf_y)  { int r; BEGIN; r = _scwprintf(wbad, 1); EXPECT_LEN(r, 5); }
TC_CASE(vscwprintf_y) { int r; BEGIN; r = vscw(wbad, 1); EXPECT_LEN(r, 5); }
TC_CASE(scprintf_trailing_percent)  { int r; BEGIN; r = _scprintf(trail); EXPECT_LEN(r, 2); }
TC_CASE(scwprintf_trailing_percent) { int r; BEGIN; r = _scwprintf(wtrail); EXPECT_LEN(r, 2); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(scprintf_y); TC_RUN(vscprintf_y); TC_RUN(scwprintf_y); TC_RUN(vscwprintf_y);
    TC_RUN(scprintf_trailing_percent); TC_RUN(scwprintf_trailing_percent);
    return tc_end();
}
