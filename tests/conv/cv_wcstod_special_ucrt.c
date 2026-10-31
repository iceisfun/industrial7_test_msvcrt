/* TC
 * id:    cv_wcstod_special_ucrt
 * area:  conv
 * funcs: wcstod
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.4.1.1 / 7.22.1.3 (INF, INFINITY, NAN, hex floating); MS docs wcstod (UCRT)
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>

static double W(const wchar_t *volatile s, wchar_t **e) { return wcstod(s, e); }
static int isinf_pos(double d) { return d > 1.7976931348623157e308; }
static int isinf_neg(double d) { return d < -1.7976931348623157e308; }

TC_CASE(inf) { wchar_t *e; const wchar_t *s = L"inf"; double d = W(s, &e); CHECK(isinf_pos(d)); CHECK_EQ_I(e - s, 3); }
TC_CASE(neg_infinity) { wchar_t *e; const wchar_t *s = L"-infinity"; double d = W(s, &e); CHECK(isinf_neg(d)); CHECK_EQ_I(e - s, 9); }
TC_CASE(nan) { wchar_t *e; const wchar_t *s = L"nan"; double d = W(s, &e); CHECK_IS_NAN(d); CHECK_EQ_I(e - s, 3); }
TC_CASE(hex) { wchar_t *e; const wchar_t *s = L"0x1p3"; double d = W(s, &e); CHECK_BITS_D(d, 8.0); CHECK_EQ_I(e - s, 5); }
TC_CASE(hex_frac) { wchar_t *e; double d = W(L"0x1.8p1", &e); CHECK_BITS_D(d, 3.0); }

int main(void) { tc_begin(); TC_RUN(inf); TC_RUN(neg_infinity); TC_RUN(nan); TC_RUN(hex); TC_RUN(hex_frac); return tc_end(); }
