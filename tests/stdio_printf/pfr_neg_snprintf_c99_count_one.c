/* TC
 * id:    pfr_neg_snprintf_c99_count_one
 * area:  stdio_printf
 * kind:  negative
 * funcs: snprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.5 snprintf; MS docs snprintf (UCRT is C99 conformant)
 * note:  count==1 writes only the NUL; count==len keeps len-1 chars; return is always the would-be length.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define G 0x5A
static const char * volatile s = "abcd";

TC_CASE(count_one)
{
    char b[8]; int r; memset(b, G, sizeof b); tc_iph_count = 0; errno = 0;
    r = snprintf(b, 1, "%s", s);
    CHECK_EQ_I(r, 4); CHECK_EQ_I(b[0], 0); CHECK_EQ_I((unsigned char)b[1], G);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
}
TC_CASE(count_len)
{
    char b[8]; int r; memset(b, G, sizeof b); tc_iph_count = 0; errno = 0;
    r = snprintf(b, 4, "%s", s);
    CHECK_EQ_I(r, 4); CHECK_STR(b, "abc"); CHECK_EQ_I((unsigned char)b[4], G);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
}
TC_CASE(count_len_plus_1)
{
    char b[8]; int r; memset(b, G, sizeof b); tc_iph_count = 0; errno = 0;
    r = snprintf(b, 5, "%s", s);
    CHECK_EQ_I(r, 4); CHECK_STR(b, "abcd"); CHECK_EQ_I((unsigned char)b[5], G);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(count_one); TC_RUN(count_len); TC_RUN(count_len_plus_1);
    return tc_end();
}
