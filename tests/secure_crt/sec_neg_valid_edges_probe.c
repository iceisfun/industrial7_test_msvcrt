/* TC
 * id:    sec_neg_valid_edges_probe
 * area:  secure_crt
 * funcs: strncpy_s getenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strncpy_s, getenv_s (do not spell out these edge cases)
 * note:  Probes (a) strncpy_s with count 0 and valid buffers returns 0, writes dest[0]=0 and does not call the handler;
 *        (b) getenv_s on an unset variable returns 0, *required=0, buffer[0]=0 and does not call the handler.
 *        Behaviour from implementation memory, not from the docs text.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <string.h>
#include <stdlib.h>

static volatile size_t vz = 0;

TC_CASE(strncpy_s_zero_count)
{
    char d[4] = "zzz"; int r; tc_iph_count = 0;
    r = strncpy_s(d, 4, "abc", vz);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(getenv_s_absent_variable)
{
    char b[8] = "zzz"; size_t len = 99; int r; tc_iph_count = 0;
    r = getenv_s(&len, b, 8, "TC_SEC_SURELY_NOT_SET_VAR_12345");
    CHECK_EQ_I(r, 0); CHECK_EQ_U(len, 0); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 0);
}

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no handler api\n"); return 1; }
    TC_RUN(strncpy_s_zero_count); TC_RUN(getenv_s_absent_variable);
    return tc_end();
}
