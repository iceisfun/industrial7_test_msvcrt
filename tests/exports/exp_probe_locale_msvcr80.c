/* TC
 * id:    exp_probe_locale_msvcr80
 * area:  exports
 * funcs: 
 * procs: _create_locale _free_locale _get_current_locale _configthreadlocale _wcreate_locale
 * crt:   msvcr80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _create_locale (VS2005)
 * note:  Probing: locale-object API in msvcr80.dll (VS2005); _wcreate_locale is the least certain.
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
    static const char *const names[] = { "_create_locale", "_free_locale", "_get_current_locale", "_configthreadlocale", "_wcreate_locale" };
    NEEDS(names);
}

int main(void)
{
    tc_begin();
    TC_RUN(each_resolves);
    return tc_end();
}
