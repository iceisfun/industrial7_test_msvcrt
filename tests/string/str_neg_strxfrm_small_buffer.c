/* TC
 * id:    str_neg_strxfrm_small_buffer
 * area:  string
 * funcs: strxfrm
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.24.4.5p3-4 (at most n bytes written; returns length not including NUL; if >= n contents indeterminate)
 * note:  Destination contents are deliberately not asserted when the return value is >= count; only the guard bytes beyond count.
 */
#include "tc.h"
#include <string.h>

static volatile const char *vs;

TC_CASE(count_less_than_len)
{
    char d[16]; vs = "abcdefgh";
    memset(d, 0x55, sizeof d);
    CHECK(strxfrm(d, (const char *)vs, 4) >= 4);
    CHECK_EQ_U(strxfrm(d, (const char *)vs, 4), 8);
}
TC_CASE(count_equals_len)
{
    char d[16]; vs = "abcd";
    memset(d, 0x55, sizeof d);
    CHECK_EQ_U(strxfrm(d, (const char *)vs, 4), 4);
}
TC_CASE(count_zero_null_dest)
{
    vs = "abcd";
    CHECK_EQ_U(strxfrm(NULL, (const char *)vs, 0), 4);
}
TC_CASE(count_zero_dest_untouched)
{
    char d[4]; vs = "abcd";
    memset(d, 0x55, sizeof d);
    CHECK_EQ_U(strxfrm(d, (const char *)vs, 0), 4);
    CHECK_EQ_I((unsigned char)d[0], 0x55);
}
TC_CASE(no_write_beyond_count)
{
    char d[16]; vs = "abcdefghij";
    memset(d, 0x55, sizeof d);
    CHECK(strxfrm(d, (const char *)vs, 5) >= 5);
    CHECK_EQ_I((unsigned char)d[5], 0x55);
    CHECK_EQ_I((unsigned char)d[6], 0x55);
    CHECK_EQ_I((unsigned char)d[15], 0x55);
}
int main(void)
{
    tc_begin();
    TC_RUN(count_less_than_len); TC_RUN(count_equals_len); TC_RUN(count_zero_null_dest); TC_RUN(count_zero_dest_untouched); TC_RUN(no_write_beyond_count);
    return tc_end();
}
