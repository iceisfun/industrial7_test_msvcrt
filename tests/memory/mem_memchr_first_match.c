/* TC
 * id:    mem_memchr_first_match
 * area:  memory
 * funcs: memchr
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.5.1; MS docs memchr
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>

static unsigned char buf[16 + 140];
static volatile size_t v_n;
static volatile int v_c;

TC_CASE(repeated_char)
{
    static const char s[] = "abcXdefXXghX";
    v_c = 'X'; v_n = 12;
    CHECK_EQ_P(memchr(s, v_c, v_n), s + 3);
}
TC_CASE(match_at_each_index_0_to_130)
{
    size_t i, j; int bad = 0;
    for (i = 0; i <= 130; i++) {
        memset(buf, 'a', sizeof buf);
        for (j = i; j < 131 + 3; j += 1) buf[j] = 'Q';   /* every byte from i onward matches */
        v_c = 'Q'; v_n = 134;
        if (memchr(buf, v_c, v_n) != buf + i) bad++;
    }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(misaligned_start_0_to_15)
{
    size_t a, i; int bad = 0;
    for (a = 0; a < 16; a++) for (i = 0; i < 100; i += 3) {
        memset(buf, 'a', sizeof buf);
        buf[a + i] = 'Q'; buf[a + i + 5] = 'Q'; buf[a + i + 6] = 'Q';
        v_c = 'Q'; v_n = 120;
        if (memchr(buf + a, v_c, v_n) != buf + a + i) bad++;
    }
    CHECK_EQ_I(bad, 0);
}

int main(void) { tc_begin(); TC_RUN(repeated_char); TC_RUN(match_at_each_index_0_to_130); TC_RUN(misaligned_start_0_to_15); return tc_end(); }
