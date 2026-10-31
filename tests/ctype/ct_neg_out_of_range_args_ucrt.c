/* TC
 * id:    ct_neg_out_of_range_args_ucrt
 * area:  ctype
 * kind:  negative
 * funcs: isalpha isdigit isspace ispunct isprint iscntrl isxdigit isalnum isgraph isupper islower
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.4p1 (undefined); MS docs isalpha ("values outside -1..255 are undefined" in debug, asserted)
 * note:  PROBES: the UCRT release build range-checks the table index and returns 0 for every is* class when c is outside
 *        -1..255 (no table over-read, no aliasing to the low byte). 0x141 would alias to 'A' if the low byte were used.
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;
typedef int (__cdecl *cf_t)(int);
static cf_t fns[] = { isalpha, isdigit, isspace, ispunct, isprint, iscntrl, isxdigit, isalnum, isgraph, isupper, islower };
#define NF (sizeof fns / sizeof fns[0])

static int nonzero(int c)
{
    size_t i; int bad = 0;
    for (i = 0; i < NF; i++) { vc = c; if (fns[i](vc) != 0) bad++; }
    return bad;
}
TC_CASE(c256_zero)  { CHECK_EQ_I(nonzero(256), 0); }
TC_CASE(c1000_zero) { CHECK_EQ_I(nonzero(1000), 0); }
TC_CASE(minus2_zero) { CHECK_EQ_I(nonzero(-2), 0); }
TC_CASE(minus128_zero) { CHECK_EQ_I(nonzero(-128), 0); }
TC_CASE(c0x1FF_zero_not_aliased_to_0xFF) { CHECK_EQ_I(nonzero(0x1FF), 0); }
TC_CASE(c0x141_zero_not_aliased_to_A) { CHECK_EQ_I(nonzero(0x141), 0); }

int main(void)
{
    tc_begin();
    TC_RUN(c256_zero); TC_RUN(c1000_zero); TC_RUN(minus2_zero); TC_RUN(minus128_zero);
    TC_RUN(c0x1FF_zero_not_aliased_to_0xFF); TC_RUN(c0x141_zero_not_aliased_to_A);
    return tc_end();
}
