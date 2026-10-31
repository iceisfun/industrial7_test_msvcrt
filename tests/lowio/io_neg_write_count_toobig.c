/* TC
 * id:    io_neg_write_count_toobig
 * area:  lowio
 * funcs: _write _open _close
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _write (count is unsigned int; no documented INT_MAX limit)
 * note:  PROBE: _write(fd, 16-byte buffer, UINT_MAX) is expected to fail with -1/EINVAL (count > INT_MAX
 *        validation) without touching the buffer. Run in a child process in case it is not validated.
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
static const char *fn = "tc_io_neg_write_count_toobig_" TC_S1(TC_CRT_VER) ".tmp";

static int child_main(void)
{
    char buf[16] = "0123456789abcde";
    volatile unsigned int cnt = UINT_MAX;
    int fd, r;
    tc_iph_install();
    fd = _open(fn, _O_WRONLY | _O_BINARY);
    if (fd < 0) return 90;
    errno = 0;
    r = _write(fd, buf, cnt);
    if (r == -1 && errno == EINVAL) return 100;
    if (r == -1) return 101;
    return 102;
}
TC_CASE(count_uint_max)
{
    DWORD code = 0;
    int fd, ran;
    fd = _open(fn, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_close(fd), 0);
    ran = tc_spawn_self("big", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 100);
    _unlink(fn);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return child_main();
    TC_RUN(count_uint_max);
    return tc_end();
}
