/* TC
 * id:    pfm_3_neg_scprintf_invalid_spec
 * area:  stdio_printf
 * funcs: _scprintf _vscprintf _scwprintf _vscwprintf
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _scprintf, _scwprintf, _vscprintf, _vscwprintf (format == NULL: invalid parameter handler, return -1, errno EINVAL)
 * note:  Only the NULL-format failure is asserted here. Invalid conversion characters ("%y") are probed in pfm_3_neg_scprintf_bad_conv_probe.
 *        Control case: a valid format returns the character count with no handler call.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

static const char *volatile nf = NULL;
static const wchar_t *volatile nwf = NULL;
static int vsc(const char *f, ...) { int r; va_list ap; va_start(ap, f); r = _vscprintf(f, ap); va_end(ap); return r; }
static int vscw(const wchar_t *f, ...) { int r; va_list ap; va_start(ap, f); r = _vscwprintf(f, ap); va_end(ap); return r; }

#define BEGIN tc_iph_count = 0; errno = 0
#define EXPECT_FAIL(r) do { CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(scprintf_null)   { int r; BEGIN; r = _scprintf(nf); EXPECT_FAIL(r); }
TC_CASE(vscprintf_null)  { int r; BEGIN; r = vsc(nf, 1); EXPECT_FAIL(r); }
TC_CASE(scwprintf_null)  { int r; BEGIN; r = _scwprintf(nwf); EXPECT_FAIL(r); }
TC_CASE(vscwprintf_null) { int r; BEGIN; r = vscw(nwf, 1); EXPECT_FAIL(r); }
TC_CASE(control_valid)
{
    const char * volatile f = "%d-%s"; const wchar_t * volatile w = L"%d-%hs"; int r;
    BEGIN;
    r = _scprintf(f, 12, "ab"); CHECK_EQ_I(r, 5);
    r = vsc(f, 12, "ab"); CHECK_EQ_I(r, 5);
    r = _scwprintf(w, 12, "ab"); CHECK_EQ_I(r, 5);
    r = vscw(w, 12, "ab"); CHECK_EQ_I(r, 5);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
}

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(scprintf_null); TC_RUN(vscprintf_null); TC_RUN(scwprintf_null); TC_RUN(vscwprintf_null); TC_RUN(control_valid);
    return tc_end();
}
