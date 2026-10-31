/* TC
 * id:    gsm_neg_strlwr_s_state_probe
 * area:  string
 * kind:  negative
 * funcs: _strlwr_s _strupr_s
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _strlwr_s (secure functions generally null the first char on failure - not stated for these)
 * note:  Probes (a) buf[0]=='\0' after _strlwr_s/_strupr_s fail on an unterminated buffer, (b) (NULL,0) is treated as NULL str: EINVAL with the handler invoked (NULL str is documented as EINVAL for any size; this probes that size 0 does not make it a no-op).
 *        Each guess is in its own case.
 */
#include "tc.h"
#define MINGW_HAS_SECURE_API 1
#include <string.h>
#include <errno.h>

TC_CASE(unterminated_first_char_nulled)
{
    char b[8]; tc_iph_count = 0;
    memcpy(b, "ABCDEFG", 8); (void)_strlwr_s(b, 3); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
    memcpy(b, "abcdefg", 8); (void)_strupr_s(b, 3); CHECK_EQ_I(tc_iph_count, 2); CHECK_EQ_I(b[0], 0);
}
TC_CASE(null_and_zero_size)
{
    char *volatile p = NULL; tc_iph_count = 0;
    CHECK_EQ_I(_strlwr_s(p, 0), EINVAL); CHECK_EQ_I(_strupr_s(p, 0), EINVAL); CHECK_EQ_I(tc_iph_count, 2);
}
int main(void) { tc_begin(); if (!tc_iph_install()) return 3; TC_RUN(unterminated_first_char_nulled); TC_RUN(null_and_zero_size); return tc_end(); }
