/* TC
 * id:    fio_probe_tmpnam_format
 * area:  stdio_file
 * funcs: tmpnam
 * crt:   all
 * tier:  p2
 * conf:  observed
 * ref:   MS docs tmpnam (names are relative to the root: "\s1jo.0" style)
 * note:  PROBE (conf could be documented: MS tmpnam says names are unique in the GetTempPathW dir; newer Win11 ucrtbase uses GetTempPath2W, only 19045 observed): the generated name starts with a backslash and has no further path separator.
 *        Exact shape beyond that is deliberately not asserted.
 *        TRIAGE win10, class A + F): ucrtbase 10.0.19045 tmpnam(buf) writes GetTempPath()+"sNNN.N" (e.g. C:\Users\ADMINI~1\AppData\Local\Temp\s690.0, 43 chars), not the old
 *        "\s1jo.0" root-relative form; the test crashed because its L_tmpnam+16 (=30, mingw L_tmpnam is 14) buffer overflowed. The prefix is compared with GetTempPathA() so it is
 *        independent of the user/TMP value. Old backslash expectation kept for non-UCRT targets (unverified).
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

/* ucrtbase writes an absolute path (GetTempPath() + "sXXX.Y", about 43 chars) into the caller's buffer, far beyond the toolchain's
 * L_tmpnam (14): a buffer of L_tmpnam + 16 overflows (observed: 0xC0000005). Use a MAX_PATH-sized buffer. */
#define TB 300
TC_CASE(leading_backslash)
{
    char b[TB];
#if TC_AT_LEAST(140)
    /* kept case name; now checks the temp-directory prefix (observed ucrtbase 10.0.19045: "C:\...\Temp\s690.0") */
    char t[TB]; DWORD n = GetTempPathA(TB, t); tmpnam(b);
    CHECK(n > 0 && n < TB);
    CHECK(_strnicmp(b, t, n) == 0);
    CHECK(strlen(b) > n);
#else
    tmpnam(b); CHECK(b[0] == '\\');
#endif
}
TC_CASE(no_more_separators)
{
    char b[TB];
#if TC_AT_LEAST(140)
    char t[TB]; DWORD n = GetTempPathA(TB, t); CHECK(tmpnam(b) != NULL);
    CHECK(n > 0 && n < TB);
    CHECK(strlen(b) > n);
    CHECK(strchr(b + n, '\\') == NULL);
    CHECK(strchr(b + n, '/') == NULL);
#else
    tmpnam(b);
    CHECK(strchr(b + 1, '\\') == NULL);
    CHECK(strchr(b, '/') == NULL);
#endif
}
int main(void) { tc_begin(); TC_RUN(leading_backslash); TC_RUN(no_more_separators); return tc_end(); }
