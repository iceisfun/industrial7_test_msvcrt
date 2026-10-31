/* TC
 * id:    str_strlen_boundaries
 * area:  string
 * funcs: strlen
 * crt:   all
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.24.6.3
 * note:  Page-end behaviour (no read past the NUL page) is an implementation property, probed separately in str_strlen_pageend_probe.
 */
#include "tc.h"
#include <string.h>

static volatile char *vp;

TC_CASE(big_string)
{
    static char b[65537]; size_t i;
    for (i = 0; i < 65536; i++) b[i] = 'a';
    b[65536] = 0; vp = b;
    CHECK_EQ_U(strlen((const char *)vp), 65536);
}
TC_CASE(unaligned)
{
    static char b[128]; size_t off;
    memset(b, 'q', sizeof b); b[100] = 0;
    for (off = 0; off < 16; off++) { vp = b + off; CHECK_EQ_U(strlen((const char *)vp), 100 - off); }
}
TC_CASE(all_byte_values)
{
    static char b[256 * 3 + 1]; size_t i;
    for (i = 0; i < 765; i++) b[i] = (char)(1 + i % 255);
    b[765] = 0; vp = b;
    CHECK_EQ_U(strlen((const char *)vp), 765);
}
int main(void)
{
    tc_begin();
    TC_RUN(big_string); TC_RUN(unaligned); TC_RUN(all_byte_values);
    return tc_end();
}
