/* TC
 * id:    cv_neg_wtoi64_clamp_probe
 * area:  conv
 * funcs: _wtoi64 _atoi64
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _atoi64, _wtoi64 (overflow result not spelled out)
 * note:  PROBES: _atoi64/_wtoi64 saturate to LLONG_MAX / LLONG_MIN on 20-digit overflow (like _strtoi64) rather than wrapping.
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

static const wchar_t *volatile v_w;
static const char *volatile v_s;

TC_CASE(wide)
{
    v_w = L"99999999999999999999"; CHECK_EQ_I(_wtoi64(v_w), LLONG_MAX);
    v_w = L"-99999999999999999999"; CHECK_EQ_I(_wtoi64(v_w), LLONG_MIN);
}
TC_CASE(narrow)
{
    v_s = "99999999999999999999"; CHECK_EQ_I(_atoi64(v_s), LLONG_MAX);
    v_s = "-99999999999999999999"; CHECK_EQ_I(_atoi64(v_s), LLONG_MIN);
}

int main(void) { tc_begin(); TC_RUN(wide); TC_RUN(narrow); return tc_end(); }
