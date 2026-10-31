/* TC
 * id:    gdr_neg_stat32_default_fatal
 * area:  dir
 * kind:  negative
 * funcs: _stat32 _fstat32
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs "Parameter validation": with the default invalid-parameter handler the process terminates
 * note:  PROBE: in a child process with no handler installed, _stat32(NULL, &st) and _fstat32(-1, &st) terminate the
 *        process (ucrtbase default handler raises a fast-fail, exit status expected 0xC0000409; we assert only that
 *        the child neither returns normally (77) nor exits 0).
 */
#include "tc.h"
#include <sys/types.h>
#include <sys/stat.h>

static const char *volatile v_path;
static volatile int v_fd;

TC_CASE(null_path_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("nullpath", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
TC_CASE(bad_fd_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("badfd", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        struct _stat32 st; v_path = NULL; v_fd = -1;
        if (c[0] == 'n') _stat32(v_path, &st); else _fstat32(v_fd, &st);
        return 77;
    }
    TC_RUN(null_path_terminates); TC_RUN(bad_fd_terminates);
    return tc_end();
}
