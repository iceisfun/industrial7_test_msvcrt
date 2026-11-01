/* TC
 * id:    fio_tmpnam_s_basic
 * area:  stdio_file
 * funcs:
 * procs: tmpnam_s
 * crt:   >=80
 * tier:  p1
 * conf:  observed
 * ref:   MS docs tmpnam_s, _wtmpnam_s (L_tmpnam_s)
 *        OBSERVED (Win10 22H2, class A): mingw header L_tmpnam_s is too small for UCRT, which returns full temp paths; tmpnam_s(buf,260) == 0, sizes 25/16/10 give ERANGE
 *        (raw: tc_p_tmpnam_s(a, L_tmpnam_s) == 0 got=34). Names depend on %TEMP% (length), not asserted beyond non-empty/distinct.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

/* UCRT tmpnam_s needs room for a full temp-dir path (observed names like C:\Users\..\Temp\u62s.0, 43 chars;
 * ucrt <stdio.h> has L_tmpnam_s == 260). mingw's own L_tmpnam_s (small) is the wrong constant for ucrtbase: ERANGE (34). */
#if TC_CRT_VER >= 140
#undef L_tmpnam_s
#define L_tmpnam_s 260
#endif
TC_DECL_PROC(int, tmpnam_s, (char *, size_t));

TC_CASE(success)
{
    char a[L_tmpnam_s], b[L_tmpnam_s];
    if (!tc_p_tmpnam_s) TC_SKIP("tmpnam_s not exported");
    memset(a, 0, sizeof a); memset(b, 0, sizeof b);
    CHECK_EQ_I(tc_p_tmpnam_s(a, L_tmpnam_s), 0);
    CHECK_EQ_I(tc_p_tmpnam_s(b, L_tmpnam_s), 0);
    CHECK(strlen(a) > 0);
    CHECK(strlen(b) > 0);
    CHECK(!tc_strequ(a, b));
}
int main(void) { tc_begin(); TC_LOAD_PROC(tmpnam_s); CHECK(tc_p_tmpnam_s != NULL); TC_RUN(success); return tc_end(); }
