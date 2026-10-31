/* TC
 * id:    exp_probe_atexit_export
 * area:  exports
 * funcs: 
 * procs: atexit
 * crt:   msvcrt msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs atexit
 * note:  Probing: whether msvcrt.dll and msvcr120.dll export atexit. ucrtbase and (believed) msvcr80..110 do not: their atexit is a static
 *        wrapper in the startup library around _onexit/_crt_atexit, so they are excluded.
 */
#include "tc.h"

static void need_all(const char *const *names, int n)
{
    int i;
    for (i = 0; i < n; i++)
        if (!tc_crt_proc(names[i])) tc_fail_expr(__FILE__, __LINE__, names[i]);
}
#define NEEDS(arr) need_all(arr, (int)(sizeof(arr) / sizeof((arr)[0])))

TC_CASE(atexit)
{
    static const char *const names[] = { "atexit" };
    NEEDS(names);
}

int main(void)
{
    tc_begin();
    TC_RUN(atexit);
    return tc_end();
}
