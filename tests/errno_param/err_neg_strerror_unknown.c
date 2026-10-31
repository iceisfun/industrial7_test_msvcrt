/* TC
 * id:    err_neg_strerror_unknown
 * area:  errno_param
 * kind:  negative
 * funcs: strerror
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strerror (out-of-range errnum yields a message, not NULL)
 * note:  Only non-NULL / terminated / non-empty (INT_MAX, INT_MIN) asserted; invalid-parameter-handler behaviour is in err_neg_strerror_unknown_no_iph_probe.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <limits.h>

static volatile int v_e;
static int bounded_len(const char *s) { int i; for (i = 0; i < 256; i++) if (!s[i]) return i; return -1; }

TC_CASE(minus_one)  { const char *s; v_e = -1; tc_iph_count = 0; s = strerror(v_e); CHECK(s != NULL); if (s) CHECK(bounded_len(s) >= 0); }
TC_CASE(big)        { const char *s; v_e = 9999; s = strerror(v_e); CHECK(s != NULL); if (s) CHECK(bounded_len(s) >= 0); }
TC_CASE(int_max)    { const char *s; v_e = INT_MAX; s = strerror(v_e); CHECK(s != NULL); if (s) CHECK(bounded_len(s) > 0); }
TC_CASE(int_min)    { const char *s; v_e = INT_MIN; s = strerror(v_e); CHECK(s != NULL); if (s) CHECK(bounded_len(s) > 0); }

int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(minus_one); TC_RUN(big); TC_RUN(int_max); TC_RUN(int_min);
    return tc_end();
}
