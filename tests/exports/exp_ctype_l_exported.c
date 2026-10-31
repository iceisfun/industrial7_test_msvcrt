/* TC
 * id:    exp_ctype_l_exported
 * area:  exports
 * funcs: 
 * procs: _isalpha_l _isdigit_l _isalnum_l _tolower_l _toupper_l _iswalpha_l _towlower_l _towupper_l
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs isalpha, _isalpha_l (VS2005+)
 * note:  Plain export presence.
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


TC_CASE(narrow_l)
{
    HAS("_isalpha_l");
    HAS("_isdigit_l");
    HAS("_isalnum_l");
}

TC_CASE(wide_l)
{
    HAS("_iswalpha_l");
}

TC_CASE(case_l)
{
    HAS("_tolower_l");
    HAS("_toupper_l");
    HAS("_towlower_l");
    HAS("_towupper_l");
}

int main(void)
{
    tc_begin();
    TC_RUN(narrow_l);
    TC_RUN(wide_l);
    TC_RUN(case_l);
    return tc_end();
}
