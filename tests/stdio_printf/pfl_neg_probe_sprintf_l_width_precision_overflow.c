/* TC
 * id:    pfl_neg_probe_sprintf_l_width_precision_overflow
 * area:  stdio_printf
 * funcs: _sprintf_l _snprintf_l _create_locale _free_locale
 * crt:   win10
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf width specification (width is a nonnegative decimal integer)
 * note:  PROBE: a width or precision literal that does not fit in int (2147483648, 99999999999) is treated by ucrt as an
 *        invalid format (args are ints so a lenient parse cannot crash): invalid-parameter handler runs once, errno == EINVAL, return -1. All uses go through _snprintf_l
 *        with a 16-byte count so a lenient implementation cannot overrun. Only the no-overrun guard and the
 *        subsequent valid width-5 call are expected to hold under any behaviour.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <locale.h>

static const char * volatile v_c = "C";
static volatile const char *v_fmt;
static int have_iph;

static void overflow(const char *fmt, int expect_probe)
{
    char b[32]; _locale_t l = _create_locale(LC_ALL, v_c); int r, i;
    if (!l || !have_iph) TC_SKIP("setup");
    memset(b, 'G', sizeof b);
    v_fmt = fmt; tc_iph_count = 0; errno = 0;
    r = _snprintf_l(b, 16, (const char *)v_fmt, l, 1, "abc");
    for (i = 16; i < 32; i++) CHECK_EQ_I(b[i], 'G');
    if (expect_probe) { CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
    _free_locale(l);
}
TC_CASE(width_2147483648) { overflow("%2147483648d", 1); }
TC_CASE(precision_2147483648) { overflow("%.2147483648d", 1); }
TC_CASE(width_11_digits) { overflow("%99999999999d", 1); }
TC_CASE(valid_width_after)
{
    char b[32]; _locale_t l = _create_locale(LC_ALL, v_c); int r;
    if (!l || !have_iph) TC_SKIP("setup");
    tc_iph_count = 0;
    r = _sprintf_l(b, "%5d", l, 42);
    CHECK_EQ_I(r, 5); CHECK_STR(b, "   42"); CHECK_EQ_I(tc_iph_count, 0);
    _free_locale(l);
}

int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
    TC_RUN(width_2147483648); TC_RUN(precision_2147483648); TC_RUN(width_11_digits); TC_RUN(valid_width_after);
    return tc_end();
}
