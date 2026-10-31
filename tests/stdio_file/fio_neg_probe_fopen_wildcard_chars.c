/* TC
 * id:    fio_neg_probe_fopen_wildcard_chars
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen fclose remove
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fopen, remove; Win32 file naming conventions (reserved characters < > : " / \ | ? *)
 * note:  Probes that names containing the reserved characters * ? < > | " are rejected by fopen (NULL, no file created),
 *        that errno is one of {ENOENT, EINVAL} (ERROR_INVALID_NAME is expected to map to ENOENT), and that remove() does
 *        not expand wildcards (returns -1, sibling file survives).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define PFX "tc_fio_wc" TC_S1(TC_CRT_VER) "_"

static void put(const char *n)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
}
static int count_matching(void)   /* number of files named PFX* in cwd */
{
    WIN32_FIND_DATAA fd; int n = 0; HANDLE h = FindFirstFileA(PFX "*", &fd);
    if (h == INVALID_HANDLE_VALUE) return 0;
    do { n++; } while (FindNextFileA(h, &fd));
    FindClose(h); return n;
}
static void cleanup(void)
{
    WIN32_FIND_DATAA fd; HANDLE h = FindFirstFileA(PFX "*", &fd);
    if (h == INVALID_HANDLE_VALUE) return;
    do { DeleteFileA(fd.cFileName); } while (FindNextFileA(h, &fd));
    FindClose(h);
}
static const char *volatile names[] = {
    PFX "a*b.tmp", PFX "a?b.tmp", PFX "a<b.tmp", PFX "a>b.tmp", PFX "a|b.tmp", PFX "a\"b.tmp"
};

TC_CASE(fopen_rejects)
{
    int i;
    cleanup();
    for (i = 0; i < 6; i++) {
        FILE *f; errno = 0;
        f = fopen(names[i], "w");
        CHECK(f == NULL);
        if (f) fclose(f);
        else CHECK(errno == ENOENT || errno == EINVAL);
    }
    CHECK_EQ_I(count_matching(), 0);
}
TC_CASE(fopen_read_rejects)
{
    int i;
    for (i = 0; i < 6; i++) {
        FILE *f = fopen(names[i], "r");
        CHECK(f == NULL);
        if (f) fclose(f);
    }
}
TC_CASE(remove_no_wildcard)
{
    int r;
    put(PFX "axb.tmp");
    CHECK(GetFileAttributesA(PFX "axb.tmp") != INVALID_FILE_ATTRIBUTES);
    errno = 0; r = remove(names[0]);
    CHECK_EQ_I(r, -1);
    CHECK(errno == ENOENT || errno == EINVAL);
    CHECK(GetFileAttributesA(PFX "axb.tmp") != INVALID_FILE_ATTRIBUTES);
    errno = 0; r = remove(names[1]);
    CHECK_EQ_I(r, -1);
    CHECK(GetFileAttributesA(PFX "axb.tmp") != INVALID_FILE_ATTRIBUTES);
}
int main(void)
{
    tc_begin();
    TC_RUN(fopen_rejects); TC_RUN(fopen_read_rejects); TC_RUN(remove_no_wildcard);
    cleanup();
    return tc_end();
}
