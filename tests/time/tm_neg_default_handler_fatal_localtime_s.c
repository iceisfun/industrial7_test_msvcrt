/* TC
 * id:    tm_neg_default_handler_fatal_localtime_s
 * area:  time
 * kind:  negative
 * funcs:
 * procs: _localtime64_s _localtime32_s
 * crt:   >=140
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _localtime32_s, _localtime64_s; "Parameter validation"
 * note:  Documented: NULL arguments invoke the invalid parameter handler, which by default terminates the process.
 *        Probed: the exact termination status is a fast-fail (0xC0000409). Children are separate processes.
 *        Children call tc_iph_default(): the mingw startup code installs a no-op invalid-parameter handler, which hid the CRT default.
 */
#include "tc.h"
#include <time.h>

TC_DECL_PROC(int, _localtime64_s, (struct tm *, const __time64_t *));
TC_DECL_PROC(int, _localtime32_s, (struct tm *, const __time32_t *));

static void run(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
TC_CASE(null_tm)    { run("nulltm"); }
TC_CASE(null_timer) { run("nulltimer"); }
TC_CASE(null_tm32)  { run("nulltm32"); }
TC_CASE(parent_alive) { CHECK(1); }

int main(void)
{
    const char *c;
    tc_begin();
    TC_LOAD_PROC(_localtime64_s); TC_LOAD_PROC(_localtime32_s);
    if ((c = tc_child_name()) != NULL) {
        tc_iph_default();  /* mingw startup installs a no-op handler; restore the CRT default (fast-fail) */
        struct tm t; __time64_t t64 = 0; __time32_t t32 = 0;
        if (!tc_p__localtime64_s || !tc_p__localtime32_s) return 78;
        if (!lstrcmpA(c, "nulltm")) tc_p__localtime64_s(NULL, &t64);
        else if (!lstrcmpA(c, "nulltimer")) tc_p__localtime64_s(&t, NULL);
        else tc_p__localtime32_s(NULL, &t32);
        return 77;
    }
    CHECK(tc_p__localtime64_s && tc_p__localtime32_s);
    TC_RUN(null_tm); TC_RUN(null_timer); TC_RUN(null_tm32); TC_RUN(parent_alive);
    return tc_end();
}
