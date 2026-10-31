/* TC
 * id:    cv_neg_strtod_inf_nan_partial_probe
 * area:  conv
 * funcs: strtod wcstod strtof
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.1.3
 * note:  Probes UCRT handling of incomplete forms: "infinit" -> inf with endptr s+3 (not a partial INFINITY match);
 *        "nan(" -> NaN with endptr s+3 (unterminated n-char-sequence not consumed); "nan(abc" same.
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>
#include <math.h>

static const char *volatile vs;
static const wchar_t *volatile vw;
TC_CASE(infinit) { char *e; double d; vs = "infinit"; d = strtod(vs, &e); CHECK_BITS_D(d, HUGE_VAL); CHECK_EQ_P(e, vs + 3); }
TC_CASE(nan_open_paren) { char *e; double d; vs = "nan("; d = strtod(vs, &e); CHECK_IS_NAN(d); CHECK_EQ_P(e, vs + 3); }
TC_CASE(nan_unterminated) { char *e; double d; vs = "nan(abc"; d = strtod(vs, &e); CHECK_IS_NAN(d); CHECK_EQ_P(e, vs + 3); }
TC_CASE(infi) { char *e; double d; vs = "infi"; d = strtod(vs, &e); CHECK_BITS_D(d, HUGE_VAL); CHECK_EQ_P(e, vs + 3); }
TC_CASE(infi_float) { char *e; float f; vs = "infi"; f = strtof(vs, &e); CHECK_BITS_F(f, HUGE_VALF); CHECK_EQ_P(e, vs + 3); }
TC_CASE(infi_wide) { wchar_t *e; double d; vw = L"infi"; d = wcstod(vw, &e); CHECK_BITS_D(d, HUGE_VAL); CHECK_EQ_P(e, vw + 3); }
int main(void) { tc_begin(); TC_RUN(infinit); TC_RUN(infi); TC_RUN(infi_float); TC_RUN(infi_wide); TC_RUN(nan_open_paren); TC_RUN(nan_unterminated); return tc_end(); }
