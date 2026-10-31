/* TC
 * id:    str_strset_s_basic
 * area:  string
 * funcs: _strset_s _strnset_s
 * procs: 
 * crt:   >=100
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _strset_s, _strnset_s

 */
#include "tc.h"
#include <string.h>
#include <errno.h>
TC_CASE(set_fill) { char b[8] = "abcd"; CHECK_EQ_I(_strset_s(b, sizeof b, 'x'), 0); CHECK_STR(b, "xxxx"); }
TC_CASE(nset_partial) { char b[8] = "abcdef"; CHECK_EQ_I(_strnset_s(b, sizeof b, 'x', 2), 0); CHECK_STR(b, "xxcdef"); }
TC_CASE(nset_over) { char b[16] = "abc"; CHECK_EQ_I(_strnset_s(b, sizeof b, 'y', 10), 0); CHECK_STR(b, "yyy"); CHECK_EQ_U(b[3], 0); }
TC_CASE(nset_zero) { char b[8] = "abc"; CHECK_EQ_I(_strnset_s(b, sizeof b, 'z', 0), 0); CHECK_STR(b, "abc"); }
int main(void) { tc_begin(); TC_RUN(set_fill); TC_RUN(nset_partial); TC_RUN(nset_over); TC_RUN(nset_zero); return tc_end(); }
