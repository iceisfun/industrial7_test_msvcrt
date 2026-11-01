/* TC
 * id:    pfl_probe_snprintf_l_count_zero
 * area:  stdio_printf
 * funcs: _snprintf_l _create_locale _free_locale
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _snprintf, _vsnprintf (return value when buffer is NULL and count is 0)
 * note:  OBSERVED Win10 22H2 19045 ucrtbase (run twice): _snprintf_l(NULL, 0, "abc%d", l, 42) returns 5, the needed length (the old guess -1 was wrong); a non-NULL
 *        buffer with count 0 is left untouched and returns -1. Pre-ucrt CRTs excluded.
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

static const char * volatile v_c = "C";

TC_CASE(null_buf_count0_returns_needed_len)
{
    _locale_t l = _create_locale(LC_ALL, v_c);
    if (!l) TC_SKIP("no C locale");
    CHECK_EQ_I(_snprintf_l(NULL, 0, "abc%d", l, 42), 5);
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

int main(void) { tc_begin(); TC_RUN(null_buf_count0_returns_needed_len); TC_RUN(nonnull_buf_count0_writes_nothing); return tc_end(); }
