/* TC
 * id:    exp_ucrt_conio_common_exported
 * area:  exports
 * funcs: 
 * procs: __conio_common_vcprintf __conio_common_vcwprintf __conio_common_vcscanf __conio_common_vcwscanf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   conio.h inline _cprintf/_cwprintf/_cscanf expand to these
 * note:  Resolve only.
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
    static const char *const names[] = { "__conio_common_vcprintf", "__conio_common_vcwprintf", "__conio_common_vcscanf", "__conio_common_vcwscanf" };
    NEEDS(names);
}

int main(void)
{
    tc_begin();
    TC_RUN(each_resolves);
    return tc_end();
}
