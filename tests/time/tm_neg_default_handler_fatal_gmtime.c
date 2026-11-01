/* TC
 * id:    tm_neg_default_handler_fatal_gmtime
 * area:  time
 * kind:  negative
 * funcs: _gmtime64_s
 * crt:   >=140
 * tier:  p0
 * conf:  probe
 * ref:   MS docs "Parameter validation"; _gmtime64_s (NULL arguments invoke the invalid parameter handler)
 * note:  Probes the exact termination status (expected fast-fail 0xC0000409) of the UCRT default invalid-parameter
 *        path reached via _gmtime64_s. The docs only promise termination. Children are separate processes.
 */
#include "tc.h"
#include <time.h>

static void run(const char *name, const char *label)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    (void)label;
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
TC_CASE(null_tm)   { run("nulltm", "tm"); }
TC_CASE(null_time) { run("nulltime", "time"); }
TC_CASE(parent_alive) { CHECK(1); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        tc_iph_default();  /* mingw startup installs a no-op handler; restore the CRT default (fast-fail) */
        volatile __time64_t t = 0; struct tm tmv; __time64_t tt = t;
        if (c[4] == 't' && c[5] == 'm') _gmtime64_s(NULL, &tt);     /* "nulltm" */
        else _gmtime64_s(&tmv, NULL);                               /* "nulltime" */
        return 77;
    }
    TC_RUN(null_tm);
    TC_RUN(null_time);
    TC_RUN(parent_alive);
    return tc_end();
}
