/* TC
 * id:    exp_locale_internals_exported
 * area:  exports
 * funcs: 
 * procs: ___mb_cur_max_func __pctype_func __pwctype_func ___lc_codepage_func ___lc_collate_cp_func ___lc_locale_name_func
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p1
 * conf:  observed
 * ref:   stdlib.h MB_CUR_MAX / ctype.h macros expand to these accessors
 * note:  msvcr100 omitted because ___lc_locale_name_func is unsure there.
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
    static const char *const names[] = { "___mb_cur_max_func", "__pctype_func", "__pwctype_func", "___lc_codepage_func", "___lc_collate_cp_func", "___lc_locale_name_func" };
    NEEDS(names);
}

int main(void)
{
    tc_begin();
    TC_RUN(each_resolves);
    return tc_end();
}
