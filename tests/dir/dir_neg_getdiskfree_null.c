/* TC
 * id:    dir_neg_getdiskfree_null
 * area:  dir
 * funcs: _getdiskfree
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _getdiskfree
 * note:  PROBE: _getdiskfree(drive, NULL) is expected to fail with a nonzero return, errno EINVAL and one
 *        invalid-parameter handler call. The docs do not list the NULL-buffer case explicitly.
 *        crt narrowed to ucrtbase: old CRTs may dereference NULL (crash) rather than validate.
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

static volatile struct _diskfree_t *v_null;

TC_CASE(null_buffer)
{
    char cwd[MAX_PATH]; unsigned drive = 0; unsigned r;
    if (GetCurrentDirectoryA(MAX_PATH, cwd) && cwd[1] == ':') {
        char c = cwd[0]; if (c >= 'a' && c <= 'z') c = (char)(c - 'a' + 'A');
        drive = (unsigned)(c - 'A' + 1);
    }
    if (!tc_iph_install()) TC_SKIP("no iph api");
    tc_iph_count = 0; errno = 0;
    r = _getdiskfree(drive, (struct _diskfree_t *)v_null);
    CHECK(r != 0);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}

int main(void) { tc_begin(); TC_RUN(null_buffer); return tc_end(); }
