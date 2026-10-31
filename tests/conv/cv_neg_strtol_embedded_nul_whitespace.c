/* TC
 * id:    cv_neg_strtol_embedded_nul_whitespace
 * area:  conv
 * funcs: strtol strtoul _strtoi64 _strtoui64 atoi
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.4 (subject sequence, isspace skipping, no-conversion endptr = nptr); MS docs strtol, strtoul, atoi
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile v_s;

TC_CASE(embedded_nul)
{
    static const char s[] = "12\0 34"; char *e = 0;
    v_s = s; CHECK_EQ_I(strtol(v_s, &e, 10), 12); CHECK_EQ_P(e, s + 2);
    v_s = s; CHECK_EQ_U(strtoul(v_s, &e, 10), 12); CHECK_EQ_P(e, s + 2);
    v_s = s; CHECK_EQ_I(_strtoi64(v_s, &e, 10), 12); CHECK_EQ_P(e, s + 2);
    v_s = s; CHECK_EQ_U(_strtoui64(v_s, &e, 10), 12); CHECK_EQ_P(e, s + 2);
    v_s = s; CHECK_EQ_I(atoi(v_s), 12);
}
TC_CASE(all_isspace_skipped)
{
    static const char s[] = "\t\n\v\f\r 7"; char *e = 0;
    v_s = s; CHECK_EQ_I(strtol(v_s, &e, 10), 7); CHECK_EQ_P(e, s + 7);
    v_s = s; CHECK_EQ_I(_strtoi64(v_s, &e, 10), 7); CHECK_EQ_P(e, s + 7);
    v_s = s; CHECK_EQ_I(atoi(v_s), 7);
}
TC_CASE(space_after_sign)
{
    static const char s[] = "- 7"; char *e = 0;
    v_s = s; CHECK_EQ_I(strtol(v_s, &e, 10), 0); CHECK_EQ_P(e, s);
    v_s = s; CHECK_EQ_U(strtoul(v_s, &e, 10), 0); CHECK_EQ_P(e, s);
    v_s = s; CHECK_EQ_I(_strtoi64(v_s, &e, 10), 0); CHECK_EQ_P(e, s);
    v_s = s; CHECK_EQ_I(atoi(v_s), 0);
}
TC_CASE(underscore_stops)
{
    static const char s[] = "1_000"; char *e = 0;
    v_s = s; CHECK_EQ_I(strtol(v_s, &e, 10), 1); CHECK_EQ_P(e, s + 1);
    v_s = s; CHECK_EQ_I(atoi(v_s), 1);
}
TC_CASE(ul_space_minus)
{
    static const char s[] = " -"; char *e = 0;
    v_s = s; CHECK_EQ_U(strtoul(v_s, &e, 10), 0); CHECK_EQ_P(e, s);
    v_s = s; CHECK_EQ_U(_strtoui64(v_s, &e, 10), 0); CHECK_EQ_P(e, s);
}

int main(void)
{
    tc_begin();
    TC_RUN(embedded_nul); TC_RUN(all_isspace_skipped); TC_RUN(space_after_sign); TC_RUN(underscore_stops); TC_RUN(ul_space_minus);
    return tc_end();
}
