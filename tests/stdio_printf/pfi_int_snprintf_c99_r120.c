/* TC
 * id:    pfi_int_snprintf_c99_r120
 * area:  stdio_printf
 * funcs:
 * procs: snprintf
 * crt:   msvcr120
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.6.5; MS docs snprintf (UCRT, VS2015+, msvcr120 export)
 * note:  PROBE: whether msvcr120.dll exports a C99-conforming snprintf (spec says VS2013 has it; MS docs list VS2015+). Expected yes.
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>
TC_DECL_PROC(int, snprintf, (char *, size_t, const char *, ...));
#define snprintf tc_p_snprintf

TC_CASE(fits) { char b[32]; int n; b[0] = 'Z'; n = snprintf(b, sizeof b, "%d", -1234); CHECK_EQ_I(n, 5); CHECK_STR(b, "-1234"); }
TC_CASE(truncated)
{
    char b[16]; int n; int i; for (i = 0; i < 16; i++) b[i] = 'Z';
    n = snprintf(b, 4, "%d", 1234567);
    CHECK_EQ_I(n, 7);
    CHECK(b[0] == '1' && b[1] == '2' && b[2] == '3' && b[3] == 0);
    CHECK(b[4] == 'Z');
}
TC_CASE(exact_boundary)
{
    char b[16]; int n; int i; for (i = 0; i < 16; i++) b[i] = 'Z';
    n = snprintf(b, 5, "%d", 1234);   /* needs 5 bytes with NUL: exact fit */
    CHECK_EQ_I(n, 4); CHECK_STR(b, "1234");
    n = snprintf(b, 4, "%d", 1234);   /* one short */
    CHECK_EQ_I(n, 4); CHECK_STR(b, "123");
}
TC_CASE(count_zero_null)
{
    volatile int v = INT_MIN; int n = snprintf(NULL, 0, "%d", v);
    CHECK_EQ_I(n, 11);
}
TC_CASE(formats)
{
    char b[64]; int n; volatile unsigned long long big = 0xFFFFFFFFFFFFFFFFULL;
    n = snprintf(b, sizeof b, "%d|%u|%x|%I64d", -5, 4000000000u, 0xbeefu, (long long)INT_MIN * 4);
    CHECK_STR(b, "-5|4000000000|beef|-8589934592"); CHECK_EQ_I(n, 30);
    n = snprintf(b, sizeof b, "%I64u", (unsigned long long)big); CHECK_STR(b, "18446744073709551615"); CHECK_EQ_I(n, 20);
}
int main(void) { tc_begin(); TC_LOAD_PROC(snprintf); CHECK(tc_p_snprintf != NULL); if (!tc_p_snprintf) return tc_end(); TC_RUN(fits); TC_RUN(truncated); TC_RUN(exact_boundary); TC_RUN(count_zero_null); TC_RUN(formats); return tc_end(); }
