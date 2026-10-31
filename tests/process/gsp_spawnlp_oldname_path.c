/* TC
 * id:    gsp_spawnlp_oldname_path
 * area:  process
 * funcs: _spawnlp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _spawnlp: the p variants search the PATH environment variable
 * note:  Unprefixed spawnlp. Relies on cmd.exe being found through PATH (System32 is always on a default Windows PATH).
 *        Searching a name with the extension omitted is deliberately NOT asserted.
 */
#include "tc.h"
#include <process.h>

TC_CASE(path_search_cmd)
{
    intptr_t r = spawnlp(_P_WAIT, "cmd.exe", "cmd.exe", "/c", "exit 9", (char *)NULL);
    CHECK_EQ_I(r, 9);
}
TC_CASE(full_path_cmd)
{
    char cmd[MAX_PATH + 16]; UINT n = GetSystemDirectoryA(cmd, MAX_PATH); intptr_t r;
    lstrcpyA(cmd + n, "\\cmd.exe");
    r = spawnlp(_P_WAIT, cmd, cmd, "/c", "exit 8", (char *)NULL);
    CHECK_EQ_I(r, 8);
}
int main(void)
{
    tc_begin();
    TC_RUN(path_search_cmd); TC_RUN(full_path_cmd);
    return tc_end();
}
