/* TC
 * id:    sec_neg_itoa_s_small_buf_reset_probe
 * area:  secure_crt
 * funcs: _itoa_s strtok_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _itoa_s (silent on buffer contents after ERANGE)
 * note:  PROBE: (1) after ERANGE the secure conversion writes an empty string (buf[0]==0) when size>0;
 *        (2) strtok_s stores in *context the position just after the NUL it wrote over the delimiter (a+2 for "x y z").
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
static volatile int v = 123456;
static char a[16];
TC_CASE(buffer_emptied_after_erange)
{
    char b[4] = "zzz"; int r;
    tc_iph_count = 0;
    r = _itoa_s(v, b, 4, 10);
    CHECK_EQ_I(r, ERANGE);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(strtok_context_position)
{
    char *c = NULL, *t; a[0]='x'; a[1]=' '; a[2]='y'; a[3]=' '; a[4]='z'; a[5]=0;
    t = strtok_s(a, " ", &c); CHECK_STR(t, "x"); CHECK(c == a + 2);
    t = strtok_s(NULL, " ", &c); CHECK_STR(t, "y"); CHECK(c == a + 4);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(buffer_emptied_after_erange); TC_RUN(strtok_context_position); return tc_end(); }
