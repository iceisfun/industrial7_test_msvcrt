/* TC
 * id:    pfi_int_width_large
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1; MS docs printf format specification
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile int v = 7;

static void chk(const char *b, int n, char fill_first, char last)
{
    CHECK_EQ_I(n, 300);
    CHECK_EQ_U(strlen(b), 300);
    CHECK(b[0] == fill_first);
    CHECK(b[299] == last);
}
TC_CASE(right) { static char b[512]; int i, ok = 1; int n = sprintf(b, "%300d", v); chk(b, n, ' ', '7');
    for (i = 0; i < 299; i++) if (b[i] != ' ') ok = 0; CHECK(ok); }
TC_CASE(left) { static char b[512]; int i, ok = 1; int n = sprintf(b, "%-300d", v); chk(b, n, '7', ' ');
    for (i = 1; i < 300; i++) if (b[i] != ' ') ok = 0; CHECK(ok); }
TC_CASE(zeros) { static char b[512]; int i, ok = 1; int n = sprintf(b, "%0300d", v); chk(b, n, '0', '7');
    for (i = 0; i < 299; i++) if (b[i] != '0') ok = 0; CHECK(ok); }

int main(void) { tc_begin(); TC_RUN(right); TC_RUN(left); TC_RUN(zeros); return tc_end(); }
