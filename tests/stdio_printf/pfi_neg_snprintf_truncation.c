/* TC
 * id:    pfi_neg_snprintf_truncation
 * area:  stdio_printf
 * funcs: _snprintf
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf: returns negative if more than count chars; no NUL if exactly count
 */
#include "tc.h"
#include <stdio.h>

static void fill(char *b) { int i; for (i = 0; i < 32; i++) b[i] = 'Z'; }

TC_CASE(count3)
{
    char b[32]; int n; volatile int v = 12345; fill(b);
    n = _snprintf(b, 3, "%d", v);
    CHECK(n < 0);
    CHECK(b[0] == '1' && b[1] == '2' && b[2] == '3');
}
TC_CASE(count1)
{
    char b[32]; int n; volatile int v = 12345; fill(b);
    n = _snprintf(b, 1, "%d", v);
    CHECK(n < 0);
    CHECK(b[0] == '1');
}
TC_CASE(width)
{
    char b[32]; int n; volatile int v = 42; fill(b);
    n = _snprintf(b, 6, "%10d", v);
    CHECK(n < 0);
    CHECK(b[0] == ' ' && b[1] == ' ' && b[2] == ' ' && b[3] == ' ' && b[4] == ' ' && b[5] == ' ');
}
TC_CASE(i64)
{
    char b[32]; int n; volatile long long v = 1234567890123LL; fill(b);
    n = _snprintf(b, 8, "%I64d", v);
    CHECK(n < 0);
    CHECK(b[0]=='1' && b[1]=='2' && b[2]=='3' && b[3]=='4' && b[4]=='5' && b[5]=='6' && b[6]=='7' && b[7]=='8');
}

int main(void) { tc_begin(); TC_RUN(count3); TC_RUN(count1); TC_RUN(width); TC_RUN(i64); return tc_end(); }
