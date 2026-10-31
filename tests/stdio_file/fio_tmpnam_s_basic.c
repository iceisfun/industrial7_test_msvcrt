/* TC
 * id:    fio_tmpnam_s_basic
 * area:  stdio_file
 * funcs:
 * procs: tmpnam_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs tmpnam_s, _wtmpnam_s (L_tmpnam_s)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

#ifndef L_tmpnam_s
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
