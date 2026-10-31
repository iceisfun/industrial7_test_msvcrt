/* TC
 * id:    pfl_neg_sprintf_l_null_format
 * area:  stdio_printf
 * funcs: _sprintf_l _printf_l _fprintf_l _vsprintf_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs printf/sprintf/fprintf parameter validation (NULL format or stream: invalid parameter handler, -1, EINVAL)
 * note:  NULL output buffer for _sprintf_l is probed in pfl_neg_sprintf_l_null_buf_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <locale.h>

static const char * volatile v_c = "C";
static const char * volatile v_nullfmt = NULL;
static int have_iph;

static int call_vsprintf_l(char *b, const char *f, _locale_t l, ...)
{ va_list ap; int r; va_start(ap, l); r = _vsprintf_l(b, f, l, ap); va_end(ap); return r; }

#define SETUP() char b[16]; _locale_t l = _create_locale(LC_ALL, v_c); int r; \
    if (!l || !have_iph) TC_SKIP("setup"); tc_iph_count = 0; errno = 0; b[0] = 'X'; (void)b
#define EXPECT_FAIL() do { CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); _free_locale(l); } while (0)

TC_CASE(sprintf_l_null_fmt) { SETUP(); r = _sprintf_l(b, v_nullfmt, l); EXPECT_FAIL(); }
TC_CASE(printf_l_null_fmt) { SETUP(); r = _printf_l(v_nullfmt, l); EXPECT_FAIL(); }
TC_CASE(fprintf_l_null_stream) { SETUP(); r = _fprintf_l(NULL, "abc", l); EXPECT_FAIL(); }
TC_CASE(fprintf_l_null_fmt) { SETUP(); r = _fprintf_l(stdout, v_nullfmt, l); EXPECT_FAIL(); }
TC_CASE(vsprintf_l_null_fmt) { SETUP(); r = call_vsprintf_l(b, v_nullfmt, l); EXPECT_FAIL(); }

int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
    TC_RUN(sprintf_l_null_fmt); TC_RUN(printf_l_null_fmt);
    TC_RUN(fprintf_l_null_stream); TC_RUN(fprintf_l_null_fmt); TC_RUN(vsprintf_l_null_fmt);
    return tc_end();
}
