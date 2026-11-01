/* TC
 * id:    env_neg_probe_putenv_malformed_iph
 * area:  env
 * kind:  negative
 * funcs: _putenv _wputenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _putenv (return value/errno for malformed strings is covered by other tests)
 * note:  PROBE: number of invalid-parameter handler invocations for malformed strings (no '=', empty, leading '=').
 *        Expected 1 each (the CRT validates the string via _VALIDATE_RETURN). Return values are not asserted here.
 *        Win10 22H2 19045 (observed): for "TCNOEQ", "", "=x" (and the wide form) _putenv/_wputenv return -1 with errno EINVAL but do NOT call the invalid
 *        parameter handler (delta 0).
 *        [unverified] on other CRTs: only ucrtbase 19045 was observed; the expectation is gated with TC_AT_LEAST(140) and the original expectation is kept for older CRTs.
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile s_noeq = "TCNOEQ";
static const char *volatile s_empty = "";
static const char *volatile s_leq = "=x";
static const wchar_t *volatile w_noeq = L"TCNOEQ";

TC_CASE(no_equals_handler_count) { int b = tc_iph_count; 
#if TC_AT_LEAST(140)
_putenv(s_noeq); CHECK_EQ_I(tc_iph_count - b, 0);
#else
_putenv(s_noeq); CHECK_EQ_I(tc_iph_count - b, 1);
#endif
 }
TC_CASE(empty_string_handler_count) { int b = tc_iph_count; 
#if TC_AT_LEAST(140)
_putenv(s_empty); CHECK_EQ_I(tc_iph_count - b, 0);
#else
_putenv(s_empty); CHECK_EQ_I(tc_iph_count - b, 1);
#endif
 }
TC_CASE(leading_equals_handler_count) { int b = tc_iph_count; 
#if TC_AT_LEAST(140)
_putenv(s_leq); CHECK_EQ_I(tc_iph_count - b, 0);
#else
_putenv(s_leq); CHECK_EQ_I(tc_iph_count - b, 1);
#endif
 }
TC_CASE(wide_no_equals_handler_count) { int b = tc_iph_count; 
#if TC_AT_LEAST(140)
_wputenv(w_noeq); CHECK_EQ_I(tc_iph_count - b, 0);
#else
_wputenv(w_noeq); CHECK_EQ_I(tc_iph_count - b, 1);
#endif
 }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP " TC_TEST_ID " no invalid parameter handler API\n"); return tc_end(); }
    TC_RUN(no_equals_handler_count); TC_RUN(empty_string_handler_count);
    TC_RUN(leading_equals_handler_count); TC_RUN(wide_no_equals_handler_count);
    return tc_end();
}
