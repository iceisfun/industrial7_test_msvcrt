/* TC
 * id:    cv_wcstol_wide_probe
 * area:  conv
 * funcs: wcstol
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   (none; behaviour of UCRT parse_digit / iswspace)
 * note:  PROBE: (1) UCRT wcstol accepts fullwidth digits U+FF11 as digit values (recollection of strtox parse_digit handling
 *        the fullwidth forms block); (2) leading U+2003 EM SPACE is skipped (iswspace on wide chars is table based and
 *        locale independent); (3) U+00A0 is NOT skipped. Each is its own case so a wrong guess is isolated.
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>

TC_CASE(fullwidth_digit) { const wchar_t *volatile s = L"\xFF11"; wchar_t *e = NULL; CHECK_EQ_I(wcstol(s, &e, 10), 1); CHECK_EQ_P(e, s + 1); }
TC_CASE(em_space_skipped) { const wchar_t *volatile s = L"\x2003" L"5"; wchar_t *e = NULL; CHECK_EQ_I(wcstol(s, &e, 10), 5); CHECK_EQ_P(e, s + 2); }
TC_CASE(nbsp_not_skipped) { const wchar_t *volatile s = L"\x00A0" L"5"; wchar_t *e = NULL; CHECK_EQ_I(wcstol(s, &e, 10), 0); CHECK_EQ_P(e, s); }

int main(void) { tc_begin(); TC_RUN(fullwidth_digit); TC_RUN(em_space_skipped); TC_RUN(nbsp_not_skipped); return tc_end(); }
