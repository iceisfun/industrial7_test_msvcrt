/* TC
 * id:    gio_tempnam_unique
 * area:  lowio
 * funcs: _tempnam _wtempnam _open _close _unlink _access _waccess free
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _tempnam, _wtempnam: returns malloc'd name of a file that does not exist; free with free()
 * note:  The TMP environment variable overrides the dir argument and the CRT keeps its own environment copy,
 *        so no assertion is made about the directory part. Complements fio_tempnam_dir_prefix.
 *        Win10 22H2 19045 (diagnosed): the baseline CRASH (0xC0000005 in the first case) was a test bug: <stdio.h> was not included, so _tempnam was implicitly
 *        declared returning int and the 64-bit pointer was truncated. Including <stdio.h> fixes it (class A, no expectation changed).
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>      /* _tempnam is declared here; without it the implicit int return truncates the pointer and crashes */
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <sys/stat.h>

TC_CASE(contains_prefix_not_existing)
{
    char *n = _tempnam(".", "tcg");
    CHECK(n != NULL); if (!n) return;
    CHECK(strstr(n, "tcg") != NULL);
    CHECK_EQ_I(_access(n, 0), -1);
    free(n);
}
TC_CASE(distinct_after_create)
{
    char *a = _tempnam(".", "tcg"), *b; int fd;
    CHECK(a != NULL); if (!a) return;
    fd = _open(a, _O_CREAT | _O_EXCL | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd < 0) { free(a); TC_SKIP("cannot create file in temp dir"); }
    b = _tempnam(".", "tcg");
    CHECK(b != NULL);
    if (b) { CHECK(strcmp(a, b) != 0); free(b); }
    _close(fd); _unlink(a); free(a);
}
TC_CASE(empty_prefix_ok)
{
    char *n = _tempnam(".", "");
    CHECK(n != NULL);
    free(n);
}
TC_CASE(wide_variant)
{
    wchar_t *n = _wtempnam(L".", L"tcg");
    CHECK(n != NULL); if (!n) return;
    CHECK(wcsstr(n, L"tcg") != NULL);
    CHECK_EQ_I(_waccess(n, 0), -1);
    free(n);
}
int main(void)
{
    tc_begin();
    TC_RUN(contains_prefix_not_existing); TC_RUN(distinct_after_create); TC_RUN(empty_prefix_ok); TC_RUN(wide_variant);
    return tc_end();
}
