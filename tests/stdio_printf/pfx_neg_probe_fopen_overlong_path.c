/* TC
 * id:    pfx_neg_probe_fopen_overlong_path
 * area:  stdio_printf
 * kind:  negative
 * funcs: fopen fclose
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fopen (returns NULL on failure); Win32 ERROR_FILENAME_EXCED_RANGE / ERROR_INVALID_NAME
 * note:  Probes errno for a 5000 character file name: expected ENOENT or EINVAL (OS error mapped by _dosmaperr), and
 *        that no invalid-parameter handler is involved. Assumes the machine does not accept such a name.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static char name[5100];

TC_CASE(fails)
{
    FILE *f; int i;
    for (i = 0; i < 5000; i++) name[i] = 'a';
    name[0]='t'; name[1]='c'; name[2]='_'; name[3]='p'; name[4]='f'; name[5]='x'; name[6]='_'; name[7]='o'; name[8]='l'; name[9]='_';
    name[5000] = 0;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    f = fopen(name, "w");
    if (f) { fclose(f); DeleteFileA(name); }
    CHECK(f == NULL);
    CHECK(errno == ENOENT || errno == EINVAL);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(nothing_created)
{
    WIN32_FIND_DATAA fd; HANDLE h = FindFirstFileA("tc_pfx_ol_*", &fd);
    CHECK(h == INVALID_HANDLE_VALUE);
    if (h != INVALID_HANDLE_VALUE) FindClose(h);
}
int main(void) { tc_begin(); TC_RUN(fails); TC_RUN(nothing_created); return tc_end(); }
