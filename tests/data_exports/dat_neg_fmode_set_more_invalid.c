/* TC
 * id:    dat_neg_fmode_set_more_invalid
 * area:  data_exports
 * funcs: _set_fmode _get_fmode
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_fmode: only _O_TEXT, _O_BINARY (and _O_WTEXT etc. in UCRT) valid; else EINVAL + invalid parameter handler
 * note:  Only modes clearly invalid are used.
 */
#include "tc.h"
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>

static int cur(void) { int m = -99; _get_fmode(&m); return m; }
static void bad(int mode)
{
    int before = cur(), c = tc_iph_count, r;
    errno = 0; r = _set_fmode(mode);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, c + 1); CHECK_EQ_I(cur(), before);
}
static void good(int mode)
{
    int c = tc_iph_count, r = _set_fmode(mode);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, c); CHECK_EQ_I(cur(), mode);
}
static int orig;
TC_CASE(valid_controls) { good(_O_BINARY); good(_O_TEXT); }
TC_CASE(o_rdwr)  { bad(_O_RDWR); }
TC_CASE(o_creat) { bad(_O_CREAT); }
TC_CASE(int_max) { bad(0x7FFFFFFF); }
TC_CASE(int_min) { bad(INT_MIN); }

int main(void)
{
    tc_begin(); tc_iph_install();
    orig = cur();
    TC_RUN(valid_controls); TC_RUN(o_rdwr); TC_RUN(o_creat); TC_RUN(int_max); TC_RUN(int_min);
    _set_fmode(orig);
    return tc_end();
}
