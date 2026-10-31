/* TC
 * id:    dat_neg_fmode_set_combo_zero_probe
 * area:  data_exports
 * funcs: _set_fmode _get_fmode
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _set_fmode
 * note:  PROBE: validation is by equality with _O_TEXT/_O_BINARY (UCRT also _O_WTEXT/_O_U8TEXT/_O_U16TEXT), so 0 and combos are rejected.
 */
#include "tc.h"
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

static int cur(void) { int m = -99; _get_fmode(&m); return m; }
static void bad(int mode)
{
    int before = cur(), c = tc_iph_count, r;
    errno = 0; r = _set_fmode(mode);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, c + 1); CHECK_EQ_I(cur(), before);
}
static int orig;
TC_CASE(zero)            { bad(0); }
TC_CASE(text_or_binary)  { bad(_O_TEXT | _O_BINARY); }
TC_CASE(text_noinherit)  { bad(_O_TEXT | _O_NOINHERIT); }

int main(void)
{
    tc_begin(); tc_iph_install();
    orig = cur();
    TC_RUN(zero); TC_RUN(text_or_binary); TC_RUN(text_noinherit);
    _set_fmode(orig);
    return tc_end();
}
