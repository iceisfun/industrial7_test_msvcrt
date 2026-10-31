/* TC
 * id:    io_tell_text_crlf
 * area:  lowio
 * funcs: _tell _write _open _close
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _write: in text mode LF is expanded to CR-LF; _tell reports the file position
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <winioctl.h>
#define TC_STR2(x) #x
#define TC_STR(x) TC_STR2(x)

static const char *volatile fname = "tc_io_tell_text_crlf_" TC_STR(TC_CRT_VER) ".tmp";
TC_CASE(text_lf_expands)
{
    int fd = _open(fname, _O_CREAT | _O_TRUNC | _O_RDWR | _O_TEXT, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "a\n", 2), 2);   /* return counts input bytes consumed (probe-free: docs say CR not counted) */
    CHECK_EQ_I(_tell(fd), 3);
    _close(fd);
}
TC_CASE(binary_no_expand)
{
    int fd = _open(fname, _O_CREAT | _O_TRUNC | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "a\n", 2), 2);
    CHECK_EQ_I(_tell(fd), 2);
    _close(fd);
}
int main(void)
{
    tc_begin();
    TC_RUN(text_lf_expands); TC_RUN(binary_no_expand);
    _unlink(fname);
    return tc_end();
}
