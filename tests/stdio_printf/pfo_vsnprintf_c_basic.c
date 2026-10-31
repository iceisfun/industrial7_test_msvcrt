/* TC
 * id:    pfo_vsnprintf_c_basic
 * area:  stdio_printf
 * funcs: _vsnprintf_c
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _snprintf_c, _vsnprintf_c (returns number of bytes written, -1 on truncation)
 * note:  _vsnprintf_c is not exported by ucrtbase (inline-only there). Count semantics (count includes NUL) are in pfo_probe_vsnprintf_c_count.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int cc(char *b, size_t c, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = _vsnprintf_c(b, c, f, ap); va_end(ap); return r; }
static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 'Z'; }

TC_CASE(fits) { char b[16]; int r; fill(b); r = cc(b, 16, "%s", "abc"); CHECK_EQ_I(r, 3); CHECK_STR(b, "abc"); }
int main(void) { tc_begin(); TC_RUN(fits); return tc_end(); }
