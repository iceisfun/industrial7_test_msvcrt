/* TC
 * id:    gsp_popen_pclose_oldname_read
 * area:  process
 * funcs: _popen _pclose
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _popen: creates a pipe and runs the command via cmd.exe; _pclose returns the command's exit status
 * note:  Unprefixed popen/pclose (oldnames thunks to _popen/_pclose).
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(read_echo)
{
    char line[64] = ""; int r;
    FILE *volatile f = popen("cmd /c echo hi", "r");
    CHECK(f != NULL);
    if (!f) return;
    CHECK(fgets(line, sizeof line, f) != NULL);
    CHECK(line[0] == 'h' && line[1] == 'i');
    r = pclose(f);
    CHECK_EQ_I(r, 0);
}
TC_CASE(exit_status_3)
{
    FILE *volatile f = popen("cmd /c exit 3", "r");
    CHECK(f != NULL);
    if (!f) return;
    CHECK_EQ_I(pclose(f), 3);
}
TC_CASE(modes_rt_rb)
{
    FILE *volatile f = popen("cmd /c exit 0", "rt");
    CHECK(f != NULL);
    if (f) CHECK_EQ_I(pclose(f), 0);
    f = popen("cmd /c exit 0", "rb");
    CHECK(f != NULL);
    if (f) CHECK_EQ_I(pclose(f), 0);
}
TC_CASE(status_after_eof)
{
    char buf[64]; FILE *volatile f = popen("cmd /c echo x& exit 6", "r");
    CHECK(f != NULL);
    if (!f) return;
    while (fgets(buf, sizeof buf, f)) ;
    CHECK(feof(f) != 0);
    CHECK_EQ_I(pclose(f), 6);
}
int main(void)
{
    tc_begin();
    TC_RUN(read_echo); TC_RUN(exit_status_3); TC_RUN(modes_rt_rb); TC_RUN(status_after_eof);
    return tc_end();
}
