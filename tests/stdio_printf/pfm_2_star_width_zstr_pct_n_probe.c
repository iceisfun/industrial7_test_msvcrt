/* TC
 * id:    pfm_2_star_width_zstr_pct_n_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p2
 * conf:  observed
 * ref:   MS docs printf type field (%Z = ANSI_STRING/UNICODE_STRING address, %n disabled by default in ucrt)
 * note:  OBSERVED on ucrtbase (Win10 22H2 x64 (19045), reference, observed twice; skeptic probe runs
 *        raw/zzd_dot_probe_p0.txt, plus zzp_probe2): %Z IS supported by ucrt, but the narrow printf family takes plain %Z as a UNICODE_STRING*
 *        and %hZ as an ANSI_STRING* (MS docs say ANSI by default; this deviates from the docs, class B). Original intent restored with correctly typed
 *        arguments: "[%*hZ]|%d" with an ANSI_STRING "ab" and width 5 -> "[   ab]|7", r==9; "[%*Z]|%d" with a UNICODE_STRING L"ab" -> same.
 *        Case star_Z_ansi_mismatch_probe pins the docs-deviating behaviour: plain %Z given an ANSI_STRING reads "ab" as wchar 0x6261, which does not convert in
 *        the C locale: r=-1, errno EILSEQ, no handler (LOCALE-DEPENDENT: C locale only). Cases star_percent and star_n were already correct:
 *        (2) '%*%' consumes the int for '*' but prints a plain '%'; (3) ucrt only: '%*n' is rejected like '%n'. Stable.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

typedef struct { USHORT Length, MaximumLength; char *Buffer; } TC_ANSI;
static volatile int w5 = 5;

typedef struct { USHORT Length, MaximumLength; wchar_t *Buffer; } TC_UNI;

TC_CASE(star_Z)
{
    static char txt[] = "ab"; TC_ANSI a; char b[64]; int r;
    a.Length = 2; a.MaximumLength = 3; a.Buffer = txt;
#if TC_CRT_VER >= 140
    errno = 0; tc_iph_count = 0;
    r = sprintf(b, "[%*hZ]|%d", w5, &a, 7);          /* ucrt: %hZ = ANSI_STRING */
    CHECK_STR(b, "[   ab]|7"); CHECK_EQ_I(r, 9); CHECK_EQ_I(tc_iph_count, 0);
    r = sprintf(b, "[%-*hZ]|%d", w5, &a, 7);
    CHECK_STR(b, "[ab   ]|7"); CHECK_EQ_I(r, 9);
#else
    r = sprintf(b, "[%*Z]|%d", w5, &a, 7);
    CHECK_STR(b, "[   ab]|7"); CHECK_EQ_I(r, 9);
#endif
}
TC_CASE(star_Z_unicode)
{
#if TC_CRT_VER >= 140
    static wchar_t wtxt[] = L"ab"; TC_UNI u; char b[64]; int r;
    u.Length = 4; u.MaximumLength = 6; u.Buffer = wtxt;
    tc_iph_count = 0;
    r = sprintf(b, "[%*Z]|%d", w5, &u, 7);           /* ucrt: plain %Z in narrow printf = UNICODE_STRING */
    CHECK_STR(b, "[   ab]|7"); CHECK_EQ_I(r, 9); CHECK_EQ_I(tc_iph_count, 0);
#else
    TC_SKIP("ucrt only");
#endif
}
TC_CASE(star_Z_ansi_mismatch_probe)
{
#if TC_CRT_VER >= 140
    static char txt[] = "ab"; TC_ANSI a; char b[64]; int r;
    a.Length = 2; a.MaximumLength = 3; a.Buffer = txt;
    errno = 0; tc_iph_count = 0;
    r = sprintf(b, "[%*Z]|%d", w5, &a, 7);           /* wrong type for plain %Z; C locale only */
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ); CHECK_EQ_I(tc_iph_count, 0);
#else
    TC_SKIP("ucrt only");
#endif
}
TC_CASE(star_percent)
{
    char b[64]; int r = sprintf(b, "%*%|%d", w5, 7);
    CHECK_STR(b, "%|7"); CHECK_EQ_I(r, 3);
}
TC_CASE(star_n)
{
#if TC_CRT_VER >= 140
    char b[64]; int n = 0, r;
    tc_iph_count = 0; b[0] = 0;
    r = sprintf(b, "ab%*n", w5, &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 0);
#else
    TC_SKIP("ucrt only");
#endif
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(star_Z); TC_RUN(star_Z_unicode); TC_RUN(star_Z_ansi_mismatch_probe); TC_RUN(star_percent); TC_RUN(star_n);
    return tc_end();
}
