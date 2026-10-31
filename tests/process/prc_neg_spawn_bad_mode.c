/* TC
 * id:    prc_neg_spawn_bad_mode
 * area:  process
 * funcs: _spawnl
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _spawn functions (EINVAL: invalid mode argument); parameter validation
 * note:  PROBE: an invalid mode (99) makes _spawnl return -1 with errno EINVAL and call the invalid-parameter handler.
 *        The target deliberately does not exist (safe even if a CRT skipped mode validation); if a CRT searches for the file
 *        before validating the mode the result is ENOENT instead - that would be the finding.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

TC_CASE(bad_mode)
{
    /* nonexistent target: if a CRT ignored the mode and tried to launch, nothing (e.g. this test itself) is started */
    const char *exe = "C:\\tc_no_such_dir\\x.exe";
    if (!tc_iph_install()) TC_SKIP("no iph api");
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(_spawnl(99, exe, "x.exe", (char *)NULL), -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(bad_mode); return tc_end(); }
