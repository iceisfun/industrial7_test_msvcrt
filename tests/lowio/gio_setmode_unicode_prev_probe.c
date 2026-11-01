/* TC
 * id:    gio_setmode_unicode_prev_probe
 * area:  lowio
 * funcs: _setmode _open _close _unlink
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _setmode ("returns the previous translation mode")
 * note:  OBSERVED on Win10 22H2 ucrtbase: after _setmode(fd,_O_U8TEXT) the next _setmode returns the exact previous mode
 *        _O_U8TEXT (0x40000); after _O_U16TEXT it returns _O_WTEXT (0x10000), i.e. U16TEXT is reported as WTEXT but U8TEXT is
 *        not collapsed.
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define TCV TCS(TC_CRT_VER)
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#ifndef _O_WTEXT
#define _O_WTEXT 0x10000
#endif
#ifndef _O_U16TEXT
#define _O_U16TEXT 0x20000
#endif
#ifndef _O_U8TEXT
#define _O_U8TEXT 0x40000
#endif

static const char *volatile fn = "tc_gio_setmode_unicode_probe_" TCV ".tmp";
static int fd = -1;
static int open_fd(void)
{
    _unlink(fn);
    fd = _open(fn, _O_CREAT | _O_TRUNC | _O_RDWR | _O_TEXT, _S_IREAD | _S_IWRITE);
    return fd >= 3;
}
static void done(void) { if (fd >= 0) _close(fd); fd = -1; _unlink(fn); }

TC_CASE(u8text_prev_value)
{
    CHECK(open_fd()); if (fd < 0) return;
    CHECK_EQ_I(_setmode(fd, _O_U8TEXT), _O_TEXT);
    CHECK_EQ_I(_setmode(fd, _O_BINARY), _O_U8TEXT);
    done();
}
TC_CASE(u16text_prev_value)
{
    CHECK(open_fd()); if (fd < 0) return;
    CHECK_EQ_I(_setmode(fd, _O_U16TEXT), _O_TEXT);
    CHECK_EQ_I(_setmode(fd, _O_BINARY), _O_WTEXT);
    done();
}
int main(void)
{
    tc_begin();
    TC_RUN(u8text_prev_value); TC_RUN(u16text_prev_value);
    return tc_end();
}
