/* TC
 * id:    pff_f_precision_512_ucrt
 * area:  stdio_printf
 * funcs: sprintf _snprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1p15 (at least 4095 chars from one conversion); MS docs printf precision
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

/* sprintf one double with fmt, compare string and return value (== strlen of expected) */
#define TF(fmt, val, want) do { char b[1100]; volatile double tc_v = (val); int n = sprintf(b, fmt, tc_v); \
    CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

static void want_half(char *w, int prec) { int i; w[0] = '0'; w[1] = '.'; w[2] = '5'; for (i = 3; i < prec + 2; i++) w[i] = '0'; w[prec + 2] = 0; }

TC_CASE(half512) { char b[1100], w[1100]; volatile double v = 0.5; int n = sprintf(b, "%.512f", v); want_half(w, 512); CHECK_STR(b, w); CHECK_EQ_I(n, 514); }
TC_CASE(one512)
{
    char b[1100], w[1100]; volatile double v = 1.0; int n = sprintf(b, "%.512f", v), i;
    w[0] = '1'; w[1] = '.'; for (i = 0; i < 512; i++) w[2 + i] = '0'; w[514] = 0;
    CHECK_STR(b, w); CHECK_EQ_I(n, 514);
}
TC_CASE(snprintf_count)
{
    char b[1100]; volatile double v = 0.5; int n = _snprintf(b, sizeof b, "%.512f", v);
    CHECK_EQ_I(n, 514); CHECK_EQ_I((int)strlen(b), 514);
}

int main(void) { tc_begin(); TC_RUN(half512); TC_RUN(one512); TC_RUN(snprintf_count); return tc_end(); }
