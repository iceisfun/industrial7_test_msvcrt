/* TC
 * id:    fio_neg_fdopen_bad_mode
 * area:  stdio_file
 * funcs: _fdopen
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _fdopen (parameter validation not explicit)
 * note:  Invalid first mode character and empty mode.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static const char * volatile fn = "tc_fio_neg_fdopen_bad_mode_a.tmp";
static const char * volatile m_z = "z";
static const char * volatile m_empty = "";

TC_CASE(bad_modes)
{
    int fd = _open(fn, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    FILE *f;
    CHECK(fd >= 0); if (fd < 0) return;
    tc_iph_count = 0; errno = 0;
    f = _fdopen(fd, m_z);
    CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL);
    if (f) { fclose(f); return; }
    errno = 0;
    f = _fdopen(fd, m_empty);
    CHECK(f == NULL); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 2);
    if (f) fclose(f); else _close(fd);
}
int main(void)
{
    tc_begin();
    DeleteFileA(fn);
    CHECK(tc_iph_install());
    TC_RUN(bad_modes);
    DeleteFileA(fn);
    return tc_end();
}
