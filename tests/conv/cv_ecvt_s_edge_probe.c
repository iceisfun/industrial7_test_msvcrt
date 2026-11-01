/* TC
 * id:    cv_ecvt_s_edge_probe
 * area:  conv
 * funcs:
 * procs: _ecvt_s _fcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _ecvt_s, _fcvt_s
 * note:  OBSERVED (Win10 22H2 19045 ucrtbase, run twice): _ecvt_s(0.0,5) -> "00000" dec 0 (as guessed); _fcvt_s(0.0, 2) -> "00" dec 0 (guess was "000"/1);
 *        _ecvt_s(123.456, count 0) -> "" dec 3 (as guessed); a buffer of count+1 bytes is NOT enough: _ecvt_s(buf,6,123.456,5) returns ERANGE (34),
 *        empties the buffer and leaves *decpt untouched; count+2 (7) is the minimum and gives "12346" dec 3. Case renamed ecvt_min_buffer.
 *        Unverified on pre-UCRT CRTs (these still list the old expectations only where unchanged; treat as ucrtbase facts).
 */
#include "tc.h"

TC_DECL_PROC(int, _ecvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _fcvt_s, (char *, size_t, double, int, int *, int *));

TC_CASE(ecvt_zero)
{
    char b[32]; int d = -99, s = -99; volatile double v = 0.0;
    if (!tc_p__ecvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__ecvt_s(b, sizeof b, v, 5, &d, &s), 0);
    CHECK_STR(b, "00000"); CHECK_EQ_I(d, 0); CHECK_EQ_I(s, 0);
}
TC_CASE(fcvt_zero)
{
    char b[32]; int d = -99, s = -99; volatile double v = 0.0;
    if (!tc_p__fcvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__fcvt_s(b, sizeof b, v, 2, &d, &s), 0);
    CHECK_STR(b, "00"); CHECK_EQ_I(d, 0); CHECK_EQ_I(s, 0);   /* observed Win10 22H2 ucrtbase */
}
TC_CASE(ecvt_count_zero)
{
    char b[32]; int d = -99, s = -99; volatile double v = 123.456;
    if (!tc_p__ecvt_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__ecvt_s(b, sizeof b, v, 0, &d, &s), 0);
    CHECK_STR(b, ""); CHECK_EQ_I(d, 3);
}
TC_CASE(ecvt_min_buffer)
{
    char b[7]; int d = -99, s = -99; volatile double v = 123.456;   /* count + 2 bytes */
    if (!tc_p__ecvt_s) TC_SKIP("not exported");
    /* observed Win10 22H2 ucrtbase: digits + NUL is NOT enough (size count+1 -> ERANGE, buffer emptied); count + 2 is the minimum */
    CHECK_EQ_I(tc_p__ecvt_s(b, 7, v, 5, &d, &s), 0);
    CHECK_STR(b, "12346"); CHECK_EQ_I(d, 3);
    b[0] = 'x'; d = -99;
    tc_iph_install(); tc_iph_count = 0;   /* counted, not fatal (real default handler would fastfail 0xC0000409) */
    CHECK_EQ_I(tc_p__ecvt_s(b, 6, v, 5, &d, &s), 34);   /* ERANGE */
    CHECK_STR(b, ""); CHECK_EQ_I(d, -99); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_ecvt_s); TC_LOAD_PROC(_fcvt_s);
    TC_RUN(ecvt_zero); TC_RUN(fcvt_zero); TC_RUN(ecvt_count_zero); TC_RUN(ecvt_min_buffer);
    return tc_end();
}
