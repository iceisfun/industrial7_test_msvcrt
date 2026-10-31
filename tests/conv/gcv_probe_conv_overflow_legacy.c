/* TC
 * id:    gcv_probe_conv_overflow_legacy
 * area:  conv
 * funcs: _wtoi64_l _wtol_l _wtoi_l
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   probe, MS docs atoi
 * note:  PROBE: what legacy CRTs return on out-of-range input to _wtoi64_l/_wtol_l/_wtoi_l. Asserts saturation (the documented/ucrt behaviour); old CRTs may instead wrap. Real-Windows result decides.
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

TC_CASE(i64_sat) { const wchar_t *volatile s = L"9223372036854775808"; const wchar_t *volatile n = L"-9223372036854775809";
    CHECK_EQ_I(_wtoi64_l(s, NULL), 0x7fffffffffffffffLL); CHECK_EQ_I(_wtoi64_l(n, NULL), -0x7fffffffffffffffLL - 1); }
TC_CASE(long_sat) { const wchar_t *volatile s = L"4294967296"; const wchar_t *volatile n = L"-4294967297";
    CHECK_EQ_I(_wtol_l(s, NULL), LONG_MAX); CHECK_EQ_I(_wtol_l(n, NULL), LONG_MIN); }
TC_CASE(int_sat) { const wchar_t *volatile s = L"2147483648"; const wchar_t *volatile n = L"-2147483649";
    CHECK_EQ_I(_wtoi_l(s, NULL), INT_MAX); CHECK_EQ_I(_wtoi_l(n, NULL), INT_MIN); }

int main(void) { tc_begin(); TC_RUN(i64_sat); TC_RUN(long_sat); TC_RUN(int_sat); return tc_end(); }
