/* TC
 * id:    tm_neg_default_handler_fatal_get_tzname
 * area:  time
 * kind:  negative
 * funcs: _get_tzname _ftime64_s _strdate_s
 * crt:   >=140
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _get_tzname (index must be 0 or 1), _ftime_s (NULL timeptr), _strdate_s (buffer >= 9)
 * note:  Each misuse runs in a child process; default handler terminates it
 *        (exit != 0, != 77; exact NTSTATUS not asserted).
 */
#include "tc.h"
#include <time.h>
#include <sys/timeb.h>

static void run(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
}
TC_CASE(tzname_index5) { run("tz5"); }
TC_CASE(ftime_null)    { run("ftime"); }
TC_CASE(strdate_small) { run("strdate"); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char buf[32]; size_t n = 0; volatile int idx = 5; volatile size_t sz = 3;
        if (c[0] == 't') _get_tzname(&n, buf, 10, idx);
        else if (c[0] == 'f') _ftime64_s(NULL);
        else _strdate_s(buf, sz);
        return 77;
    }
    TC_RUN(tzname_index5);
    TC_RUN(ftime_null);
    TC_RUN(strdate_small);
    return tc_end();
}
