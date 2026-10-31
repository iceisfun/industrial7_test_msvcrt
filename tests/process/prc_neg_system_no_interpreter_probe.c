/* TC
 * id:    prc_neg_system_no_interpreter_probe
 * area:  process
 * kind:  negative
 * funcs: system _putenv
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs system: system(NULL) returns 0 if no interpreter; -1 with errno ENOENT if it cannot be found
 * note:  Runs in a child (modifies environment and cwd). The child points COMSPEC at a missing file, empties PATH and
 *        chdirs into an empty directory, then reports bit0 = (system(NULL)==0), bit1 = (system("exit 3")==-1),
 *        bit2 = (errno==ENOENT) in its exit code, plus 0x10. Expected 0x17.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

#define TC_STR2(x) #x
#define TC_STR(x) TC_STR2(x)
#define DIRNAME "tc_prc_neg_nointerp_" TC_STR(TC_CRT_VER) ".tmp"

static int child_main(void)
{
    int code = 0x10, r;
    _putenv("COMSPEC=C:\\no\\such\\cmd.exe");
    _putenv("PATH=");
    if (!SetCurrentDirectoryA(DIRNAME)) return 0x70;
    { const char *volatile n = NULL; if (system(n) == 0) code |= 1; }
    errno = 0;
    { const char *volatile c = "exit 3"; r = system(c); }
    if (r == -1) code |= 2;
    if (errno == ENOENT) code |= 4;
    return code;
}

TC_CASE(no_interpreter)
{
    DWORD code = 0; int ran;
    CreateDirectoryA(DIRNAME, NULL);
    ran = tc_spawn_self("nointerp", &code, 20000);
    RemoveDirectoryA(DIRNAME);
    CHECK(ran);
    CHECK_EQ_U(code & 1, 1);   /* system(NULL) == 0 */
    CHECK_EQ_U(code & 2, 2);   /* system("exit 3") == -1 */
    CHECK_EQ_U(code & 4, 4);   /* errno == ENOENT */
    CHECK_EQ_U(code, 0x17);
}

int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main();
    TC_RUN(no_interpreter);
    return tc_end();
}
