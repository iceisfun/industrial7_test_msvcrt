/* TC
 * id:    pfl_probe_snprintf_l_count_zero
 * area:  stdio_printf
 * funcs: _snprintf_l _create_locale _free_locale
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _snprintf, _vsnprintf (return value when buffer is NULL and count is 0)
 * note:  PROBE: the legacy (non-C99) _snprintf family returns -1 whenever the output does not fit in count, including count 0, and
 *        unlike C99 snprintf does NOT report the needed length (_scprintf/_vscprintf exist for that). Probed: _snprintf_l(NULL, 0, ...)
 *        returns -1 (not the needed length) and a non-NULL buffer with count 0 is left untouched and returns -1. Most-likely value,
 *        unverified. Pre-ucrt CRTs excluded.
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

static const char * volatile v_c = "C";

TC_CASE(null_buf_count0_returns_minus1)
{
    _locale_t l = _create_locale(LC_ALL, v_c);
    if (!l) TC_SKIP("no C locale");
    CHECK_EQ_I(_snprintf_l(NULL, 0, "abc%d", l, 42), -1);
    _free_locale(l);
}
TC_CASE(nonnull_buf_count0_writes_nothing)
{
    char b[4] = { 'X', 'X', 'X', 'X' }; _locale_t l = _create_locale(LC_ALL, v_c); int n;
    if (!l) TC_SKIP("no C locale");
    n = _snprintf_l(b, 0, "abc", l);
    CHECK_MEM(b, "XXXX", 4);
    CHECK_EQ_I(n, -1);
    _free_locale(l);
}

int main(void) { tc_begin(); TC_RUN(null_buf_count0_returns_minus1); TC_RUN(nonnull_buf_count0_writes_nothing); return tc_end(); }
