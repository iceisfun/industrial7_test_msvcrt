/* TC
 * id:    err_neg_probe_perror_unknown_errno_preserved
 * area:  errno_param
 * kind:  negative
 * funcs: perror freopen
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs perror (silent on errno after the call)
 * note:  PROBE: perror with an out-of-range errno (9999, -1, INT_MAX) leaves errno unchanged and does not invoke the
 *        invalid-parameter handler. Runs in a child whose stderr goes to a temp file (tc_err_neg_perror_keep_<ver>.tmp); child exit 77 = both held.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>

#define TC_STR2(x) #x
#define TC_STR(x) TC_STR2(x)
#define TMPF "tc_err_neg_perror_keep_" TC_STR(TC_CRT_VER) ".tmp"
static volatile int v_e;

TC_CASE(unchanged)
{
    DWORD code = 0; int ran = tc_spawn_self("keep", &code, 20000);
    DeleteFileA(TMPF);
    CHECK(ran); CHECK_EQ_U(code, 77);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        static const int vals[3] = { 9999, -1, INT_MAX }; int i, bad = 0;
        tc_iph_install();
        if (!freopen(TMPF, "w", stderr)) return 3;
        for (i = 0; i < 3; i++) { v_e = vals[i]; errno = v_e; perror("pfx"); if (errno != v_e) bad = 1; }
        if (tc_iph_count != 0) bad = 1;
        fclose(stderr);
        return bad ? 78 : 77;
    }
    TC_RUN(unchanged);
    return tc_end();
}
