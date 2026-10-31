/* TC
 * id:    dir_probe_searchenv_path
 * area:  dir
 * funcs: _searchenv
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _searchenv
 * note:  Probes that a file located only through a directory listed in PATH is found: kernel32.dll is searched for in PATH
 *        (system directory is on PATH of any normal Windows) and the result is an absolute path ending in "\kernel32.dll".
 *        Depends on the machine's PATH containing the system directory.
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile v_fn = "kernel32.dll", *volatile v_var = "PATH";

TC_CASE(find_kernel32_on_path)
{
    char buf[_MAX_PATH]; int n;
    buf[0] = 0;
    _searchenv(v_fn, v_var, buf);
    n = lstrlenA(buf);
    CHECK(n > 13);
    if (n > 13) {
        CHECK(lstrcmpiA(buf + n - 13, "\\kernel32.dll") == 0);
        CHECK(buf[1] == ':' && buf[2] == '\\');
    }
}
int main(void) { tc_begin(); TC_RUN(find_kernel32_on_path); return tc_end(); }
