/* TC
 * id:    tm_neg_default_handler_fatal_wasctime_s
 * area:  time
 * kind:  negative
 * funcs:
 * procs: _wasctime_s asctime_s
 * crt:   >=140
 * tier:  p0
 * conf:  probe
 * ref:   MS docs asctime_s, _wasctime_s; "Parameter validation"
 * note:  Documented: NULL buffer, buffer smaller than 26 elements and out-of-range tm fields invoke the invalid
 *        parameter handler; default terminates. Probed: exact status fast-fail 0xC0000409.
 */
#include "tc.h"
#include <time.h>

TC_DECL_PROC(int, _wasctime_s, (wchar_t *, size_t, const struct tm *));
TC_DECL_PROC(int, asctime_s, (char *, size_t, const struct tm *));

static void run(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
TC_CASE(wnull_buf) { run("wnullbuf"); }
TC_CASE(wsize25)   { run("wsize25"); }
TC_CASE(whour24)   { run("whour24"); }
TC_CASE(mon12)     { run("mon12"); }

int main(void)
{
    const char *c;
    tc_begin();
    TC_LOAD_PROC(_wasctime_s); TC_LOAD_PROC(asctime_s);
    if ((c = tc_child_name()) != NULL) {
        wchar_t w[32]; char b[32]; struct tm t; volatile size_t sz = 25;
        if (!tc_p__wasctime_s || !tc_p_asctime_s) return 78;
        t.tm_year = 117; t.tm_mon = 0; t.tm_mday = 1; t.tm_hour = t.tm_min = t.tm_sec = 0; t.tm_wday = 0; t.tm_yday = 0; t.tm_isdst = 0;
        if (!lstrcmpA(c, "wnullbuf")) tc_p__wasctime_s(NULL, 26, &t);
        else if (!lstrcmpA(c, "wsize25")) tc_p__wasctime_s(w, sz, &t);
        else if (!lstrcmpA(c, "whour24")) { t.tm_hour = 24; tc_p__wasctime_s(w, 26, &t); }
        else { t.tm_mon = 12; tc_p_asctime_s(b, 26, &t); }
        return 77;
    }
    CHECK(tc_p__wasctime_s && tc_p_asctime_s);
    TC_RUN(wnull_buf); TC_RUN(wsize25); TC_RUN(whour24); TC_RUN(mon12);
    return tc_end();
}
