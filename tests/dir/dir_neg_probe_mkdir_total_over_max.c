/* TC
 * id:    dir_neg_probe_mkdir_total_over_max
 * area:  dir
 * funcs: _mkdir _rmdir
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   Win32 naming files (MAX_PATH); MS docs _mkdir
 * note:  PROBE: _mkdir of cwd + a 255-char new component (total path > 260) returns -1 because the test exe has no
 *        longPathAware manifest. A system/process with long-path opt-in might succeed; if it does the dir is removed.
 */
#include "tc.h"
#include <direct.h>

static const char *volatile v_name;

TC_CASE(total_over_260)
{
    char cwd[MAX_PATH], name[300]; int i, L; int r;
    L = (int)GetCurrentDirectoryA(MAX_PATH, cwd);
    if (!L || L + 256 <= 260) TC_SKIP("cwd too short to exceed 260");
    for (i = 0; i < 255; i++) name[i] = 'o';
    name[0] = 't'; name[1] = 'c'; name[2] = '_'; name[255] = 0;
    v_name = name;
    r = _mkdir(v_name);
    if (r == 0) _rmdir(v_name);
    CHECK_EQ_I(r, -1);
}

int main(void) { tc_begin(); TC_RUN(total_over_260); return tc_end(); }
