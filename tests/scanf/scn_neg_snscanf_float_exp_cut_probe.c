/* TC
 * id:    scn_neg_snscanf_float_exp_cut_probe
 * area:  scanf
 * kind:  negative
 * funcs: _snscanf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.2p9-10 (a bare "1.5e" / "1.5e+" is not a valid floating sequence)
 * note:  Probes _snscanf %lf when count ends in the middle of the exponent ("1.5e", "1.5e+"). Believed: the MS CRT still converts the valid prefix 1.5 and returns 1 (it does not report a matching failure).
 */
#include "tc.h"
#include <stdio.h>

static volatile size_t cnt;

TC_CASE(cut_after_e)    { double d = 99.0; int r; cnt = 4; r = _snscanf("1.5e+10", cnt, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 1.5); }
TC_CASE(cut_after_sign) { double d = 99.0; int r; cnt = 5; r = _snscanf("1.5e+10", cnt, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 1.5); }

int main(void) { tc_begin(); TC_RUN(cut_after_e); TC_RUN(cut_after_sign); return tc_end(); }
