/* TC
 * id:    io_wsopen_s_basic
 * area:  lowio
 * funcs: _wsopen_s _close _wunlink _write _read
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _sopen_s, _wsopen_s
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define TCV TCS(TC_CRT_VER) /* per-CRT file names: targets may run concurrently in one directory */
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <share.h>
#include <wchar.h>
#include <sys/stat.h>

static const wchar_t *volatile wfn = L"tc_io_wsopen_s_basic_\u00e9_" TCV ".tmp";

TC_CASE(create_reopen_unlink)
{
    int fh = -99, fh2 = -99; char b[4] = {0};
    _wunlink(wfn);
    CHECK_EQ_I(_wsopen_s(&fh, wfn, _O_CREAT | _O_WRONLY | _O_BINARY, _SH_DENYNO, _S_IREAD | _S_IWRITE), 0);
    CHECK(fh >= 0);
    if (fh >= 0) { CHECK_EQ_I(_write(fh, "hi", 2), 2); CHECK_EQ_I(_close(fh), 0); }
    CHECK_EQ_I(_wsopen_s(&fh2, wfn, _O_RDONLY | _O_BINARY, _SH_DENYNO, 0), 0);
    CHECK(fh2 >= 0);
    if (fh2 >= 0) { CHECK_EQ_I(_read(fh2, b, 4), 2); CHECK_MEM(b, "hi", 2); CHECK_EQ_I(_close(fh2), 0); }
    CHECK_EQ_I(_wunlink(wfn), 0);
}
int main(void) { tc_begin(); TC_RUN(create_reopen_unlink); return tc_end(); }
