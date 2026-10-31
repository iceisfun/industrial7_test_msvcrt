/* TC
 * id:    pff_neg_snprintf_f_truncate
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf: returns -1 if more than count chars needed; no NUL if exactly count
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

/* sprintf one double with fmt, compare string and return value (== strlen of expected) */
#define TF(fmt, val, want) do { char b[1100]; volatile double tc_v = (val); int n = sprintf(b, fmt, tc_v); \
    CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

static void fill(char *b) { int i; for (i = 0; i < 32; i++) b[i] = 'Z'; }
TC_CASE(truncated) { char b[32]; volatile double v = 1.5; int n; fill(b); n = _snprintf(b, 5, "%f", v); CHECK_EQ_I(n, -1); CHECK(b[0]=='1' && b[1]=='.' && b[2]=='5' && b[3]=='0' && b[4]=='0'); CHECK(b[5]=='Z'); }
TC_CASE(exact) { char b[32]; volatile double v = 1.5; int n; fill(b); n = _snprintf(b, 8, "%f", v); CHECK_EQ_I(n, 8); CHECK(b[7]=='0'); CHECK(b[8]=='Z'); }
TC_CASE(fits) { char b[32]; volatile double v = 1.5; int n; fill(b); n = _snprintf(b, 9, "%f", v); CHECK_EQ_I(n, 8); CHECK_STR(b, "1.500000"); }
TC_CASE(sign_counts) { char b[32]; volatile double v = -1.5; int n; fill(b); n = _snprintf(b, 8, "%f", v); CHECK_EQ_I(n, -1); CHECK(b[8]=='Z'); }
TC_CASE(big_value) { char b[32]; volatile double v = 1e10; int n; fill(b); n = _snprintf(b, 10, "%.2f", v); CHECK_EQ_I(n, -1); CHECK(b[10]=='Z'); }

int main(void) { tc_begin(); TC_RUN(truncated); TC_RUN(exact); TC_RUN(fits); TC_RUN(sign_counts); TC_RUN(big_value); return tc_end(); }
