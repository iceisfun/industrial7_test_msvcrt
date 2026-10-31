/* TC
 * id:    gtm_neg_tzset_malformed_probe
 * area:  time
 * funcs: _tzset _putenv _get_tzname _get_timezone
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _tzset (TZ format tzn[+|-]hh[:mm[:ss]][dzn]; tzn and dzn are three letters)
 * note:  The "AB" (name shorter than 3 chars) case relies on the CRT's own handling of a short name (classic parsers advance 3 chars
 *        past the start, i.e. beyond the terminator); a 0 result is a guess. The 40-char offset case accepts 0 or 18000 only.
 *        PROBE: malformed TZ values. Complements dat_neg_tz_malformed_probe (ZZZ, XYZ+, 400-char name); only other cases here.
 *        TZ="" (via putenv "TZ=" which removes the variable) only checks no crash + invariants. "AB" (no offset) probes that
 *        _timezone becomes 0 (a prior TZ=PST8 sets 28800 first). A 40-char name probes 3-char truncation. No iph expected.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void name0(char *out, size_t n) { size_t k = 0; out[0] = 'x'; if (_get_tzname(&k, out, n, 0) != 0) out[0] = '?'; }

TC_CASE(empty_tz)
{
    char a[64]; long tz = 99; int r;
    _putenv("TZ="); _tzset();
    r = _get_timezone(&tz);
    CHECK_EQ_I(r, 0);
    name0(a, sizeof a);
    CHECK(memchr(a, 0, sizeof a) != NULL);
    CHECK(strlen(a) <= 63);
}
TC_CASE(two_letter_no_offset)
{
    long tz = 99;
    _putenv("TZ=PST8"); _tzset();   /* priming only; the documented PST8 behaviour is asserted elsewhere */
    _putenv("TZ=AB"); _tzset();
    CHECK_EQ_I(_get_timezone(&tz), 0);
    CHECK_EQ_I(tz, 0);
}
TC_CASE(partial_rule)
{
    char a[64]; long tz = 99;
    _putenv("TZ=EST5EDT4,M3.2.0"); _tzset();
    CHECK_EQ_I(_get_timezone(&tz), 0);
    name0(a, sizeof a);
    CHECK_STR(a, "EST");
}
TC_CASE(long_name_truncated)
{
    static char s[64]; char a[64];
    memcpy(s, "TZ=", 3); memset(s + 3, 'A', 40); s[43] = '5'; s[44] = 0;
    _putenv(s); _tzset();
    name0(a, sizeof a);
    CHECK_STR(a, "AAA");
}
TC_CASE(long_name_offset)
{
    static char s[64]; long tz = 99;
    memcpy(s, "TZ=", 3); memset(s + 3, 'A', 40); s[43] = '5'; s[44] = 0;
    _putenv(s); _tzset();
    CHECK_EQ_I(_get_timezone(&tz), 0);
    /* Unknown: a 3-char-name parser reads the offset after char 3 (-> 0); a parser that skips the whole alpha run gives 18000.
       Only the two plausible outcomes are accepted; the value is recorded for real-Windows runs. */
    { char t[24]; tc_i64(tz, t); tc_write("TC NOTE 40-char TZ name: _timezone="); tc_write(t); tc_write("\n"); }
    CHECK(tz == 0 || tz == 18000);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(empty_tz); TC_RUN(two_letter_no_offset); TC_RUN(partial_rule); TC_RUN(long_name_truncated); TC_RUN(long_name_offset);
    _putenv("TZ="); _tzset();
    return tc_end();
}
