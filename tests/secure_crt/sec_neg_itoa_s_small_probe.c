/* TC
 * id:    sec_neg_itoa_s_small_probe
 * area:  secure_crt
 * funcs: _itoa_s _ultoa_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _itoa_s, _ultoa_s
 * note:  PROBE: buffer too small for the digits. Docs table lists EINVAL for some small-buffer cases; most likely
 *        real behaviour is ERANGE, buffer[0]==0, invalid-parameter handler invoked once. Isolated from documented checks.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

static volatile int v_n = 12345, v_neg = -12;
static volatile unsigned long v_ul = 12345UL;

TC_CASE(itoa_s_12345_size3_ERANGE_buf_empty)
{
    char b[8]; int r;
    memset(b, 'x', sizeof b); tc_iph_count = 0; errno = 0;
    r = _itoa_s(v_n, b, 3, 10);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(b[0], 0);
}
TC_CASE(itoa_s_neg_needs_sign_slot)
{
    char b[8]; int r;   /* "-12" needs 3 chars + NUL = 4 */
    memset(b, 'x', sizeof b); errno = 0;
    r = _itoa_s(v_neg, b, 3, 10);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(b[0], 0);
}
TC_CASE(ultoa_s_size_equals_digits_no_room_for_nul)
{
    char b[8]; int r;
    memset(b, 'x', sizeof b); errno = 0;
    r = _ultoa_s(v_ul, b, 5, 10);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(b[0], 0);
}
TC_CASE(handler_called_once)
{
    char b[8];
    tc_iph_count = 0;
    _itoa_s(v_n, b, 3, 10);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(itoa_s_12345_size3_ERANGE_buf_empty); TC_RUN(itoa_s_neg_needs_sign_slot);
    TC_RUN(ultoa_s_size_equals_digits_no_room_for_nul); TC_RUN(handler_called_once);
    return tc_end();
}
