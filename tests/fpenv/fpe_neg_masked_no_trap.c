/* TC
 * id:    fpe_neg_masked_no_trap
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp _clearfp _statusfp
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _controlfp, _clearfp, _statusfp; C11 F.9 / Annex F default (masked) exception handling
 * note:  With all exceptions masked, invalid operations must not raise an SEH exception; they set status flags instead.
 */
#include "tc.h"
#include <float.h>

/* Microsoft float.h values (equal to the _EM_* bits) */
#define SW_INVALID 0x10
#define SW_ZERODIVIDE 0x08
#define SW_OVERFLOW 0x04
static volatile double v_zero = 0.0, v_one = 1.0, v_max = 1.7976931348623157e308, v_two = 2.0;
static volatile double sink;

static void mask_all(void) { _controlfp(_MCW_EM, _MCW_EM); _clearfp(); }

TC_CASE(div_by_zero_inf)
{
    double r; mask_all();
    r = v_one / v_zero;
    CHECK_BITS_D(r, __builtin_inf());
    r = -v_one / v_zero;
    CHECK_BITS_D(r, -__builtin_inf());
}
TC_CASE(zero_over_zero_nan)
{
    double r; mask_all();
    r = v_zero / v_zero;
    CHECK_IS_NAN(r);
}
TC_CASE(overflow_inf)
{
    double r; mask_all();
    r = v_max * v_two;
    CHECK_BITS_D(r, __builtin_inf());
}
TC_CASE(status_flags_set)
{
    unsigned s;
    mask_all(); sink = v_one / v_zero; s = _statusfp();
    CHECK((s & SW_ZERODIVIDE) != 0);
    _clearfp(); sink = v_zero / v_zero; s = _statusfp();
    CHECK((s & SW_INVALID) != 0);
    _clearfp(); sink = v_max * v_two; s = _statusfp();
    CHECK((s & SW_OVERFLOW) != 0);
    CHECK((s & SW_ZERODIVIDE) == 0);
    _clearfp();
}
TC_CASE(mask_bits_readback)
{
    unsigned c;
    mask_all(); c = _controlfp(0, 0);
    CHECK((c & _MCW_EM) == _MCW_EM);
}
int main(void)
{
    tc_begin();
    TC_RUN(div_by_zero_inf); TC_RUN(zero_over_zero_nan); TC_RUN(overflow_inf);
    TC_RUN(status_flags_set); TC_RUN(mask_bits_readback);
    return tc_end();
}
