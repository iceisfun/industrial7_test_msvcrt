/* TC
 * id:    stl_rotl64_wrapcount_probe
 * area:  stdlib
 * funcs: 
 * procs: _rotl64 _rotr64
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _rotl64 (counts outside 0..63 not specified)
 * note:  PROBE: _rotl64/_rotr64 with count 64 and 65 behave as count & 63 (rol/ror semantics); docs do not promise it.
 */
#include "tc.h"
#include <stdlib.h>

#undef _rotl64
TC_DECL_PROC(unsigned __int64, _rotl64, (unsigned __int64, int));
#define _rotl64 tc_p__rotl64
#undef _rotr64
TC_DECL_PROC(unsigned __int64, _rotr64, (unsigned __int64, int));
#define _rotr64 tc_p__rotr64

static volatile unsigned __int64 x; static volatile int c;

TC_CASE(sixty_four) { x = 0x0123456789abcdefULL; c = 64; CHECK_EQ_U(_rotl64(x, c), 0x0123456789abcdefULL); CHECK_EQ_U(_rotr64(x, c), 0x0123456789abcdefULL); }
TC_CASE(sixty_five) { x = 0x0123456789abcdefULL; c = 65; CHECK_EQ_U(_rotl64(x, c), 0x02468acf13579bdeULL); }

int main(void) { tc_begin(); TC_LOAD_PROC(_rotl64); TC_LOAD_PROC(_rotr64); if (!(tc_p__rotl64 && tc_p__rotr64)) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; } TC_RUN(sixty_four); TC_RUN(sixty_five); return tc_end(); }
