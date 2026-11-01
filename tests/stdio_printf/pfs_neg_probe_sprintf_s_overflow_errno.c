/* TC
 * id:    pfs_neg_probe_sprintf_s_overflow_errno
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs sprintf_s (errno on buffer overflow not documented)
 * note:  D (export alias): ucrtbase has no exported sprintf_s; now reached via __stdio_common_vsprintf_s. PROBE: errno == ERANGE after sprintf_s overflows its buffer (handler returns).
 *        Observed ERANGE after sprintf_s overflow (handler returning) on Win10 22H2 19045.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#if TC_CRT_VER >= 140
/* ucrtbase does not export sprintf_s (header inline): go through the exported __stdio_common_vsprintf_s */
static int tc_sprintf_s(char *b, size_t n, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_s(0, b, n, f, NULL, ap); va_end(ap); return r; }
#else
#undef sprintf_s
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
#define tc_sprintf_s tc_p_sprintf_s
#endif
static const char *volatile s6 = "abcdef";
TC_CASE(erange) {
    char b[4]; int n;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    n = tc_sprintf_s(b, sizeof b, "%s", s6);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE);
}
int main(void) {
    tc_begin();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(sprintf_s); if (!tc_p_sprintf_s) { tc_write("TC FAIL no sprintf_s\n"); return 1; }
#endif
    TC_RUN(erange); return tc_end();
}
