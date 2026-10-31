/* TC
 * id:    mem_memchr_int_arg
 * area:  memory
 * funcs: memchr
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.5.1 (c converted to unsigned char); MS docs memchr
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>

static volatile size_t v_n = 6;
static volatile int v_c;
static const unsigned char D[] = {'x', 0xff, 'A', 0x00, 'y', 0x80};

TC_CASE(c_0x141_finds_A) { v_c = 0x141; CHECK_EQ_P(memchr(D, v_c, v_n), D + 2); }
TC_CASE(c_minus1_finds_0xff) { v_c = -1; CHECK_EQ_P(memchr(D, v_c, v_n), D + 1); v_c = -128; CHECK_EQ_P(memchr(D, v_c, v_n), D + 5); }
TC_CASE(c_0_finds_embedded_nul) { v_c = 0; CHECK_EQ_P(memchr(D, v_c, v_n), D + 3); }
TC_CASE(c_0x100_finds_nul) { v_c = 0x100; CHECK_EQ_P(memchr(D, v_c, v_n), D + 3); }

int main(void) { tc_begin(); TC_RUN(c_0x141_finds_A); TC_RUN(c_minus1_finds_0xff); TC_RUN(c_0_finds_embedded_nul); TC_RUN(c_0x100_finds_nul); return tc_end(); }
