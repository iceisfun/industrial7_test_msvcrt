/* TC
 * id:    exp_probe_time_inline_exports
 * area:  exports
 * funcs: 
 * procs: difftime mktime ctime
 * crt:   msvcrt msvcr80 msvcr90 msvcr100 msvcr110
 * tier:  p2
 * conf:  probe
 * ref:   MS docs difftime, mktime, ctime
 * note:  Probing: whether mktime/ctime (msvcrt..msvcr110) and difftime (msvcrt, msvcr80 only) are real DLL exports. ucrtbase and msvcr120 are
 *        excluded: their DLLs are believed not to export these names (headers inline them to the 64-bit variants).
 */
#include "tc.h"

static void need_all(const char *const *names, int n)
{
    int i;
    for (i = 0; i < n; i++)
        if (!tc_crt_proc(names[i])) tc_fail_expr(__FILE__, __LINE__, names[i]);
}
#define NEEDS(arr) need_all(arr, (int)(sizeof(arr) / sizeof((arr)[0])))

TC_CASE(difftime)
{
    if (TC_CRT_VER > 80) TC_SKIP("difftime is believed not exported after msvcr80");
    static const char *const names[] = { "difftime" };
    NEEDS(names);
}

TC_CASE(mktime)
{
    static const char *const names[] = { "mktime" };
    NEEDS(names);
}

TC_CASE(ctime)
{
    static const char *const names[] = { "ctime" };
    NEEDS(names);
}

int main(void)
{
    tc_begin();
    TC_RUN(difftime);
    TC_RUN(mktime);
    TC_RUN(ctime);
    return tc_end();
}
