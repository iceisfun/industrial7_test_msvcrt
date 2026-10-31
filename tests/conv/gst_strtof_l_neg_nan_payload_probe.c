/* TC
 * id:    gst_strtof_l_neg_nan_payload_probe
 * area:  conv
 * funcs: _strtof_l _create_locale _free_locale
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.1.3p3 (NAN(n-char-sequence)); MS docs strtof
 * note:  Probes that UCRT consumes the optional "(n-char-sequence)" after "nan". Also probes that malformed "nan(12" consumes just "nan".
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

static _locale_t loc;
static const char *volatile v;

TC_CASE(payload) { char *e = NULL; float f; v = "nan(123)x"; f = _strtof_l(v, &e, loc); CHECK(f != f); CHECK_EQ_P(e, v + 8); }
TC_CASE(unterminated) { char *e = NULL; float f; v = "nan(12"; f = _strtof_l(v, &e, loc); CHECK(f != f); CHECK_EQ_P(e, v + 3); }
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(payload); TC_RUN(unterminated);
    _free_locale(loc);
    return tc_end();
}
