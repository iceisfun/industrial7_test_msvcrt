/* TC
 * id:    ct_neg_probe_isx_out_of_range
 * area:  ctype
 * funcs: isalpha isdigit isspace ispunct iscntrl isxdigit isupper islower
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs isalpha (behavior undefined unless c is EOF or 0..255 in debug CRT)
 * note:  PROBE (ucrtbase only; msvcrt..msvcr120 index their ctype table directly and an out-of-range read is UB/possible crash): in the release CRT, is* with ints outside -1..255 (-2, 256, 0x1234, -1000) in the C locale return 0 without crashing. Our own invalid-parameter handler is installed but its count is not asserted.
 */
#include "tc.h"
#include <ctype.h>

static volatile int bad[] = {-2, 256, 0x1234, -1000, 100000};
#define N ((int)(sizeof bad / sizeof bad[0]))

TC_CASE(out_of_range_is_zero)
{
    int i;
    tc_iph_install();
    for (i = 0; i < N; i++) {
        int c = bad[i];
        CHECK_EQ_I(isalpha(c), 0); CHECK_EQ_I(isdigit(c), 0); CHECK_EQ_I(isspace(c), 0); CHECK_EQ_I(ispunct(c), 0);
        CHECK_EQ_I(iscntrl(c), 0); CHECK_EQ_I(isxdigit(c), 0); CHECK_EQ_I(isupper(c), 0); CHECK_EQ_I(islower(c), 0);
    }
}
int main(void) { tc_begin(); TC_RUN(out_of_range_is_zero); return tc_end(); }
