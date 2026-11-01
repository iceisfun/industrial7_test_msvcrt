/* TC
 * id:    scn_neg_sscanf_s_size_wider_than_data_probe
 * area:  scanf
 * kind:  negative
 * funcs: sscanf_s
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   MS docs scanf_s: the size argument is the buffer size in characters
 * note:  PROBE. Does the secure-scanf size check compare the buffer size with the field WIDTH, or with the characters actually
 *        stored? Expected (most likely): the characters actually stored, i.e. %10s size 4 on "ab" succeeds (ret 1, no handler);
 *        %3s size 3 on "abcdef" needs 4 chars (3 + NUL) -> handler, ret 0.
 *        CORRECTED for Win10 22H2 ucrtbase (observed 2026-10-31): a too-small _s destination returns 0 with errno=ENOMEM (12) (not ERANGE) and does NOT call the invalid parameter handler (count 0).
 *        MS docs do not document either. Stable (2 runs).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile vs;
TC_CASE(width10_size4_short_data)
{
    char a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = 'Z';
    CHECK(tc_iph_install()); tc_iph_count = 0; vs = "ab";
    n = sscanf_s(vs, "%10s", a, (unsigned)4);
    CHECK_EQ_I(n, 1); CHECK_STR(a, "ab"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(width3_size3_long_data)
{
    char a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = 'Z';
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0; vs = "abcdef";
    n = sscanf_s(vs, "%3s", a, (unsigned)3);
    CHECK_EQ_I(n, 0); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, ENOMEM); CHECK_EQ_I(a[3], 'Z');
}
TC_CASE(width_eq_size_minus1_control)
{
    char a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = 'Z';
    CHECK(tc_iph_install()); tc_iph_count = 0; vs = "abcdef";
    n = sscanf_s(vs, "%2s", a, (unsigned)3);
    CHECK_EQ_I(n, 1); CHECK_STR(a, "ab"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(width10_size4_short_data); TC_RUN(width3_size3_long_data); TC_RUN(width_eq_size_minus1_control); return tc_end(); }
