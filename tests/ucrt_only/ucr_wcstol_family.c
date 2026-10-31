/* TC
 * id:    ucr_wcstol_family
 * area:  ucrt_only
 * funcs: wcstol wcstoul
 * crt:   all
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.29.4.1.2; MS docs wcstol, wcstoul
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <wchar.h>

static const wchar_t *volatile w;
TC_CASE(bases) { wchar_t *e; w = L"0x1f"; CHECK_EQ_I(wcstol((const wchar_t *)w, &e, 16), 31); CHECK_EQ_P(e, w + 4);
  w = L"0x1f"; CHECK_EQ_I(wcstol((const wchar_t *)w, &e, 0), 31); w = L"077"; CHECK_EQ_I(wcstol((const wchar_t *)w, &e, 0), 63);
  w = L"101"; CHECK_EQ_I(wcstol((const wchar_t *)w, &e, 2), 5); w = L"zz"; CHECK_EQ_U(wcstoul((const wchar_t *)w, &e, 36), 1295); }
TC_CASE(overflow) { wchar_t *e; w = L"2147483648"; errno = 0; CHECK_EQ_I(wcstol((const wchar_t *)w, &e, 10), LONG_MAX); CHECK_EQ_I(errno, ERANGE);
  w = L"-2147483649"; errno = 0; CHECK_EQ_I(wcstol((const wchar_t *)w, &e, 10), LONG_MIN); CHECK_EQ_I(errno, ERANGE);
  w = L"4294967296"; errno = 0; CHECK_EQ_U(wcstoul((const wchar_t *)w, &e, 10), ULONG_MAX); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(no_digits) { wchar_t *e; w = L"xyz"; CHECK_EQ_I(wcstol((const wchar_t *)w, &e, 10), 0); CHECK_EQ_P(e, w);
  w = L""; CHECK_EQ_U(wcstoul((const wchar_t *)w, &e, 10), 0); CHECK_EQ_P(e, w); }
TC_CASE(whitespace) { wchar_t *e; w = L" \t\n42rest"; CHECK_EQ_I(wcstol((const wchar_t *)w, &e, 10), 42); CHECK_EQ_P(e, w + 5);
  w = L" \t-7"; CHECK_EQ_I(wcstol((const wchar_t *)w, &e, 10), -7); }

int main(void) { tc_begin(); TC_RUN(bases); TC_RUN(overflow); TC_RUN(no_digits); TC_RUN(whitespace); return tc_end(); }
