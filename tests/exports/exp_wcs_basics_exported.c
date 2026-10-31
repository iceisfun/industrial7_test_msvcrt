/* TC
 * id:    exp_wcs_basics_exported
 * area:  exports
 * funcs: 
 * procs: wcslen wcscpy wcsncpy wcscat wcsncat wcscmp wcsncmp wcschr wcsrchr wcsstr wcsspn wcscspn wcspbrk wcstok wcscoll wcsxfrm _wcsdup _wcsicmp _wcsnicmp _wcslwr _wcsupr _wcsrev _wcsnset _wcsset wcsftime wcstombs mbstowcs wctomb mbtowc
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs wcs* functions
 * note:  Plain export presence via GetProcAddress.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <malloc.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#define HAS(n) do { if (!tc_crt_proc(n)) tc_fail_expr(__FILE__, __LINE__, "not exported: " n); } while (0)
#define HASNOT(n) do { if (tc_crt_proc(n)) tc_fail_expr(__FILE__, __LINE__, "unexpectedly exported: " n); } while (0)


TC_CASE(copy_group)
{
    HAS("wcscpy");
    HAS("wcsncpy");
    HAS("wcscat");
    HAS("wcsncat");
    HAS("_wcsdup");
}

TC_CASE(compare_group)
{
    HAS("wcscmp");
    HAS("wcsncmp");
    HAS("wcscoll");
    HAS("wcsxfrm");
    HAS("_wcsicmp");
    HAS("_wcsnicmp");
}

TC_CASE(search_group)
{
    HAS("wcslen");
    HAS("wcschr");
    HAS("wcsrchr");
    HAS("wcsstr");
    HAS("wcsspn");
    HAS("wcscspn");
    HAS("wcspbrk");
    HAS("wcstok");
}

TC_CASE(case_group)
{
    HAS("_wcslwr");
    HAS("_wcsupr");
    HAS("_wcsrev");
    HAS("_wcsnset");
    HAS("_wcsset");
}

TC_CASE(conversion_group)
{
    HAS("wcsftime");
    HAS("wcstombs");
    HAS("mbstowcs");
    HAS("wctomb");
    HAS("mbtowc");
}

int main(void)
{
    tc_begin();
    TC_RUN(copy_group);
    TC_RUN(compare_group);
    TC_RUN(search_group);
    TC_RUN(case_group);
    TC_RUN(conversion_group);
    return tc_end();
}
