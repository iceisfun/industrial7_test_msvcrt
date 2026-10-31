/* TC
 * id:    mem_neg_memccpy_not_found_copies_all
 * area:  memory
 * funcs: _memccpy
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _memccpy (copies count bytes or until c; returns NULL if c not found in the first count bytes)
 * note:  the 0x1FF int-folding question is deliberately NOT asserted here (see mem_memccpy_int_arg_probe).
 */
#include "tc.h"
#include <string.h>
#include <memory.h>

static volatile size_t v_n;
static volatile int v_c;

TC_CASE(no_match_copies_count)
{
    unsigned char d[8]; const unsigned char s[8] = "abcdefg"; int i;
    for (i = 0; i < 8; i++) d[i] = 0xEE;
    v_n = 5; v_c = 'z';
    CHECK(_memccpy(d, s, v_c, v_n) == NULL);
    CHECK_MEM(d, s, 5);
    CHECK_EQ_I(d[5], 0xEE); CHECK_EQ_I(d[6], 0xEE);
}
TC_CASE(match_only_past_count)
{
    unsigned char d[8]; const unsigned char s[8] = "abcdefg"; int i;
    for (i = 0; i < 8; i++) d[i] = 0xEE;
    v_n = 5; v_c = 'f';  /* s[5] */
    CHECK(_memccpy(d, s, v_c, v_n) == NULL);
    CHECK_MEM(d, s, 5);
    CHECK_EQ_I(d[5], 0xEE);
}
TC_CASE(match_last_byte_is_found)
{   /* contrast: match at s[count-1] returns pointer just past it */
    unsigned char d[8]; const unsigned char s[8] = "abcdefg"; int i; void *r;
    for (i = 0; i < 8; i++) d[i] = 0xEE;
    v_n = 5; v_c = 'e';
    r = _memccpy(d, s, v_c, v_n);
    CHECK(r == d + 5);
    CHECK_EQ_I(d[4], 'e'); CHECK_EQ_I(d[5], 0xEE);
}
int main(void)
{
    tc_begin();
    TC_RUN(no_match_copies_count); TC_RUN(match_only_past_count); TC_RUN(match_last_byte_is_found);
    return tc_end();
}
