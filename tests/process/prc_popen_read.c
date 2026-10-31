/* TC
 * id:    prc_popen_read
 * area:  process
 * funcs: _popen _pclose
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _popen/_pclose
 */
#include "tc.h"
#include <stdio.h>
TC_CASE(read_echo)
{
    char line[64] = ""; char *g; int r;
    FILE *volatile f = _popen("cmd /c echo hello", "r");
    CHECK(f != NULL);
    if (!f) return;
    g = fgets(line, sizeof line, f);
    CHECK(g != NULL);
    CHECK_STR(line, "hello\n");
    r = _pclose(f);
    CHECK_EQ_I(r, 0);
}
int main(void) { tc_begin(); TC_RUN(read_echo); return tc_end(); }
