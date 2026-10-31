/* TC
 * id:    fio_neg_rename_dest_exists
 * area:  stdio_file
 * funcs: rename fopen fclose
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs rename: destination exists => EACCES
 * note:  Docs say EACCES; the Win32 mapping of ERROR_ALREADY_EXISTS may give EEXIST, either accepted. Windows rename never replaces.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
static const char * volatile na = "tc_fio_neg_rename_dest_exists_a.tmp";
static const char * volatile nb = "tc_fio_neg_rename_dest_exists_b.tmp";

static void mk(const char *n, const char *s)
{
    FILE *f = fopen(n, "w");
    CHECK(f != NULL);
    if (f) { fputs(s, f); fclose(f); }
}
static void rd(const char *n, char *b)
{
    FILE *f = fopen(n, "r"); size_t k;
    b[0] = 0;
    CHECK(f != NULL);
    if (f) { k = fread(b, 1, 15, f); b[k] = 0; fclose(f); }
}
TC_CASE(dest_exists)
{
    char b[16];
    int r;
    mk(na, "AAA"); mk(nb, "BBB");
    errno = 0;
    r = rename(na, nb);
    CHECK(r != 0);
    CHECK(errno == EACCES || errno == EEXIST);
    rd(na, b); CHECK_STR(b, "AAA");
    rd(nb, b); CHECK_STR(b, "BBB");
}
int main(void) { tc_begin(); DeleteFileA(na); DeleteFileA(nb); TC_RUN(dest_exists); DeleteFileA(na); DeleteFileA(nb); return tc_end(); }
