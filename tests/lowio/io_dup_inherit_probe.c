/* TC
 * id:    io_dup_inherit_probe
 * area:  lowio
 * funcs: _dup _open _close _get_osfhandle _unlink
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _dup (creates a second descriptor for an open file)
 * note:  PROBE: (1) _dup'ed fd maps to a different OS handle (DuplicateHandle); (2) the duplicate handle is inheritable
 *        (CRT calls DuplicateHandle with bInheritHandle=TRUE), even if the source was opened with _O_NOINHERIT.
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

static const char *volatile fn = "tc_io_dup_inherit.tmp";

static int inh(int fd)
{
    DWORD fl = 0; HANDLE h = (HANDLE)_get_osfhandle(fd);
    if (h == INVALID_HANDLE_VALUE || !GetHandleInformation(h, &fl)) return -1;
    return (fl & HANDLE_FLAG_INHERIT) ? 1 : 0;
}
TC_CASE(handle_differs)
{
    int fd, d; _unlink(fn);
    fd = _open(fn, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    d = _dup(fd);
    CHECK(d >= 0 && d != fd);
    if (d >= 0) { CHECK(_get_osfhandle(d) != _get_osfhandle(fd)); _close(d); }
    _close(fd); _unlink(fn);
}
TC_CASE(dup_inheritable)
{
    int fd, d; _unlink(fn);
    fd = _open(fn, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    d = _dup(fd);
    CHECK(d >= 0);
    if (d >= 0) { CHECK_EQ_I(inh(d), 1); _close(d); }
    _close(fd); _unlink(fn);
}
TC_CASE(dup_of_noinherit)
{
    int fd, d; _unlink(fn);
    fd = _open(fn, _O_CREAT | _O_RDWR | _O_BINARY | _O_NOINHERIT, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(inh(fd), 0);
    d = _dup(fd);
    CHECK(d >= 0);
    if (d >= 0) { CHECK_EQ_I(inh(d), 1); _close(d); }
    _close(fd); _unlink(fn);
}
int main(void) { tc_begin(); TC_RUN(handle_differs); TC_RUN(dup_inheritable); TC_RUN(dup_of_noinherit); return tc_end(); }
