/* TC
 * id:    io_sopen_s_basic
 * area:  lowio
 * funcs: _sopen_s _close _unlink _write
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _sopen_s, _wsopen_s (returns 0 on success and stores the descriptor in *pfh)
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

static const char *volatile fn = "tc_io_sopen_s_basic_" TCV ".tmp";

TC_CASE(create_and_reopen)
{
    int fh = -99, fh2 = -99;
    errno_t e;
    _unlink(fn);
    e = _sopen_s(&fh, fn, _O_CREAT | _O_RDWR | _O_BINARY, _SH_DENYNO, _S_IREAD | _S_IWRITE);
    CHECK_EQ_I(e, 0);
    CHECK(fh >= 0);
    if (fh >= 0) {
        CHECK_EQ_I(_write(fh, "ab", 2), 2);
        CHECK_EQ_I(_close(fh), 0);
    }
    e = _sopen_s(&fh2, fn, _O_RDONLY | _O_BINARY, _SH_DENYWR, 0);
    CHECK_EQ_I(e, 0);
    CHECK(fh2 >= 0);
    if (fh2 >= 0) CHECK_EQ_I(_close(fh2), 0);
    CHECK_EQ_I(_unlink(fn), 0);
}
TC_CASE(distinct_descriptors)
{
    int a = -1, b = -1;
    _unlink(fn);
    CHECK_EQ_I(_sopen_s(&a, fn, _O_CREAT | _O_RDWR | _O_BINARY, _SH_DENYNO, _S_IREAD | _S_IWRITE), 0);
    CHECK_EQ_I(_sopen_s(&b, fn, _O_RDWR | _O_BINARY, _SH_DENYNO, 0), 0);
    CHECK(a >= 0 && b >= 0 && a != b);
    if (a >= 0) _close(a);
    if (b >= 0) _close(b);
    _unlink(fn);
}
int main(void) { tc_begin(); TC_RUN(create_and_reopen); TC_RUN(distinct_descriptors); return tc_end(); }
