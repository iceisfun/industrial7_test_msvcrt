/* TC
 * id:    exp_locale_l_string_exported
 * area:  exports
 * funcs: 
 * procs: _strcoll_l _stricoll_l _strnicoll_l _strncoll_l _strxfrm_l _strlwr_l _strupr_l _stricmp_l _strnicmp_l _mbstowcs_l _wcstombs_l _wcstombs_s_l _mbtowc_l _wctomb_l _atoi_l _atof_l _strtod_l _strtol_l _strtoul_l _wcsicmp_l _wcscoll_l _wcsxfrm_l
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs Locale-dependent routines (VS2005+)
 */
#include "tc.h"

static void need_all(const char *const *names, int n)
{
    int i;
    for (i = 0; i < n; i++)
        if (!tc_crt_proc(names[i])) tc_fail_expr(__FILE__, __LINE__, names[i]);
}
#define NEEDS(arr) need_all(arr, (int)(sizeof(arr) / sizeof((arr)[0])))

TC_CASE(collate_group)
{
    static const char *const names[] = { "_strcoll_l", "_stricoll_l", "_strnicoll_l", "_strncoll_l", "_strxfrm_l" };
    NEEDS(names);
}

TC_CASE(case_group)
{
    static const char *const names[] = { "_strlwr_l", "_strupr_l", "_stricmp_l", "_strnicmp_l" };
    NEEDS(names);
}

TC_CASE(convert_group)
{
    static const char *const names[] = { "_mbstowcs_l", "_wcstombs_l", "_wcstombs_s_l", "_mbtowc_l", "_wctomb_l" };
    NEEDS(names);
}

TC_CASE(strtonum_group)
{
    static const char *const names[] = { "_atoi_l", "_atof_l", "_strtod_l", "_strtol_l", "_strtoul_l" };
    NEEDS(names);
}

TC_CASE(wide_group)
{
    static const char *const names[] = { "_wcsicmp_l", "_wcscoll_l", "_wcsxfrm_l" };
    NEEDS(names);
}

int main(void)
{
    tc_begin();
    TC_RUN(collate_group);
    TC_RUN(case_group);
    TC_RUN(convert_group);
    TC_RUN(strtonum_group);
    TC_RUN(wide_group);
    return tc_end();
}
