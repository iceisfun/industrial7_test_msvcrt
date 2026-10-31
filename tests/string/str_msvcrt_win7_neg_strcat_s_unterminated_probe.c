/* TC
 * id:    str_msvcrt_win7_neg_strcat_s_unterminated_probe
 * area:  string
 * funcs:
 * procs: strcat_s _set_invalid_parameter_handler
 * crt:   msvcr100 msvcr110 msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strcat_s
 * note:  PROBE: strcat_s with a dest that has no NUL within numberOfElements: expect EINVAL (not ERANGE) and handler once.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, strcat_s, (char *, size_t, const char *));

TC_CASE(unterminated)
{
    char b[8]; int r; if (!tc_p_strcat_s) TC_SKIP("missing");
    b[0] = 'a'; b[1] = 'b'; b[2] = 'c'; b[3] = 'd'; b[4] = 0;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = tc_p_strcat_s(b, 4, "x");
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_LOAD_PROC(strcat_s); TC_RUN(unterminated); return tc_end(); }
