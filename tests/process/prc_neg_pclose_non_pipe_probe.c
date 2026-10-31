/* TC
 * id:    prc_neg_pclose_non_pipe_probe
 * area:  process
 * funcs: _pclose fopen fclose remove
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _pclose (stream must come from _popen)
 * note:  PROBE: _pclose on a FILE from fopen returns -1 (stream not found in the pipe table). Run in a child process in case it
 *        faults. Child exit codes: 10 = got -1 (expected), 11 = other return, 12 = fopen failed. errno value not asserted.
 */
#include "tc.h"
#include <stdio.h>

static const char *const fname = "tc_prc_neg_pclose_non_pipe_probe_0.tmp";

TC_CASE(pclose_file_fails)
{
    DWORD code = 0;
    DeleteFileA(fname);
    CHECK(tc_spawn_self("pcfile", &code, 20000));
    CHECK_EQ_U(code, 10);
}
TC_CASE(file_removed)
{
    CHECK_EQ_U(GetFileAttributesA(fname), INVALID_FILE_ATTRIBUTES);
    DeleteFileA(fname);
}
int main(void)
{
    tc_begin();
    if (tc_child_name() != NULL) {
        FILE *volatile f; int r;
        tc_iph_install();
        f = fopen(fname, "w");
        if (!f) return 12;
        r = _pclose(f);
        fclose(f);
        remove(fname);
        return r == -1 ? 10 : 11;
    }
    TC_RUN(pclose_file_fails); TC_RUN(file_removed);
    return tc_end();
}
