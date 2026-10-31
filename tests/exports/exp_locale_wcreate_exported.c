/* TC
 * id:    exp_locale_wcreate_exported
 * area:  exports
 * funcs: 
 * procs: _wcreate_locale
 * crt:   >=110
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _create_locale, _wcreate_locale (VS2005+; here only the generations where it is certain)
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
