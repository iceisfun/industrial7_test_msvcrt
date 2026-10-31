/* TC
 * id:    ucr_strtoimax_wcstoimax
 * area:  ucrt_only
 * funcs: strtoimax wcstoimax wcstoll
 * crt:   win10
 * tier:  p1
 * conf:  standard
 * ref:   C99 7.8.2.3, 7.22.1.4; MS docs strtoimax, wcstoimax
 */
#include "tc.h"
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <wchar.h>

static const char *volatile v;
static const wchar_t *volatile w;
TC_CASE(narrow) { char *e; v = "-1234567890123"; CHECK_EQ_I(strtoimax((const char *)v, &e, 10), -1234567890123LL); CHECK_EQ_P(e, v + 14);
  v = "0xFF"; CHECK_EQ_I(strtoimax((const char *)v, &e, 16), 255); CHECK_EQ_P(e, v + 4); }
TC_CASE(wide) { wchar_t *e; w = L"-1234567890123"; CHECK_EQ_I(wcstoimax((const wchar_t *)w, &e, 10), -1234567890123LL); CHECK_EQ_P(e, w + 14);
  w = L"0xFF"; CHECK_EQ_I(wcstoimax((const wchar_t *)w, &e, 16), 255); }
TC_CASE(wcstoll_same) { wchar_t *e; w = L"9223372036854775807"; CHECK_EQ_I(wcstoll((const wchar_t *)w, &e, 10), INTMAX_MAX); CHECK_EQ_I(wcstoimax((const wchar_t *)w, &e, 10), INTMAX_MAX); }
TC_CASE(overflow) { char *e; wchar_t *we; v = "9223372036854775808"; errno = 0; CHECK_EQ_I(strtoimax((const char *)v, &e, 10), INTMAX_MAX); CHECK_EQ_I(errno, ERANGE);
  w = L"-9223372036854775809"; errno = 0; CHECK_EQ_I(wcstoimax((const wchar_t *)w, &we, 10), INTMAX_MIN); CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); TC_RUN(narrow); TC_RUN(wide); TC_RUN(wcstoll_same); TC_RUN(overflow); return tc_end(); }
