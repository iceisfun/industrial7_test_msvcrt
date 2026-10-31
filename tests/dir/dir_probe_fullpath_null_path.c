/* TC
 * id:    dir_probe_fullpath_null_path
 * area:  dir
 * funcs: _fullpath
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _fullpath (a NULL/empty path is treated as the current directory in the CRT implementation)
 * note:  PROBE: ucrtbase _fullpath(buf, NULL, MAX_PATH) returns buf holding the current directory, without calling
 *        the invalid-parameter handler. Documentation does not state this.
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile v_null;

TC_CASE(null_path_is_cwd)
{
    char buf[MAX_PATH], cwd[MAX_PATH];
    if (!GetCurrentDirectoryA(MAX_PATH, cwd)) TC_SKIP("cwd");
    tc_iph_install(); tc_iph_count = 0;
    CHECK(_fullpath(buf, v_null, MAX_PATH) == buf);
    CHECK_STR(buf, cwd);
    CHECK_EQ_I(tc_iph_count, 0);
}

int main(void) { tc_begin(); TC_RUN(null_path_is_cwd); return tc_end(); }
