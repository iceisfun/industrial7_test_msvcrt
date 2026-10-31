/* TC
 * id:    cv_neg_wcstol_fullwidth_probe
 * area:  conv
 * funcs: wcstol
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wcstol (digits; C locale)
 * note:  Probes whether the fullwidth digit U+FF11 is accepted as digit 1 by UCRT wcstol (the CRT's wide digit parsing
 *        has historically mapped the fullwidth ranges U+FF10..FF19/FF21../FF41.. to values). Expected (best recollection,
 *        not documented): 1, endptr==nptr+1. The standard leaves this outside the C-locale digit set, so it is a pure probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>

static const wchar_t *volatile vw;
TC_CASE(fullwidth_one) { wchar_t *e; vw = L"\xFF11"; CHECK_EQ_I(wcstol(vw, &e, 10), 1); CHECK_EQ_P(e, vw + 1); }
int main(void) { tc_begin(); TC_RUN(fullwidth_one); return tc_end(); }
