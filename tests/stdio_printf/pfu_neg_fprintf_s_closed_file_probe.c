/* TC
 * id:    pfu_neg_fprintf_s_closed_file_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf_s
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.3p4 (use of a closed FILE is undefined)
 * note:  Probing exactly: what a use-after-fclose fprintf_s does on UCRT. Done in a CHILD process only. The child installs a
 *        counting invalid-parameter handler and exits 0 if fprintf_s returned -1, 77 if it returned anything else; a crash
 *        or fast-fail shows up as its NTSTATUS exit code. The parent only asserts that the child ran to an exit and that
 *        the code is not the setup-failure value 78 (use after fclose is UB, so no outcome is asserted); the actual code is printed as an INFO line.
 *        Temp file tc_pfu_neg_fprintf_s_closed_*.tmp, removed.
 */
#include "tc.h"
#include <stdio.h>

#define FN "tc_pfu_neg_fprintf_s_closed_" TC_CRT_DLL ".tmp"

static int child_main(const char *name)
{
    FILE *f; int r;
    (void)name;
    tc_iph_install();
    f = fopen(FN, "wb"); if (!f) return 78;
    fclose(f);
    r = fprintf_s(f, "x%d", 1);
    return r == -1 ? 0 : 77;
}
TC_CASE(child_after_fclose)
{
    DWORD code = 0; int ran = tc_spawn_self("closed", &code, 20000); char h[24];
    CHECK(ran);
    tc_hex(code, h); tc_write("TC INFO " TC_TEST_ID " child exit code "); tc_write(h); tc_write("\n");
    CHECK(code != 78);   /* 78 = child setup failure; any other outcome (UB per C11) is informational */
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(child_after_fclose);
    DeleteFileA(FN);
    return tc_end();
}
