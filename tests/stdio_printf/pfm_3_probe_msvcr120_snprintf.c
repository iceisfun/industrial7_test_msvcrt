/* TC
 * id:    pfm_3_probe_msvcr120_snprintf
 * area:  stdio_printf
 * funcs: strlen
 * procs: snprintf vsnprintf
 * crt:   msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs snprintf, vsnprintf (C99 snprintf added with VS2013)
 * note:  PROBE: msvcr120.dll exports C99-conforming snprintf and vsnprintf (truncates, always NUL-terminates, returns the
 *        untruncated length). Export presence and the C99 truncation return are what is being probed.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

TC_DECL_PROC(int, snprintf, (char *, size_t, const char *, ...));
TC_DECL_PROC(int, vsnprintf, (char *, size_t, const char *, va_list));
#define FMT "%5.2f|%-4d|%s"
#define WANT " 3.14|7   |ok"

static int call_v(char *b, size_t n, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = tc_p_vsnprintf(b, n, f, ap); va_end(ap); return r; }

TC_CASE(c99) {
    char b[64]; volatile double d = 3.14159; volatile int i = 7; int r;
    CHECK_EQ_I((int)strlen(" 3.1"), 4);
    CHECK(tc_p_snprintf != NULL); CHECK(tc_p_vsnprintf != NULL);
    if (!tc_p_snprintf || !tc_p_vsnprintf) return;
    r = tc_p_snprintf(b, 64, FMT, d, i, "ok"); CHECK_STR(b, WANT); CHECK_EQ_I(r, 13);
    r = call_v(b, 64, FMT, d, i, "ok"); CHECK_STR(b, WANT); CHECK_EQ_I(r, 13);
    r = tc_p_snprintf(b, 5, FMT, d, i, "ok"); CHECK_STR(b, " 3.1"); CHECK_EQ_I(r, 13);
}

int main(void) { tc_begin(); TC_LOAD_PROC(snprintf); TC_LOAD_PROC(vsnprintf); TC_RUN(c99); return tc_end(); }
