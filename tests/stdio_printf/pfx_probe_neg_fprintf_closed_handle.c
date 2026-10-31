/* TC
 * id:    pfx_probe_neg_fprintf_closed_handle
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen fclose _fileno _get_osfhandle setvbuf ferror
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _get_osfhandle, errno mapping of Win32 errors
 * note:  Probing -1 / EBADF (mapped from ERROR_INVALID_HANDLE) and ferror nonzero when the OS handle is closed behind the CRT's back.
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <io.h>
#include <errno.h>

#define TMP "tc_pfx_probe_neg_fprintf_closed_handle_" TCV ".tmp"
static const char *volatile g_fmt = "hello %d";

TC_CASE(write_after_closehandle)
{
    FILE *f = fopen(TMP, "w"); int r;
    intptr_t h;
    CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(setvbuf(f, NULL, _IONBF, 0), 0);
    h = _get_osfhandle(_fileno(f));
    CHECK(h != -1);
    CHECK(CloseHandle((HANDLE)h));
    errno = 0;
    r = fprintf(f, g_fmt, 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK(ferror(f) != 0);
    fclose(f);          /* result not asserted: internal CloseHandle fails */
    DeleteFileA(TMP);
}
int main(void) { tc_begin(); TC_RUN(write_after_closehandle); return tc_end(); }
