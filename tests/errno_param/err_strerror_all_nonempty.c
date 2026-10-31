/* TC
 * id:    err_strerror_all_nonempty
 * area:  errno_param
 * funcs: strerror
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.24.6.2; MS docs strerror
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

static volatile int v_e;
static int bounded_len(const char *s) { int i; for (i = 0; i < 256; i++) if (!s[i]) return i; return -1; }

TC_CASE(range_0_42)
{
    int e;
    for (e = 0; e <= 42; e++) {
        const char *s; v_e = e; s = strerror(v_e);
        CHECK(s != NULL);
        if (s) { CHECK(bounded_len(s) > 0); }
    }
}
TC_CASE(strunc_80)
{
    const char *s; v_e = 80; s = strerror(v_e);
    CHECK(s != NULL);
    if (s) CHECK(bounded_len(s) >= 0);
}
TC_CASE(repeatable)
{
    int e;
    for (e = 0; e <= 42; e++) {
        char a[260]; const char *s; int n;
        v_e = e; s = strerror(v_e); if (!s) { CHECK(s != NULL); continue; }
        n = bounded_len(s); if (n < 0) { CHECK(n >= 0); continue; }
        memcpy(a, s, (size_t)n + 1);
        s = strerror(v_e);
        CHECK(s != NULL);
        if (s) CHECK_STR(s, a);
    }
}
int main(void) { tc_begin(); TC_RUN(range_0_42); TC_RUN(strunc_80); TC_RUN(repeatable); return tc_end(); }
