/* TC
 * id:    tm_neg_default_handler_fatal_ctime_s
 * area:  time
 * kind:  negative
 * funcs:
 * procs: _ctime64_s _ctime32_s _wctime64_s
 * crt:   >=140
 * tier:  p0
 * conf:  probe
 * ref:   MS docs ctime_s, _ctime32_s, _ctime64_s, _wctime_s; "Parameter validation"
 * note:  Documented: NULL buffer, size 0 and NULL timer invoke the invalid parameter handler; default is termination.
 *        Probed: exact status is fast-fail 0xC0000409 (not the 77 sentinel). Children are separate processes.
 */
#include "tc.h"
#include <time.h>

TC_DECL_PROC(int, _ctime64_s, (char *, size_t, const __time64_t *));
TC_DECL_PROC(int, _ctime32_s, (char *, size_t, const __time32_t *));
TC_DECL_PROC(int, _wctime64_s, (wchar_t *, size_t, const __time64_t *));

static void run(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
TC_CASE(null_buf)   { run("nullbuf"); }
TC_CASE(size0)      { run("size0"); }
TC_CASE(null_timer) { run("nulltimer"); }
TC_CASE(null_buf32) { run("nullbuf32"); }
TC_CASE(wnull_buf)  { run("wnullbuf"); }

int main(void)
{
    const char *c;
    tc_begin();
    TC_LOAD_PROC(_ctime64_s); TC_LOAD_PROC(_ctime32_s); TC_LOAD_PROC(_wctime64_s);
    if ((c = tc_child_name()) != NULL) {
        char b[32]; __time64_t t64 = 0; __time32_t t32 = 0; volatile size_t z = 0;
        if (!tc_p__ctime64_s || !tc_p__ctime32_s || !tc_p__wctime64_s) return 78;
        if (!lstrcmpA(c, "nullbuf")) tc_p__ctime64_s(NULL, 26, &t64);
        else if (!lstrcmpA(c, "size0")) tc_p__ctime64_s(b, z, &t64);
        else if (!lstrcmpA(c, "nulltimer")) tc_p__ctime64_s(b, 26, NULL);
        else if (!lstrcmpA(c, "nullbuf32")) tc_p__ctime32_s(NULL, 26, &t32);
        else tc_p__wctime64_s(NULL, 26, &t64);
        return 77;
    }
    CHECK(tc_p__ctime64_s && tc_p__ctime32_s && tc_p__wctime64_s);
    TC_RUN(null_buf); TC_RUN(size0); TC_RUN(null_timer); TC_RUN(null_buf32); TC_RUN(wnull_buf);
    return tc_end();
}
