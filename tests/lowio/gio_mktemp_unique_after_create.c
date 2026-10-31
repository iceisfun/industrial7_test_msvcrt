/* TC
 * id:    gio_mktemp_unique_after_create
 * area:  lowio
 * funcs: _mktemp _open _close _unlink _access
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _mktemp: checks for existing files, never returns the name of an existing file
 * note:  Created files are removed.
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

static int mk(char *buf, int *fdp)
{
    char *r; strcpy(buf, "tcgioXXXXXX");
    r = _mktemp(buf);
    if (!r) return 0;
    *fdp = _open(buf, _O_CREAT | _O_EXCL | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    return 1;
}
TC_CASE(create_then_next_differs)
{
    char a[32], b[32]; int fa = -1, fb = -1;
    CHECK(mk(a, &fa)); CHECK(fa >= 0);
    CHECK(mk(b, &fb));
    CHECK(strcmp(a, b) != 0);
    if (fa >= 0) _close(fa);
    if (fb >= 0) _close(fb);
    _unlink(a); _unlink(b);
}
TC_CASE(five_distinct_names)
{
    char n[5][32]; int fd[5], i, j;
    for (i = 0; i < 5; i++) { fd[i] = -1; CHECK(mk(n[i], &fd[i])); CHECK(fd[i] >= 0); }
    for (i = 0; i < 5; i++) for (j = i + 1; j < 5; j++) CHECK(strcmp(n[i], n[j]) != 0);
    for (i = 0; i < 5; i++) { if (fd[i] >= 0) _close(fd[i]); _unlink(n[i]); }
}
TC_CASE(name_is_nonexistent_each_time)
{
    char n[32]; int fd = -1, i;
    for (i = 0; i < 3; i++) {
        strcpy(n, "tcgioXXXXXX");
        CHECK(_mktemp(n) != NULL);
        CHECK_EQ_I(_access(n, 0), -1);
        fd = _open(n, _O_CREAT | _O_EXCL | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
        CHECK(fd >= 0);
        if (fd >= 0) _close(fd);
        _unlink(n);
    }
}
int main(void)
{
    tc_begin();
    TC_RUN(create_then_next_differs); TC_RUN(five_distinct_names); TC_RUN(name_is_nonexistent_each_time);
    return tc_end();
}
