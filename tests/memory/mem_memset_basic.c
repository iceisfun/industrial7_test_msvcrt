/* TC
 * id:    mem_memset_basic
 * area:  memory
 * funcs: memset
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.6.1; MS docs memset
 */
#include "tc.h"
#include <string.h>

static unsigned char buf[32];
static volatile size_t v_n;
static volatile int v_c;

TC_CASE(fill_0) { memset(buf, 0x77, 32); v_n = 32; v_c = 0; memset(buf, v_c, v_n); { size_t i, bad = 0; for (i = 0; i < 32; i++) bad += buf[i] != 0; CHECK_EQ_U(bad, 0); } }
TC_CASE(fill_0xAA) { size_t i, bad = 0; v_n = 32; v_c = 0xAA; memset(buf, 0, 32); memset(buf, v_c, v_n); for (i = 0; i < 32; i++) bad += buf[i] != 0xAA; CHECK_EQ_U(bad, 0); }
TC_CASE(returns_dest) { void *volatile d = buf + 3; v_c = 1; v_n = 4; CHECK_EQ_P(memset(d, v_c, v_n), buf + 3); }
TC_CASE(only_count_bytes_written)
{
    size_t i, bad = 0; memset(buf, 0x11, 32); v_n = 10; v_c = 0xEE;
    memset(buf + 8, v_c, v_n);
    for (i = 0; i < 32; i++) { unsigned char w = (i >= 8 && i < 18) ? 0xEE : 0x11; bad += buf[i] != w; }
    CHECK_EQ_U(bad, 0);
}
int main(void) { tc_begin(); TC_RUN(fill_0); TC_RUN(fill_0xAA); TC_RUN(returns_dest); TC_RUN(only_count_bytes_written); return tc_end(); }
