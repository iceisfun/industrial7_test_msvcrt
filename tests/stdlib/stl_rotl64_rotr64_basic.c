/* TC
 * id:    stl_rotl64_rotr64_basic
 * area:  stdlib
 * funcs: 
 * procs: _rotl64 _rotr64
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _rotl64, _rotr64
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

TC_CASE(basic) { x = 0x8000000000000001ULL; c = 1; CHECK_EQ_U(_rotl64(x, c), 3); x = 3; CHECK_EQ_U(_rotr64(x, c), 0x8000000000000001ULL); }
TC_CASE(zero) { x = 0x0123456789abcdefULL; c = 0; CHECK_EQ_U(_rotl64(x, c), 0x0123456789abcdefULL); CHECK_EQ_U(_rotr64(x, c), 0x0123456789abcdefULL); }
TC_CASE(sixteen) { x = 0x0123456789abcdefULL; c = 16; CHECK_EQ_U(_rotl64(x, c), 0x456789abcdef0123ULL); CHECK_EQ_U(_rotr64(x, c), 0xcdef0123456789abULL); }
TC_CASE(inverse)
{
    int i, bad = 0; x = 0xa5c3f00f12345678ULL;
    for (i = 0; i < 64; i++) { c = i; if (_rotr64(_rotl64(x, c), c) != 0xa5c3f00f12345678ULL) bad++; if (_rotl64(_rotr64(x, c), c) != 0xa5c3f00f12345678ULL) bad++; }
    CHECK_EQ_I(bad, 0);
}

int main(void) { tc_begin(); TC_LOAD_PROC(_rotl64); TC_LOAD_PROC(_rotr64); if (!(tc_p__rotl64 && tc_p__rotr64)) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; } TC_RUN(basic); TC_RUN(zero); TC_RUN(sixteen); TC_RUN(inverse); return tc_end(); }
