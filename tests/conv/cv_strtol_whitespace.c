/* TC
 * id:    cv_strtol_whitespace
 * area:  conv
 * funcs: strtol

 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.4p3, 7.4.1.10 (isspace in the C locale)
 * note:  \x85 and \xA0 are not isspace in the C locale.
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile p;

TC_CASE(all_ws) { char *e; p = "\t\n\v\f\r 5"; CHECK_EQ_I(strtol(p, &e, 10), 5); CHECK_EQ_P(e, p + 7); }
TC_CASE(each_ws)
{
    static const char w[] = " \t\n\v\f\r"; int i;
    for (i = 0; i < 6; i++) { char b[4]; char *e; b[0] = w[i]; b[1] = '9'; b[2] = 0; p = b; if (strtol(p, &e, 10) != 9 || e != b + 2) CHECK_EQ_I(i, -1); }
}
TC_CASE(ws_after_sign) { char *e = (char *)1; p = "- 5"; CHECK_EQ_I(strtol(p, &e, 10), 0); CHECK_EQ_P(e, p); }
TC_CASE(high_not_space) { char *e = (char *)1; p = "\x85" "5"; CHECK_EQ_I(strtol(p, &e, 10), 0); CHECK_EQ_P(e, p); e = (char *)1; p = "\xA0" "5"; CHECK_EQ_I(strtol(p, &e, 10), 0); CHECK_EQ_P(e, p); }
TC_CASE(embedded_nul) { static const char s[] = "12\0" "34"; char *e; p = s; CHECK_EQ_I(strtol(p, &e, 10), 12); CHECK_EQ_P(e, p + 2); }

int main(void) { tc_begin(); TC_RUN(all_ws); TC_RUN(each_ws); TC_RUN(ws_after_sign); TC_RUN(high_not_space); TC_RUN(embedded_nul); return tc_end(); }
