/* TC
 * id:    pfo_probe_vsnprintf_c_count
 * area:  stdio_printf
 * funcs: _vsnprintf_c
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _snprintf_c, _vsnprintf_c
 * note:  PROBE: for _vsnprintf_c, count is the total buffer size INCLUDING the terminating NUL (count == len+1 fits
 *        and returns len; count == 4 with a 6-char result gives "abc" and -1; byte count is never written).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int cc(char *b, size_t c, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = _vsnprintf_c(b, c, f, ap); va_end(ap); return r; }
static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 'Z'; }

TC_CASE(overflow) { char b[16]; volatile size_t c = 4; int r; fill(b); r = cc(b, c, "%s", "abcdef"); CHECK_EQ_I(r, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(b[4], 'Z'); }
TC_CASE(count_len_plus1) { char b[16]; volatile size_t c = 7; int r; fill(b); r = cc(b, c, "%s", "abcdef"); CHECK_EQ_I(r, 6); CHECK_STR(b, "abcdef"); }
int main(void) { tc_begin(); TC_RUN(overflow); TC_RUN(count_len_plus1); return tc_end(); }
