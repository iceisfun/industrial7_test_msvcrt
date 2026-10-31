/* TC
 * id:    fio_tmpnam_user_buffer
 * area:  stdio_file
 * funcs: tmpnam
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs tmpnam; C11 7.21.4.4
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

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
