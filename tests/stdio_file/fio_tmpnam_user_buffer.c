/* TC
 * id:    fio_tmpnam_user_buffer
 * area:  stdio_file
 * funcs: tmpnam
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs tmpnam; C11 7.21.4.4
 *        OBSERVED (Win10 22H2, class A): tmpnam(buf) returns buf holding a full temp path (e.g. C:\Users\ADMINI~1\AppData\Local\Temp\s62s.0); with the mingw L_tmpnam (14) buffers
 *        overflowed and the process died 0xc0000005. Path length depends on %TEMP%.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

/* UCRT tmpnam returns a full temp-dir path (observed 41 chars: C:\Users\..\Temp\s62s.0); ucrt <stdio.h> L_tmpnam is 260,
 * mingw's is 14, which overflowed the buffer (AV). Use the UCRT size. */
#if TC_CRT_VER >= 140
#undef L_tmpnam
#define L_tmpnam 260
#endif

TC_CASE(user_buffer)
{
    char a[L_tmpnam + 16], b[L_tmpnam + 16]; char *ra, *rb;
    memset(a, 0, sizeof a); memset(b, 0, sizeof b);
    ra = tmpnam(a);
    rb = tmpnam(b);
    CHECK_EQ_P(ra, a);
    CHECK_EQ_P(rb, b);
    CHECK(strlen(a) > 0 && strlen(a) < L_tmpnam);
    CHECK(strlen(b) > 0 && strlen(b) < L_tmpnam);
    CHECK(!tc_strequ(a, b));
}
TC_CASE(not_existing)
{
    char a[L_tmpnam + 16];
    tmpnam(a);
    CHECK_EQ_U(GetFileAttributesA(a), INVALID_FILE_ATTRIBUTES);
}
int main(void) { tc_begin(); TC_RUN(user_buffer); TC_RUN(not_existing); return tc_end(); }
