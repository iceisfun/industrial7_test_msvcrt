/* TC
 * id:    tm_neg_default_handler_fatal_strftime
 * area:  time
 * kind:  negative
 * funcs: strftime
 * crt:   >=140
 * tier:  p1
 * conf:  documented
 * ref:   MS docs strftime (NULL string/format/timeptr, out-of-range tm fields or invalid format codes invoke the invalid parameter handler)
 * note:  Each misuse runs in a child; default handler must terminate (exit != 0, != 77). The exact
 *        NTSTATUS is not asserted here (see the probe tests for 0xC0000409).
 *        Children call tc_iph_default(): the mingw startup code installs a no-op invalid-parameter handler, which hid the CRT default.
 */
#include "tc.h"
#include <time.h>

static void run(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
}
TC_CASE(null_buf) { run("nullbuf"); }
TC_CASE(bad_spec) { run("badspec"); }
TC_CASE(mon99)    { run("mon99"); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        tc_iph_default();  /* mingw startup installs a no-op handler; restore the CRT default (fast-fail) */
        char buf[64]; struct tm t; volatile size_t n = 10;
        t.tm_year = 117; t.tm_mon = 0; t.tm_mday = 1; t.tm_hour = t.tm_min = t.tm_sec = 0; t.tm_wday = 0; t.tm_yday = 0; t.tm_isdst = 0;
        if (c[0] == 'n') strftime(NULL, n, "%Y", &t);
        else if (c[0] == 'b') strftime(buf, n, "%Q", &t);
        else { t.tm_mon = 99; strftime(buf, 64, "%B", &t); }
        return 77;
    }
    TC_RUN(null_buf);
    TC_RUN(bad_spec);
    TC_RUN(mon99);
    return tc_end();
}
