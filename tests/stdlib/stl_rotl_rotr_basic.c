/* TC
 * id:    stl_rotl_rotr_basic
 * area:  stdlib
 * funcs: 
 * procs: _rotl _rotr _lrotl _lrotr
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _rotl, _rotr, _lrotl, _lrotr
 */
#include "tc.h"
#include <stdlib.h>

#undef _rotl
TC_DECL_PROC(unsigned int, _rotl, (unsigned int, int));
#define _rotl tc_p__rotl
#undef _rotr
TC_DECL_PROC(unsigned int, _rotr, (unsigned int, int));
#define _rotr tc_p__rotr
#undef _lrotl
TC_DECL_PROC(unsigned long, _lrotl, (unsigned long, int));
#define _lrotl tc_p__lrotl
#undef _lrotr
TC_DECL_PROC(unsigned long, _lrotr, (unsigned long, int));
#define _lrotr tc_p__lrotr

static volatile unsigned int x; static volatile int c; static volatile unsigned long lx;

TC_CASE(basic) { x = 0x80000001u; c = 1; CHECK_EQ_U(_rotl(x, c), 0x00000003u); x = 3; CHECK_EQ_U(_rotr(x, c), 0x80000001u); }
TC_CASE(zero_count) { x = 0xdeadbeefu; c = 0; CHECK_EQ_U(_rotl(x, c), 0xdeadbeefu); CHECK_EQ_U(_rotr(x, c), 0xdeadbeefu); }
TC_CASE(byte) { x = 0x12345678u; c = 8; CHECK_EQ_U(_rotl(x, c), 0x34567812u); CHECK_EQ_U(_rotr(x, c), 0x78123456u); }
TC_CASE(long_variants)
{
    lx = 0x80000001ul; c = 1; CHECK_EQ_U(_lrotl(lx, c), 0x3u);
    lx = 3; CHECK_EQ_U(_lrotr(lx, c), 0x80000001u);
    lx = 0x12345678ul; c = 8; CHECK_EQ_U(_lrotl(lx, c), 0x34567812u); CHECK_EQ_U(_lrotr(lx, c), 0x78123456u);
    c = 0; CHECK_EQ_U(_lrotl(lx, c), 0x12345678u);
}
TC_CASE(inverse)
{
    int i, bad = 0; x = 0xa5c3f00fu;
    for (i = 0; i < 32; i++) { c = i; if (_rotr(_rotl(x, c), c) != 0xa5c3f00fu) bad++; if (_lrotr(_lrotl(x, c), c) != 0xa5c3f00fu) bad++; }
    CHECK_EQ_I(bad, 0);
}

int main(void) { tc_begin(); TC_LOAD_PROC(_rotl); TC_LOAD_PROC(_rotr); TC_LOAD_PROC(_lrotl); TC_LOAD_PROC(_lrotr); if (!(tc_p__rotl && tc_p__rotr && tc_p__lrotl && tc_p__lrotr)) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; } TC_RUN(basic); TC_RUN(zero_count); TC_RUN(byte); TC_RUN(long_variants); TC_RUN(inverse); return tc_end(); }
