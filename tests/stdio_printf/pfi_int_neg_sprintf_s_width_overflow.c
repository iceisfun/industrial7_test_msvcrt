/* TC
 * id:    pfi_int_neg_sprintf_s_width_overflow
 * area:  stdio_printf
 * funcs: strlen
 * procs: sprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s (buffer too small: invalid parameter handler, returns -1)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

#if TC_CRT_VER >= 140
/* ucrtbase exports no sprintf_s symbol; the header inline goes through __stdio_common_vsprintf_s */
static int tc_ss(char *b, size_t n, const char *f, int v) { return sprintf_s(b, n, f, v); }
#define HAVE_SS 1
#else
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
static int tc_ss(char *b, size_t n, const char *f, int v) { return tc_p_sprintf_s ? tc_p_sprintf_s(b, n, f, v) : -99; }
#define HAVE_SS (tc_p_sprintf_s != NULL)
#endif

/* A large width must not be truncated silently by plain sprintf, and sprintf_s must reject a too-small buffer. */
TC_CASE(width_exceeds_buffer)
{
    char b[8]; int n;
    CHECK(HAVE_SS);
    if (!HAVE_SS || !tc_iph_install()) return;
    tc_iph_count = 0; b[0] = 'x';
    n = tc_ss(b, sizeof b, "%20d", (int)5);
    CHECK_EQ_I(n, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(precision_exceeds_buffer)
{
    char b[8]; int n;
    if (!HAVE_SS || !tc_iph_install()) return;
    tc_iph_count = 0;
    n = tc_ss(b, sizeof b, "%.20d", (int)5);
    CHECK_EQ_I(n, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(exact_fit_ok)
{
    char b[8]; int n;
    if (!HAVE_SS) return;
    n = tc_ss(b, sizeof b, "%7d", (int)5);
    CHECK_EQ_I(n, 7);
    CHECK_STR(b, "      5");
    CHECK_EQ_U(strlen(b), 7);
}

int main(void) { tc_begin(); 
#if TC_CRT_VER < 140
    TC_LOAD_PROC(sprintf_s);
#endif
    TC_RUN(width_exceeds_buffer); TC_RUN(precision_exceeds_buffer); TC_RUN(exact_fit_ok); return tc_end(); }
