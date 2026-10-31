/* TC
 * id:    pff_f_large_width
 * area:  stdio_printf
 * funcs: sprintf _scprintf
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1 (width, precision); MS docs printf width specification
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static int padded(const char *b, size_t total, size_t nspace, int left, const char *num)
{   /* b is total chars: nspace spaces then num (right) or num then spaces (left) */
    size_t i, nl = strlen(num);
    if (strlen(b) != total || nspace + nl != total) return 0;
    if (left) { if (memcmp(b, num, nl)) return 0; for (i = nl; i < total; i++) if (b[i] != ' ') return 0; }
    else { for (i = 0; i < nspace; i++) if (b[i] != ' ') return 0; if (memcmp(b + nspace, num, nl)) return 0; }
    return 1;
}
TC_CASE(w300) { static char b[2048]; volatile double v = 1.5; int n = sprintf(b, "%300f", v); CHECK_EQ_I(n, 300); CHECK(padded(b, 300, 292, 0, "1.500000")); }
TC_CASE(w300_left) { static char b[2048]; volatile double v = 1.5; int n = sprintf(b, "%-300.2f", v); CHECK_EQ_I(n, 300); CHECK(padded(b, 300, 296, 1, "1.50")); }
TC_CASE(star) { static char b[2048]; volatile double v = 1.5; volatile int w = 500, p = 5; int n = sprintf(b, "%*.*f", w, p, v); CHECK_EQ_I(n, 500); CHECK(padded(b, 500, 493, 0, "1.50000")); }
TC_CASE(zero_pad) { static char b[2048]; volatile double v = -1.5; int n = sprintf(b, "%0300.1f", v), i; CHECK_EQ_I(n, 300); CHECK(b[0] == '-'); CHECK(strcmp(b + 296, "01.5") == 0 || strcmp(b + 297, "1.5") == 0);
  for (i = 1; i < 297; i++) if (b[i] != '0') { CHECK(0); break; } }
TC_CASE(scprintf) { volatile double v = 1.5; CHECK_EQ_I(_scprintf("%1000.1f", v), 1000); CHECK_EQ_I(_scprintf("%-1000.1f", v), 1000); }
int main(void) { tc_begin(); TC_RUN(w300); TC_RUN(w300_left); TC_RUN(star); TC_RUN(zero_pad); TC_RUN(scprintf); return tc_end(); }
