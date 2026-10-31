/* TC
 * id:    scn_neg_sscanf_s_too_small_string
 * area:  scanf
 * funcs: sscanf_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs scanf_s: size argument is in characters and includes the terminating NUL; overflow invokes the invalid parameter handler
 * note:  Asserts only no-overflow (guard bytes past the stated size) and a failure return (< 1). Handler invocation, buffer state
 *        and errno are probed in scn_neg_sscanf_s_too_small_state_probe.
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile vin;
static volatile unsigned vsz;

static void fill(char *b) { int i; for (i = 0; i < 8; i++) b[i] = 'Z'; }

TC_CASE(six_into_four)
{
    char b[8]; int n, i; fill(b); vin = "abcdef"; vsz = 4;
    CHECK(tc_iph_install()); tc_iph_count = 0;
    n = sscanf_s(vin, "%s", b, vsz);
    CHECK(n < 1);
    for (i = 4; i < 8; i++) CHECK_EQ_I(b[i], 'Z');
}
TC_CASE(three_no_room_for_nul)
{
    char b[8]; int n, i; fill(b); vin = "abc"; vsz = 3;
    CHECK(tc_iph_install()); tc_iph_count = 0;
    n = sscanf_s(vin, "%s", b, vsz);
    CHECK(n < 1);
    for (i = 3; i < 8; i++) CHECK_EQ_I(b[i], 'Z');
}
TC_CASE(three_fits_in_four)
{
    char b[8]; int n; fill(b); vin = "abc"; vsz = 4;
    CHECK(tc_iph_install()); tc_iph_count = 0;
    n = sscanf_s(vin, "%s", b, vsz);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(n, 1); CHECK_STR(b, "abc"); CHECK_EQ_I(b[4], 'Z');
}
int main(void) { tc_begin(); TC_RUN(six_into_four); TC_RUN(three_no_room_for_nul); TC_RUN(three_fits_in_four); return tc_end(); }
