/* TC
 * id:    tm_neg_default_handler_fatal_mktime
 * area:  time
 * kind:  negative
 * funcs: _mktime64
 * procs: _mkgmtime64 _mktime32 asctime _wasctime _gmtime64 _localtime64
 * crt:   >=140
 * tier:  p0
 * conf:  probe
 * ref:   MS docs mktime, _mkgmtime, asctime, _wasctime, _gmtime64, _localtime64; "Parameter validation"
 * note:  Documented: NULL argument invokes the invalid parameter handler; default handler terminates the process.
 *        Probed: exact status is fast-fail 0xC0000409. Children are separate processes.
 */
#include "tc.h"
#include <time.h>

TC_DECL_PROC(__time64_t, _mkgmtime64, (struct tm *));
TC_DECL_PROC(__time32_t, _mktime32, (struct tm *));
TC_DECL_PROC(char *, asctime, (const struct tm *));
TC_DECL_PROC(wchar_t *, _wasctime, (const struct tm *));
TC_DECL_PROC(struct tm *, _gmtime64, (const __time64_t *));
TC_DECL_PROC(struct tm *, _localtime64, (const __time64_t *));

static void run(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
TC_CASE(mktime64)    { run("mktime64"); }
TC_CASE(mkgmtime64)  { run("mkgmtime64"); }
TC_CASE(mktime32)    { run("mktime32"); }
TC_CASE(asctime)     { run("asctime"); }
TC_CASE(wasctime)    { run("wasctime"); }
TC_CASE(gmtime64)    { run("gmtime64"); }
TC_CASE(localtime64) { run("localtime64"); }
TC_CASE(parent_alive) { CHECK(1); }

int main(void)
{
    const char *c;
    tc_begin();
    TC_LOAD_PROC(_mkgmtime64); TC_LOAD_PROC(_mktime32); TC_LOAD_PROC(asctime); TC_LOAD_PROC(_wasctime);
    TC_LOAD_PROC(_gmtime64); TC_LOAD_PROC(_localtime64);
    if ((c = tc_child_name()) != NULL) {
        tc_iph_default();  /* mingw startup installs a no-op handler; restore the CRT default (fast-fail) */
        if (!tc_p__mkgmtime64 || !tc_p__mktime32 || !tc_p_asctime || !tc_p__wasctime || !tc_p__gmtime64 || !tc_p__localtime64) return 78;
        if (!lstrcmpA(c, "mktime64")) _mktime64(NULL);
        else if (!lstrcmpA(c, "mkgmtime64")) tc_p__mkgmtime64(NULL);
        else if (!lstrcmpA(c, "mktime32")) tc_p__mktime32(NULL);
        else if (!lstrcmpA(c, "asctime")) tc_p_asctime(NULL);
        else if (!lstrcmpA(c, "wasctime")) tc_p__wasctime(NULL);
        else if (!lstrcmpA(c, "gmtime64")) tc_p__gmtime64(NULL);
        else tc_p__localtime64(NULL);
        return 77;
    }
    CHECK(tc_p__mkgmtime64 && tc_p__mktime32 && tc_p_asctime && tc_p__wasctime && tc_p__gmtime64 && tc_p__localtime64);
    TC_RUN(mktime64); TC_RUN(mkgmtime64); TC_RUN(mktime32); TC_RUN(asctime); TC_RUN(wasctime); TC_RUN(gmtime64); TC_RUN(localtime64); TC_RUN(parent_alive);
    return tc_end();
}
