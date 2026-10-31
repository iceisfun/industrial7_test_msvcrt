/* TC
 * id:    err_neg_probe_tmpnam_s_range_buf_state
 * area:  errno_param
 * kind:  negative
 * funcs: tmpnam_s _wtmpnam_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * conf:  probe
 * ref:   MS docs tmpnam_s tmpnam_s error table (contents of str not modified on ERANGE)
 * note:  PROBE: after tmpnam_s(buf, 1) fails with ERANGE the buffer is expected to be left UNMODIFIED (buf[0] stays 'q'): the MS docs error table lists 'contents of str: not modified' for ERANGE, unlike most secure string functions which write an empty string. Also probes that size 0 yields ERANGE (docs: 'too short') rather than EINVAL.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

static volatile size_t v_sz = 1;


TC_CASE(narrow) { char b[32]; int r; b[0] = 'q'; tc_iph_count = 0; v_sz = 1; r = tmpnam_s(b, v_sz); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(b[0], 'q'); }
TC_CASE(zero_size) { char b[32]; int r; b[0] = 'q'; v_sz = 0; r = tmpnam_s(b, v_sz); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(b[0], 'q'); }
TC_CASE(wide)   { wchar_t b[32]; int r; b[0] = L'q'; v_sz = 1; r = _wtmpnam_s(b, v_sz); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(b[0], L'q'); }

int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(narrow); TC_RUN(zero_size); TC_RUN(wide);
    return tc_end();
}
