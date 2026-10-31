/* TC
 * id:    tm_neg_default_handler_fatal_asctime_s
 * area:  time
 * kind:  negative
 * funcs: asctime_s
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs asctime_s, "Parameter validation"
 * note:  Probes that asctime_s with a too-small buffer (25), NULL buffer, or tm_mon=12 terminates the process under
 *        the default handler (exit code != 0 and != 77). Exact status is not asserted.
 */
#include "tc.h"
#include <time.h>

static void run(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
    CHECK(code >= 0xC0000000u);
}
TC_CASE(small_buf) { run("small"); }
TC_CASE(null_buf)  { run("nullbuf"); }
TC_CASE(mon12)     { run("mon12"); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char buf[32]; struct tm t; volatile int sz = 25;
        t.tm_year = 117; t.tm_mon = 0; t.tm_mday = 1; t.tm_hour = t.tm_min = t.tm_sec = 0; t.tm_wday = 0; t.tm_yday = 0; t.tm_isdst = 0;
        if (c[0] == 's') asctime_s(buf, (size_t)sz, &t);
        else if (c[0] == 'n') asctime_s(NULL, 26, &t);
        else { t.tm_mon = 12; asctime_s(buf, 26, &t); }
        return 77;
    }
    TC_RUN(small_buf);
    TC_RUN(null_buf);
    TC_RUN(mon12);
    return tc_end();
}
