/* TC
 * id:    str_strlwr_l_basic
 * area:  string
 * funcs: _strlwr_l _strupr_l _create_locale _free_locale
 * procs: 
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _strlwr_l, _strupr_l

 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <locale.h>
#include <stdlib.h>
static _locale_t loc;
TC_CASE(lower) { char b[16] = "HeLLo 1"; _strlwr_l(b, loc); CHECK_STR(b, "hello 1"); }
TC_CASE(upper) { char b[16] = "HeLLo 1"; _strupr_l(b, loc); CHECK_STR(b, "HELLO 1"); }
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL create_locale\n"); return 2; }
    TC_RUN(lower); TC_RUN(upper);
    _free_locale(loc);
    return tc_end();
}
