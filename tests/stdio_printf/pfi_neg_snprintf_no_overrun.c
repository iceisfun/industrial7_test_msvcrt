/* TC
 * id:    pfi_neg_snprintf_no_overrun
 * area:  stdio_printf
 * funcs: _snprintf
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf: at most count characters are written
 */
#include "tc.h"
#include <stdio.h>

static void fill(unsigned char *b) { int i; for (i = 0; i < 32; i++) b[i] = 0xAA; }
static int guard_ok(const unsigned char *b, int from) { int i; for (i = from; i < 32; i++) if (b[i] != 0xAA) return 0; return 1; }

TC_CASE(d)    { unsigned char b[32]; volatile int v = 12345; fill(b); _snprintf((char *)b, 4, "%d", v); CHECK(guard_ok(b, 4)); }
TC_CASE(w10)  { unsigned char b[32]; volatile int v = 12345; fill(b); _snprintf((char *)b, 4, "%10d", v); CHECK(guard_ok(b, 4)); }
TC_CASE(i64)  { unsigned char b[32]; volatile long long v = 1234567890123LL; fill(b); _snprintf((char *)b, 4, "%I64d", v); CHECK(guard_ok(b, 4)); }
TC_CASE(exact)
{
    unsigned char b[32]; int n; volatile int v = 12345; fill(b);
    n = _snprintf((char *)b, 5, "%d", v);
    CHECK_EQ_I(n, 5);
    CHECK(b[4] == '5');
    CHECK(guard_ok(b, 5));   /* no NUL written when count == length */
}

int main(void) { tc_begin(); TC_RUN(d); TC_RUN(w10); TC_RUN(i64); TC_RUN(exact); return tc_end(); }
