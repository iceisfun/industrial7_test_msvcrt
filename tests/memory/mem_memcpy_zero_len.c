/* TC
 * id:    mem_memcpy_zero_len
 * area:  memory
 * funcs: memcpy
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.2.1, 7.24.1p2 (valid pointers even for n=0)
 */
#include "tc.h"
#include <string.h>

static unsigned char buf[32], src[32];
static volatile size_t v_zero = 0;

static void fill(void) { size_t i; for (i = 0; i < 32; i++) { buf[i] = (unsigned char)(0xa0 + i); src[i] = (unsigned char)i; } }
TC_CASE(dest_unchanged) { unsigned char ref[32]; size_t i; fill(); for (i = 0; i < 32; i++) ref[i] = buf[i]; memcpy(buf, src, v_zero); CHECK_MEM(buf, ref, 32); }
TC_CASE(returns_dest) { void *volatile d = buf + 5; void *r; fill(); r = memcpy(d, src, v_zero); CHECK_EQ_P(r, buf + 5); }
TC_CASE(zero_len_one_past_end_ptr)
{
    unsigned char ref[32]; size_t i; unsigned char *volatile e = buf + 32; unsigned char *volatile s = src + 32; void *r;
    fill(); for (i = 0; i < 32; i++) ref[i] = buf[i];
    r = memcpy((void *)e, (void *)s, v_zero);
    CHECK_EQ_P(r, buf + 32);
    CHECK_MEM(buf, ref, 32);
}
int main(void) { tc_begin(); TC_RUN(dest_unchanged); TC_RUN(returns_dest); TC_RUN(zero_len_one_past_end_ptr); return tc_end(); }
