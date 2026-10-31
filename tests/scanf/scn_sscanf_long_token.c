/* TC
 * id:    scn_sscanf_long_token
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.2
 * note:  Checks there is no small internal buffer limit.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

static char in[20100];
static char out[20001];
TC_CASE(long_string)
{
    int n, i, pos = -1;
    for (i = 0; i < 20000; i++) in[i] = (char)('a' + i % 26);
    in[20000] = 0;
    n = sscanf(in, "%s%n", out, &pos);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(pos, 20000); CHECK(strlen(out) == 20000); CHECK_MEM(out, in, 20001);
}
TC_CASE(long_zeros)
{
    int n, i, a = 0;
    for (i = 0; i < 5000; i++) in[i] = '0';
    in[5000] = '7'; in[5001] = 0;
    n = sscanf(in, "%d", &a);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 7);
}
TC_CASE(long_space)
{
    int n, i, a = 0;
    for (i = 0; i < 10000; i++) in[i] = ' ';
    in[10000] = '4'; in[10001] = '2'; in[10002] = 0;
    n = sscanf(in, "%d", &a);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 42);
}
int main(void) { tc_begin(); TC_RUN(long_string); TC_RUN(long_zeros); TC_RUN(long_space); return tc_end(); }
