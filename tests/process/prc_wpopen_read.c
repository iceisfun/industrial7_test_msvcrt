/* TC
 * id:    prc_wpopen_read
 * area:  process
 * funcs: _wpopen _pclose
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _popen, _wpopen
 */
#include "tc.h"
#include <stdio.h>
TC_CASE(read_hi)
{
    char line[64] = ""; int r;
    FILE *volatile f = _wpopen(L"cmd /c echo hi", L"r");
    CHECK(f != NULL);
    if (!f) return;
    CHECK(fgets(line, sizeof line, f) != NULL);
    CHECK_STR(line, "hi\n");
    r = _pclose(f);
    CHECK_EQ_I(r, 0);
}
int main(void) { tc_begin(); TC_RUN(read_hi); return tc_end(); }
