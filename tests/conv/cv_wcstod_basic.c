/* TC
 * id:    cv_wcstod_basic
 * area:  conv
 * funcs: wcstod
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.4.1.1; MS docs strtod, wcstod
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>
#include <errno.h>

static const wchar_t *volatile vs;

TC_CASE(simple) { vs = L"3.14"; CHECK_BITS_D(wcstod(vs, NULL), 3.14); vs = L"-2.5"; CHECK_BITS_D(wcstod(vs, NULL), -2.5); }
TC_CASE(exponent) { vs = L"1.5e3"; CHECK_BITS_D(wcstod(vs, NULL), 1500.0); vs = L"25E-1"; CHECK_BITS_D(wcstod(vs, NULL), 2.5); }
TC_CASE(exact_tenth) { vs = L"0.1"; CHECK_EQ_U(tc_dbits(wcstod(vs, NULL)), 0x3FB999999999999AULL); }
TC_CASE(endptr) { wchar_t *e = NULL; vs = L"6.25abc"; CHECK_BITS_D(wcstod(vs, &e), 6.25); CHECK_EQ_P(e, vs + 4); }
TC_CASE(no_digits) { wchar_t *e = NULL; vs = L"  abc"; CHECK_BITS_D(wcstod(vs, &e), 0.0); CHECK_EQ_P(e, vs); }
TC_CASE(whitespace) { vs = L" \t\r\n  8.5"; CHECK_BITS_D(wcstod(vs, NULL), 8.5); }
TC_CASE(overflow) { double d; errno = 0; vs = L"1e999"; d = wcstod(vs, NULL); CHECK_EQ_U(tc_dbits(d), 0x7FF0000000000000ULL); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(neg_overflow) { double d; errno = 0; vs = L"-1e999"; d = wcstod(vs, NULL); CHECK_EQ_U(tc_dbits(d), 0xFFF0000000000000ULL); CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); TC_RUN(simple); TC_RUN(exponent); TC_RUN(exact_tenth); TC_RUN(endptr); TC_RUN(no_digits); TC_RUN(whitespace); TC_RUN(overflow); TC_RUN(neg_overflow); return tc_end(); }
