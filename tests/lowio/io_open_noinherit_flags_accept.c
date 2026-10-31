/* TC
 * id:    io_open_noinherit_flags_accept
 * area:  lowio
 * funcs: _open _close _unlink _get_osfhandle
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _open (_O_NOINHERIT, _O_SEQUENTIAL, _O_RANDOM, _O_SHORT_LIVED)
 * note:  Default descriptors being inheritable (CRT passes bInheritHandle=TRUE unless _O_NOINHERIT) is known behaviour, docs only state that _O_NOINHERIT prevents inheritance.
 *        Inheritance is verified through GetHandleInformation(HANDLE_FLAG_INHERIT) on the OS handle.
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define TCV TCS(TC_CRT_VER) /* per-CRT file names: targets may run concurrently in one directory */
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static const char *volatile fn = "tc_io_open_noinherit_" TCV ".tmp";

static int open_with(int flags)
{
    return _open(fn, _O_CREAT | _O_RDWR | _O_BINARY | flags, _S_IREAD | _S_IWRITE);
}
static int inherit_flag(int fd)
{
    DWORD fl = 0;
    HANDLE h = (HANDLE)_get_osfhandle(fd);
    if (h == INVALID_HANDLE_VALUE || !GetHandleInformation(h, &fl)) return -1;
    return (fl & HANDLE_FLAG_INHERIT) ? 1 : 0;
}
static void one(int flags, int want_inherit)
{
    int fd;
    _unlink(fn);
    fd = open_with(flags);
    CHECK(fd >= 0);
    if (fd >= 0) { CHECK_EQ_I(inherit_flag(fd), want_inherit); CHECK_EQ_I(_close(fd), 0); }
    _unlink(fn);
}
TC_CASE(default_inherits)  { one(0, 1); }
TC_CASE(noinherit)         { one(_O_NOINHERIT, 0); }
TC_CASE(sequential)        { one(_O_SEQUENTIAL, 1); }
TC_CASE(random_access)     { one(_O_RANDOM, 1); }
TC_CASE(short_lived)       { one(_O_SHORT_LIVED, 1); }
TC_CASE(noinherit_seq)     { one(_O_NOINHERIT | _O_SEQUENTIAL, 0); }

int main(void)
{
    tc_begin();
    TC_RUN(default_inherits); TC_RUN(noinherit); TC_RUN(sequential);
    TC_RUN(random_access); TC_RUN(short_lived); TC_RUN(noinherit_seq);
    return tc_end();
}
