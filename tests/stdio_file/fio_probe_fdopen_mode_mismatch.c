/* TC
 * id:    fio_probe_fdopen_mode_mismatch
 * area:  stdio_file
 * funcs: _fdopen fwrite fflush fclose
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _fdopen
 * note:  PROBE: fd opened O_RDONLY then _fdopen(fd,"w"). Expectation: _fdopen succeeds (no consistency check) and fwrite+fflush fails (ferror set, or fwrite short). Verify on real Windows.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

static const char * volatile fn = "tc_fio_probe_fdopen_mode_mismatch_a.tmp";

TC_CASE(mismatch)
{
    int fd; FILE *f; size_t n; int fr;
    f = fopen(fn, "wb"); CHECK(f != NULL); if (!f) return;
    fputc('z', f); fclose(f);
    fd = _open(fn, _O_RDONLY | _O_BINARY);
    CHECK(fd >= 0); if (fd < 0) return;
    f = _fdopen(fd, "w");
    CHECK(f != NULL);
    if (!f) { _close(fd); return; }
    n = fwrite("abc", 1, 3, f);
    fr = fflush(f);
    CHECK(n < 3 || fr != 0);
    CHECK(ferror(f) != 0);
    clearerr(f);
    fclose(f);
}
int main(void)
{
    tc_begin();
    DeleteFileA(fn);
    TC_RUN(mismatch);
    DeleteFileA(fn);
    return tc_end();
}
