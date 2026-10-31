/* TC
 * id:    pfx_neg_probe_fopen_wildcard_name
 * area:  stdio_printf
 * kind:  negative
 * funcs: fopen fclose
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fopen; Win32 file naming conventions (reserved characters * ? < > |)
 * note:  Probes that names with wildcard / reserved characters make fopen return NULL (errno ENOENT or EINVAL for the
 *        wildcard case) and that no file is created.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile n_star = "tc_pfx_neg_wc_*.tmp";
static const char *volatile n_lt   = "tc_pfx_neg_wc_a<b";
static const char *volatile n_pipe = "tc_pfx_neg_wc_a|b";

TC_CASE(star)
{
    FILE *f; errno = 0; f = fopen(n_star, "w");
    if (f) fclose(f);
    CHECK(f == NULL);
    CHECK(errno == ENOENT || errno == EINVAL);
}
TC_CASE(lt_pipe)
{
    FILE *f = fopen(n_lt, "w"); CHECK(f == NULL); if (f) fclose(f);
    f = fopen(n_pipe, "w"); CHECK(f == NULL); if (f) fclose(f);
}
TC_CASE(nothing_created)
{
    WIN32_FIND_DATAA fd; HANDLE h = FindFirstFileA("tc_pfx_neg_wc_*", &fd);
    CHECK(h == INVALID_HANDLE_VALUE);
    if (h != INVALID_HANDLE_VALUE) {
        do { DeleteFileA(fd.cFileName); } while (FindNextFileA(h, &fd));
        FindClose(h);
    }
}
int main(void) { tc_begin(); TC_RUN(star); TC_RUN(lt_pipe); TC_RUN(nothing_created); return tc_end(); }
