/* TC
 * id:    fio_neg_probe_fopen_empty_name
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen
 * procs: 
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   Win32 CreateFile with empty path
 * note:  Probes: fopen of an empty filename returns NULL; errno is expected ENOENT (or EINVAL if the CRT validates it). Only NULL and errno in {ENOENT,EINVAL} are asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char * volatile empty = "";

TC_CASE(empty_read)
{
    FILE *f; errno = 0;
    f = fopen(empty, "r");
    CHECK(f == NULL);
    CHECK(errno == ENOENT || errno == EINVAL);
    if (f) fclose(f);
}
TC_CASE(empty_write)
{
    FILE *f; errno = 0;
    f = fopen(empty, "w");
    CHECK(f == NULL);
    CHECK(errno == ENOENT || errno == EINVAL);
    if (f) fclose(f);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(empty_read); TC_RUN(empty_write);
    return tc_end();
}
