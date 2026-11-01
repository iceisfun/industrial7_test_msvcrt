/* TC
 * id:    scn_neg_snscanf_float_exp_cut_probe
 * area:  scanf
 * kind:  negative
 * funcs: _snscanf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * conf:  observed
 * ref:   C11 7.21.6.2p9-10 (a bare "1.5e" / "1.5e+" is not a valid floating sequence)
 * note:  Probes _snscanf %lf when count ends in the middle of the exponent ("1.5e", "1.5e+"). Believed: the MS CRT still converts the valid prefix 1.5 and returns 1 (it does not report a matching failure).
 *        Win10 22H2 ucrtbase (observed 2026-10-31, stable over 2 runs): input cut inside an incomplete exponent ("1.5e", "1.5e+") gives r==EOF and leaves the double untouched (99.0); the old expectation r==1,d==1.5 was wrong.
 *        A cut after an exponent digit ("1.5e+1") converts normally (15.0). Conforms to C11 7.21.6.2p9 and EXAMPLE p20 (a dangling exponent is not a matching sequence; '100ergs' with %f fails); Windows returns EOF (not 0) when the input ends inside the item. Class A: the old belief was a legacy-msvcrt reading.
 *        Gated: the observed values apply to ucrtbase only (#ifdef TC_UCRT); other CRTs keep the original (unverified, probe) expectation until they are run in the VM.
 */
#include "tc.h"
#include <stdio.h>

static volatile size_t cnt;

#ifdef TC_UCRT
TC_CASE(cut_after_e)    { double d = 99.0; int r; cnt = 4; r = _snscanf("1.5e+10", cnt, "%lf", &d); CHECK_EQ_I(r, EOF); CHECK_BITS_D(d, 99.0); }
TC_CASE(cut_after_sign) { double d = 99.0; int r; cnt = 5; r = _snscanf("1.5e+10", cnt, "%lf", &d); CHECK_EQ_I(r, EOF); CHECK_BITS_D(d, 99.0); }
TC_CASE(cut_after_exp_digit) { double d = 99.0; int r; cnt = 6; r = _snscanf("1.5e+10", cnt, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 15.0); }
#else
TC_CASE(cut_after_e)    { double d = 99.0; int r; cnt = 4; r = _snscanf("1.5e+10", cnt, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 1.5); }
TC_CASE(cut_after_sign) { double d = 99.0; int r; cnt = 5; r = _snscanf("1.5e+10", cnt, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 1.5); }
#endif

#ifdef TC_UCRT
int main(void) { tc_begin(); TC_RUN(cut_after_e); TC_RUN(cut_after_sign); TC_RUN(cut_after_exp_digit); return tc_end(); }
#else
int main(void) { tc_begin(); TC_RUN(cut_after_e); TC_RUN(cut_after_sign); return tc_end(); }
#endif
