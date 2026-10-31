/* TC
 * id:    gio_setmode_unicode_prev_wide
 * area:  lowio
 * funcs: _setmode _open _close _unlink
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _setmode: _O_WTEXT, _O_U8TEXT, _O_U16TEXT; returns the previous translation mode
 * note:  Exact previous-mode values for _O_U8TEXT/_O_U16TEXT are in gio_setmode_unicode_prev_probe; here a wide mode is only required to read back as a wide mode.
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

static const char *volatile fn = "tc_gio_setmode_unicode_prev_" TCV ".tmp";
static int fd = -1;
static int open_fd(void)
{
    _unlink(fn);
    fd = _open(fn, _O_CREAT | _O_TRUNC | _O_RDWR | _O_TEXT, _S_IREAD | _S_IWRITE);
    return fd >= 3;
}
static void done(void) { if (fd >= 0) _close(fd); fd = -1; _unlink(fn); }

TC_CASE(text_to_wtext_prev_text)
{
    CHECK(open_fd()); if (fd < 0) return;
    CHECK_EQ_I(_setmode(fd, _O_WTEXT), _O_TEXT);
    done();
}
TC_CASE(wtext_to_binary_prev_wtext)
{
    CHECK(open_fd()); if (fd < 0) return;
    _setmode(fd, _O_WTEXT);
    CHECK_EQ_I(_setmode(fd, _O_BINARY), _O_WTEXT);
    done();
}
static int wide_ok(int r, int m) { return r == m || r == _O_WTEXT; }
TC_CASE(cycle_all_five_modes_restores)
{
    CHECK(open_fd()); if (fd < 0) return;
    CHECK_EQ_I(_setmode(fd, _O_BINARY), _O_TEXT);
    CHECK_EQ_I(_setmode(fd, _O_WTEXT), _O_BINARY);
    CHECK(wide_ok(_setmode(fd, _O_U8TEXT), _O_WTEXT));
    CHECK(wide_ok(_setmode(fd, _O_U16TEXT), _O_U8TEXT));   /* previous is some wide mode */
    CHECK(wide_ok(_setmode(fd, _O_TEXT), _O_U16TEXT));
    CHECK_EQ_I(_setmode(fd, _O_TEXT), _O_TEXT);
    done();
}
int main(void)
{
    tc_begin();
    TC_RUN(text_to_wtext_prev_text); TC_RUN(wtext_to_binary_prev_wtext);
    TC_RUN(cycle_all_five_modes_restores);
    return tc_end();
}
