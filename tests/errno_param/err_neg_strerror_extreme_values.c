/* TC
 * id:    err_neg_strerror_extreme_values
 * area:  errno_param
 * kind:  negative
 * funcs: strerror _strerror _wcserror
 * crt:   all
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs strerror, _strerror, _wcserror (return a message string; behaviour for out-of-range numbers is not spelled out)
 * note:  Only: non-NULL, bounded, non-empty, no crash, and the _strerror(NULL)/_strerror(msg) layout. Whether errno/the handler are touched is probed in err_neg_probe_strerror_extreme_errno_iph.
 */
#include "tc.h"
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static volatile int v_e;
static int blen(const char *s) { int i; for (i = 0; i < 512; i++) if (!s[i]) return i; return -1; }
static int wblen(const wchar_t *s) { int i; for (i = 0; i < 512; i++) if (!s[i]) return i; return -1; }

TC_CASE(strerror_int_min)    { const char *s; v_e = INT_MIN; s = strerror(v_e); CHECK(s != NULL); if (s) CHECK(blen(s) > 0); }
TC_CASE(strerror_int_max)    { const char *s; v_e = INT_MAX; s = strerror(v_e); CHECK(s != NULL); if (s) CHECK(blen(s) > 0); }
TC_CASE(strerror_minus_one)  { const char *s; v_e = -1; s = strerror(v_e); CHECK(s != NULL); if (s) CHECK(blen(s) > 0); }
TC_CASE(strerror_above_table){ const char *s; v_e = 200; s = strerror(v_e); CHECK(s != NULL); if (s) CHECK(blen(s) > 0); }

TC_CASE(strerror_stable_text) { const char *a, *b; v_e = INT_MAX; a = strerror(v_e); b = strerror(v_e); CHECK(a && b); if (a && b) CHECK_STR(a, b); }

TC_CASE(_strerror_null_errno_max)
{
    const char *s; int n; errno = INT_MAX; s = _strerror(NULL); CHECK(s != NULL);
    if (s) { n = blen(s); CHECK(n > 1); if (n > 1) CHECK_EQ_I(s[n - 1], '\n'); }
}
TC_CASE(_strerror_prefix_errno_min)
{
    const char *s; int n; errno = INT_MIN; s = _strerror("pfx"); CHECK(s != NULL);
    if (s) { n = blen(s); CHECK(n > 6); CHECK(strncmp(s, "pfx: ", 5) == 0); if (n > 0) CHECK_EQ_I(s[n - 1], '\n'); }
    errno = 0;
}
TC_CASE(_wcserror_extremes)
{
    const wchar_t *a, *b, *c; v_e = INT_MIN; a = _wcserror(v_e); v_e = INT_MAX; b = _wcserror(v_e); v_e = -1; c = _wcserror(v_e);
    CHECK(a != NULL); CHECK(b != NULL); CHECK(c != NULL);
    if (a) CHECK(wblen(a) > 0);
    if (b) CHECK(wblen(b) > 0);
    if (c) CHECK(wblen(c) > 0);
}

int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(strerror_int_min); TC_RUN(strerror_int_max); TC_RUN(strerror_minus_one); TC_RUN(strerror_above_table);
    TC_RUN(strerror_stable_text); TC_RUN(_strerror_null_errno_max); TC_RUN(_strerror_prefix_errno_min); TC_RUN(_wcserror_extremes);
    return tc_end();
}
