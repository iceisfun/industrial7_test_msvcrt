/* TC
 * id:    io_sopen_share_basic
 * area:  lowio
 * funcs: _sopen _close _unlink _write _read
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _sopen, _wsopen (_SH_DENYNO permits read and write sharing)
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define TCV TCS(TC_CRT_VER) /* per-CRT file names: targets may run concurrently in one directory */
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <share.h>
#include <sys/stat.h>

static const char *volatile fn = "tc_io_sopen_share_basic_" TCV ".tmp";

TC_CASE(two_denyno_opens)
{
    int a, b;
    char c = 0;
    _unlink(fn);
    a = _sopen(fn, _O_CREAT | _O_RDWR | _O_BINARY, _SH_DENYNO, _S_IREAD | _S_IWRITE);
    CHECK(a >= 0);
    b = _sopen(fn, _O_RDWR | _O_BINARY, _SH_DENYNO);
    CHECK(b >= 0);
    CHECK(a != b);
    if (a >= 0 && b >= 0) {
        CHECK_EQ_I(_write(a, "Q", 1), 1);
        CHECK_EQ_I(_read(b, &c, 1), 1);
        CHECK_EQ_I(c, 'Q');
    }
    if (b >= 0) CHECK_EQ_I(_close(b), 0);
    if (a >= 0) CHECK_EQ_I(_close(a), 0);
    CHECK_EQ_I(_unlink(fn), 0);
}
TC_CASE(denywr_then_read_denyno)
{
    int a, b;
    _unlink(fn);
    a = _sopen(fn, _O_CREAT | _O_RDONLY | _O_BINARY, _SH_DENYWR, _S_IREAD | _S_IWRITE);
    CHECK(a >= 0);
    b = _sopen(fn, _O_RDONLY | _O_BINARY, _SH_DENYNO);
    CHECK(b >= 0);
    if (b >= 0) _close(b);
    if (a >= 0) _close(a);
    _unlink(fn);
}
int main(void) { tc_begin(); TC_RUN(two_denyno_opens); TC_RUN(denywr_then_read_denyno); return tc_end(); }
