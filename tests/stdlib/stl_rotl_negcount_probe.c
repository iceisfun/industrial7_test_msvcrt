/* TC
 * id:    stl_rotl_negcount_probe
 * area:  stdlib
 * funcs: 
 * procs: _rotl _rotr _lrotl _lrotr
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _rotl (counts not specified outside 0..31)
 * note:  PROBE: behaviour of out-of-range (32, 33, 100) / negative rotate counts. Expected from rol/ror semantics (count & 31), but docs do not promise it.
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

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

static volatile unsigned int x = 0x12345678u; static volatile int c; static volatile unsigned long lx = 0x12345678ul;

TC_CASE(full_count) { x = 0xdeadbeefu; c = 32; CHECK_EQ_U(_rotl(x, c), 0xdeadbeefu); CHECK_EQ_U(_rotr(x, c), 0xdeadbeefu); }
TC_CASE(wrap_33) { x = 0x12345678u; c = 33; CHECK_EQ_U(_rotl(x, c), 0x2468ACF0u); CHECK_EQ_U(_rotr(x, c), 0x091A2B3Cu); x = 0x12345678u; }
TC_CASE(rotl_neg) { c = -1; CHECK_EQ_U(_rotl(x, c), _rotr(x, 1)); CHECK_EQ_U(_rotl(x, c), 0x091A2B3Cu); }
TC_CASE(rotr_neg) { c = -4; CHECK_EQ_U(_rotr(x, c), _rotl(x, 4)); CHECK_EQ_U(_rotr(x, c), 0x23456781u); }
TC_CASE(int_min_count) { c = INT_MIN; CHECK_EQ_U(_rotl(x, c), 0x12345678u); CHECK_EQ_U(_rotr(x, c), 0x12345678u); }
TC_CASE(large_count) { c = 100; CHECK_EQ_U(_rotl(x, c), _rotl(x, 4)); c = 36; CHECK_EQ_U(_rotr(x, c), _rotr(x, 4)); }
TC_CASE(long_variants) { c = -1; CHECK_EQ_U(_lrotl(lx, c), 0x091A2B3Cu); c = 100; CHECK_EQ_U(_lrotl(lx, c), _lrotl(lx, 4)); c = -4; CHECK_EQ_U(_lrotr(lx, c), 0x23456781u); }

int main(void) { tc_begin(); TC_LOAD_PROC(_rotl); TC_LOAD_PROC(_rotr); TC_LOAD_PROC(_lrotl); TC_LOAD_PROC(_lrotr); if (!(tc_p__rotl && tc_p__rotr && tc_p__lrotl && tc_p__lrotr)) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; } TC_RUN(full_count); TC_RUN(wrap_33); TC_RUN(rotl_neg); TC_RUN(rotr_neg); TC_RUN(int_min_count); TC_RUN(large_count); TC_RUN(long_variants); return tc_end(); }
