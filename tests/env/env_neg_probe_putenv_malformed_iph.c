/* TC
 * id:    env_neg_probe_putenv_malformed_iph
 * area:  env
 * kind:  negative
 * funcs: _putenv _wputenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _putenv (return value/errno for malformed strings is covered by other tests)
 * note:  PROBE: number of invalid-parameter handler invocations for malformed strings (no '=', empty, leading '=').
 *        Expected 1 each (the CRT validates the string via _VALIDATE_RETURN). Return values are not asserted here.
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile s_noeq = "TCNOEQ";
static const char *volatile s_empty = "";
static const char *volatile s_leq = "=x";
static const wchar_t *volatile w_noeq = L"TCNOEQ";

TC_CASE(no_equals_handler_count) { int b = tc_iph_count; _putenv(s_noeq); CHECK_EQ_I(tc_iph_count - b, 1); }
TC_CASE(empty_string_handler_count) { int b = tc_iph_count; _putenv(s_empty); CHECK_EQ_I(tc_iph_count - b, 1); }
TC_CASE(leading_equals_handler_count) { int b = tc_iph_count; _putenv(s_leq); CHECK_EQ_I(tc_iph_count - b, 1); }
TC_CASE(wide_no_equals_handler_count) { int b = tc_iph_count; _wputenv(w_noeq); CHECK_EQ_I(tc_iph_count - b, 1); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP " TC_TEST_ID " no invalid parameter handler API\n"); return tc_end(); }
    TC_RUN(no_equals_handler_count); TC_RUN(empty_string_handler_count);
    TC_RUN(leading_equals_handler_count); TC_RUN(wide_no_equals_handler_count);
    return tc_end();
}
