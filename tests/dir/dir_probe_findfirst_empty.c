/* TC
 * id:    dir_probe_findfirst_empty
 * area:  dir
 * kind:  positive
 * funcs: _findclose
 * procs: _findfirst64i32
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _findfirst
 * note:  Probes an empty filespec string: expected return -1 with errno ENOENT (FindFirstFile fails with
 *        ERROR_PATH_NOT_FOUND / ERROR_FILE_NOT_FOUND, both map to ENOENT). Not documented explicitly.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

typedef intptr_t (__cdecl *ff_t)(const char *, struct _finddata64i32_t *);
static const char *volatile v_spec;

TC_CASE(empty_spec)
{
    ff_t ff = (ff_t)tc_crt_proc("_findfirst64i32"); struct _finddata64i32_t fd; intptr_t h;
    if (!ff) TC_SKIP("not exported");
    v_spec = ""; errno = 0; h = ff(v_spec, &fd);
    CHECK_EQ_I(h, -1); CHECK_EQ_I(errno, ENOENT);
    if (h != -1) _findclose(h);
}
int main(void) { tc_begin(); TC_RUN(empty_spec); return tc_end(); }
