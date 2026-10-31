/* TC
 * id:    cv_strtod_long_input
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * note:  PROBE: C11 only mandates correct rounding for <= DECIMAL_DIG significant digits; for longer inputs any of the two
 *        neighbours is allowed. UCRT is believed to round exactly (sticky bit for digits beyond its ~768-digit buffer). ucrtbase only: pre-UCRT CRTs read at most ~21 significant digits, so long-tail rounding is not expected there.
 * ref:   C11 7.22.1.3: result correctly rounded regardless of the number of digits (round-to-nearest-even)
 */
#include "tc.h"

TC_DECL_PROC(double, strtod, (const char *, char **));
static char buf[24000];
static size_t len;
static void put(const char *s) { while (*s) buf[len++] = *s++; buf[len] = 0; }
static void putn(char c, int n) { while (n-- > 0) buf[len++] = c; buf[len] = 0; }
static double conv(char **e) { return tc_p_strtod(buf, e); }

TC_CASE(one_then_400_zeros)
{
    char *e; len = 0; put("1"); putn('0', 400); put("e-400");
    CHECK_BITS_D(conv(&e), 1.0); CHECK(e == buf + len);
}
TC_CASE(500_zero_fraction)
{
    char *e; len = 0; put("0."); putn('0', 499); put("1e500");
    CHECK_BITS_D(conv(&e), 1.0); CHECK(e == buf + len);
}
TC_CASE(halfway_long_tail)
{
    /* 1 + 2^-53 exactly, padded with zeros to ~800 significant digits: tie -> even (1.0). A trailing 1 breaks the tie upward. */
    char *e; len = 0; put("1.00000000000000011102230246251565404236316680908203125"); putn('0', 750);
    CHECK_BITS_D(conv(&e), 1.0); CHECK(e == buf + len);
    put("1");
    CHECK_BITS_D(conv(&e), 1.0000000000000002220446049250313);  /* 1 + 2^-52 */
    CHECK(e == buf + len);
}
TC_CASE(leading_zeros)
{
    char *e; len = 0; putn('0', 10000); put("5");
    CHECK_BITS_D(conv(&e), 5.0); CHECK(e == buf + len);
    len = 0; put("0."); putn('0', 10000); put("5e10000");   /* 5e-10001 * 1e10000 = 0.5 */
    CHECK_BITS_D(conv(&e), 0.5); CHECK(e == buf + len);
}
TC_CASE(long_fraction_ties)
{
    char *e; len = 0; put("9007199254740993."); putn('0', 600);   /* exact tie -> even 2^53 */
    CHECK_BITS_D(conv(&e), 9007199254740992.0);
    put("1");                                                      /* just above the tie -> ..994 */
    CHECK_BITS_D(conv(&e), 9007199254740994.0);
    len = 0; put("2.5"); putn('0', 1000);
    CHECK_BITS_D(conv(&e), 2.5); CHECK(e == buf + len);
}
TC_CASE(exponent_leading_zeros)
{
    char *e; len = 0; put("1e+00000000000000000005");
    CHECK_BITS_D(conv(&e), 100000.0); CHECK(e == buf + len);
    len = 0; put("1e-00000000000000000005");
    CHECK_BITS_D(conv(&e), 1e-5); CHECK(e == buf + len);
}

int main(void)
{
    tc_begin(); TC_LOAD_PROC(strtod);
    if (!tc_p_strtod) { tc_write("TC FAIL setup\n"); return 1; }
    TC_RUN(one_then_400_zeros); TC_RUN(500_zero_fraction); TC_RUN(halfway_long_tail); TC_RUN(leading_zeros);
    TC_RUN(long_fraction_ties); TC_RUN(exponent_leading_zeros);
    return tc_end();
}
