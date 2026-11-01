/* TC
 * id:    err_neg_probe_tmpnam_s_range_buf_state
 * area:  errno_param
 * kind:  negative
 * funcs: tmpnam_s _wtmpnam_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * conf:  observed
 * ref:   MS docs tmpnam_s tmpnam_s error table (contents of str not modified on ERANGE)
 * note:  OBSERVED on Win10 22H2 (19045): after tmpnam_s(buf,1)/_wtmpnam_s(buf,1) fail with ERANGE, buf[0] is 0 (empty string
 *        written), contrary to the MS docs error table ("not modified"). size 0 returns ERANGE and leaves the buffer untouched
 *        (nothing can be written). Docs deviate from Windows; stable.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

static volatile size_t v_sz = 1;


TC_CASE(narrow) { char b[32]; int r; b[0] = 'q'; tc_iph_count = 0; v_sz = 1; r = tmpnam_s(b, v_sz); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(b[0], 0); }   /* observed: buffer set to empty string */
TC_CASE(zero_size) { char b[32]; int r; b[0] = 'q'; v_sz = 0; r = tmpnam_s(b, v_sz); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(b[0], 'q'); }
TC_CASE(wide)   { wchar_t b[32]; int r; b[0] = L'q'; v_sz = 1; r = _wtmpnam_s(b, v_sz); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(b[0], 0); }   /* observed: buffer set to empty string */

int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(narrow); TC_RUN(zero_size); TC_RUN(wide);
    return tc_end();
}
