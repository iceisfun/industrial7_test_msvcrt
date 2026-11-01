/* TC
 * id:    prc_neg_spawn_bad_mode
 * area:  process
 * funcs: _spawnl
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _spawn functions (EINVAL: invalid mode argument); parameter validation
 * note:  Two cases. PROBE: an invalid mode (99) makes _spawnl return -1 with errno EINVAL and call the invalid-parameter handler once.
 *        Win10 22H2 19045 ucrtbase: true when the target is a real executable (probe: cmd.exe -> -1, EINVAL, handler 1); with
 *        the old NONEXISTENT target the CRT never gets to the mode check and returns -1/ENOENT with no handler call (old
 *        expectation wrong). Target is now %SystemRoot%\\System32\\cmd.exe /c exit 0.
 *        Second case (kept as the finding, observed twice): with a NONEXISTENT target the CRT searches for the file before it
 *        validates the mode, so _spawnl(99, missing) is -1, errno ENOENT, handler count 0 (probe spawn99_none_r=-1 errno=2 iph=0).
 *        Not stated by the MS docs; stable across runs, independent of locale/CPU/build as far as observed.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

TC_CASE(bad_mode)
{
    /* observed: the mode is validated only after the target has been located - a NONEXISTENT target gives ENOENT (-1, no handler call) and
     * never reaches the mode check, so the target must be a real executable. cmd.exe /c exit 0 is harmless even if a CRT ignored the mode. */
    char exe[MAX_PATH + 16]; UINT n;
    n = GetSystemDirectoryA(exe, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) TC_SKIP("GetSystemDirectory failed");
    { const char *s = "\\cmd.exe"; UINT i = 0; while (s[i]) { exe[n + i] = s[i]; i++; } exe[n + i] = 0; }
    if (!tc_iph_install()) TC_SKIP("no iph api");
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_spawnl(99, exe, "cmd.exe", "/c", "exit", "0", (char *)NULL), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(bad_mode_missing_target_enoent)
{
    /* observed: the target lookup happens before the mode check; a missing file gives ENOENT and never calls the handler */
    if (!tc_iph_install()) TC_SKIP("no iph api");
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_spawnl(99, "C:\\tc_no_such_dir_wp4\\no_such_prog.exe", "no_such_prog.exe", (char *)NULL), -1);
    CHECK_EQ_I(errno, ENOENT);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(bad_mode); TC_RUN(bad_mode_missing_target_enoent); return tc_end(); }
