/* TC
 * id:    exp_probe_wcreate_locale_old
 * area:  exports
 * funcs: 
 * procs: _wcreate_locale
 * crt:   msvcr90 msvcr100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _create_locale (VS2005+)
 * note:  Probing: whether msvcr90.dll and msvcr100.dll export _wcreate_locale (docs say VS2005+; mingw import libs omit it).
 */
#include "tc.h"

static void need_all(const char *const *names, int n)
{
    int i;
    for (i = 0; i < n; i++)
        if (!tc_crt_proc(names[i])) tc_fail_expr(__FILE__, __LINE__, names[i]);
}
#define NEEDS(arr) need_all(arr, (int)(sizeof(arr) / sizeof((arr)[0])))

TC_CASE(each_resolves)
{
    static const char *const names[] = { "_wcreate_locale" };
    NEEDS(names);
}

int main(void)
{
    tc_begin();
    TC_RUN(each_resolves);
    return tc_end();
}
