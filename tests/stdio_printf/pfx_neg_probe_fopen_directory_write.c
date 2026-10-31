/* TC
 * id:    pfx_neg_probe_fopen_directory_write
 * area:  stdio_printf
 * kind:  negative
 * funcs: fopen fclose
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fopen; Win32 CreateFile on a directory without FILE_FLAG_BACKUP_SEMANTICS -> ERROR_ACCESS_DENIED -> EACCES
 * note:  Probes that fopen(".", "w"/"r") returns NULL with errno EACCES.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile dir = ".";

TC_CASE(write_mode)
{
    FILE *f; errno = 0; f = fopen(dir, "w");
    if (f) fclose(f);
    CHECK(f == NULL);
    CHECK_EQ_I(errno, EACCES);
}
TC_CASE(read_mode)
{
    FILE *f; errno = 0; f = fopen(dir, "r");
    if (f) fclose(f);
    CHECK(f == NULL);
    CHECK_EQ_I(errno, EACCES);
}
int main(void) { tc_begin(); TC_RUN(write_mode); TC_RUN(read_mode); return tc_end(); }
