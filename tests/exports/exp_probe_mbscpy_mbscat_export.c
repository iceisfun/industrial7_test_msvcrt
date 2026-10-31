/* TC
 * id:    exp_probe_mbscpy_mbscat_export
 * area:  exports
 * funcs: 
 * procs: _mbscpy _mbscat
 * crt:   msvcrt msvcr80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _mbscpy, _mbscat (deprecated in favour of _mbscpy_s/_mbscat_s)
 * note:  Probing: mingw import libs list _mbscpy/_mbscat only for msvcr80 and msvcrt; later DLLs (incl. ucrtbase) may map them to inline code or omit them, so only msvcrt and msvcr80 are listed.
 */
#include "tc.h"

static void need_all(const char *const *names, int n)
{
    int i;
    for (i = 0; i < n; i++)
        if (!tc_crt_proc(names[i])) tc_fail_expr(__FILE__, __LINE__, names[i]);
}
#define NEEDS(arr) need_all(arr, (int)(sizeof(arr) / sizeof((arr)[0])))

TC_CASE(mbscpy)
{
    static const char *const names[] = { "_mbscpy" };
    NEEDS(names);
}

TC_CASE(mbscat)
{
    static const char *const names[] = { "_mbscat" };
    NEEDS(names);
}

int main(void)
{
    tc_begin();
    TC_RUN(mbscpy);
    TC_RUN(mbscat);
    return tc_end();
}
