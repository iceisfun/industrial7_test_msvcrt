/* TC
 * id:    fio_rename_basic
 * area:  stdio_file
 * funcs: rename fopen fclose
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.4.2; MS docs rename
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
static const char * volatile na = "tc_fio_rename_basic_a.tmp";
static const char * volatile nb = "tc_fio_rename_basic_b.tmp";
static const char * volatile dn = "tc_fio_rename_basic_d.tmp";
static const char * volatile nc = "tc_fio_rename_basic_d.tmp\\c.tmp";

static void cleanup(void) { DeleteFileA(na); DeleteFileA(nb); DeleteFileA(nc); RemoveDirectoryA(dn); }
static void mk(const char *n)
{
    FILE *f = fopen(n, "w");
    CHECK(f != NULL);
    if (f) { fputs("hello", f); fclose(f); }
}
TC_CASE(rename_simple)
{
    FILE *f; char b[16] = {0};
    mk(na);
    CHECK_EQ_I(rename(na, nb), 0);
    errno = 0;
    f = fopen(na, "r");
    CHECK(f == NULL);
    CHECK_EQ_I(errno, ENOENT);
    if (f) fclose(f);
    f = fopen(nb, "r");
    CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fread(b, 1, 15, f), 5); CHECK_STR(b, "hello"); fclose(f); }
}
TC_CASE(rename_into_subdir)
{
    CHECK(CreateDirectoryA(dn, NULL));
    CHECK_EQ_I(rename(nb, nc), 0);
    CHECK(GetFileAttributesA(nc) != INVALID_FILE_ATTRIBUTES);
    CHECK(GetFileAttributesA(nb) == INVALID_FILE_ATTRIBUTES);
}
int main(void) { tc_begin(); cleanup(); TC_RUN(rename_simple); TC_RUN(rename_into_subdir); cleanup(); return tc_end(); }
