/* TC
 * id:    pfm_3_target_msvcr110_120_func_matrix
 * area:  stdio_printf
 * funcs: sprintf _snprintf _scprintf sprintf_s _sprintf_p _sprintf_l
 * crt:   msvcr120 msvcr110
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf, _sprintf_l, sprintf_s, _sprintf_p, _scprintf
 * note:  C99 snprintf/vsnprintf export presence is probed separately in pfm_3_probe_msvcr120_snprintf.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

#define FMT "%5.2f|%-4d|%s"
#define WANT " 3.14|7   |ok"

TC_CASE(plain) {
    char b[64]; volatile double d = 3.14159; volatile int i = 7; int r;
    r = sprintf(b, FMT, d, i, "ok"); CHECK_STR(b, WANT); CHECK_EQ_I(r, 13);
    r = _snprintf(b, 64, FMT, d, i, "ok"); CHECK_STR(b, WANT); CHECK_EQ_I(r, 13);
    r = _scprintf(FMT, d, i, "ok"); CHECK_EQ_I(r, 13);
}
TC_CASE(secure) {
    char b[64]; volatile double d = 3.14159; volatile int i = 7; int r;
    r = sprintf_s(b, 64, FMT, d, i, "ok"); CHECK_STR(b, WANT); CHECK_EQ_I(r, 13);
}
TC_CASE(positional_locale) {
    char b[64]; volatile double d = 3.14159; volatile int i = 7; int r;
    r = _sprintf_p(b, 64, FMT, d, i, "ok"); CHECK_STR(b, WANT); CHECK_EQ_I(r, 13);
    r = _sprintf_l(b, FMT, NULL, d, i, "ok"); CHECK_STR(b, WANT); CHECK_EQ_I(r, 13);
}
int main(void) { tc_begin(); TC_RUN(plain); TC_RUN(secure); TC_RUN(positional_locale); return tc_end(); }
