/* TC
 * id:    str_strset_basic
 * area:  string
 * funcs: _strset _strnset
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _strset, _strnset

 */
#include "tc.h"
#include <string.h>
#include <errno.h>
TC_CASE(set_all) { char b[8] = "abcd"; char *r = _strset(b, 'x'); CHECK_STR(b, "xxxx"); CHECK_EQ_P(r, b); CHECK_EQ_U(b[4], 0); }
TC_CASE(nset_partial) { char b[8] = "abcdef"; char *r = _strnset(b, 'x', 2); CHECK_STR(b, "xxcdef"); CHECK_EQ_P(r, b); }
TC_CASE(nset_over) { char b[8] = "abc"; _strnset(b, 'y', 10); CHECK_STR(b, "yyy"); CHECK_EQ_U(b[3], 0); }
TC_CASE(nset_zero) { char b[8] = "abc"; _strnset(b, 'z', 0); CHECK_STR(b, "abc"); }
TC_CASE(set_empty) { char b[4] = ""; _strset(b, 'q'); CHECK_STR(b, ""); CHECK_EQ_U(b[0], 0); }
int main(void) { tc_begin(); TC_RUN(set_all); TC_RUN(nset_partial); TC_RUN(nset_over); TC_RUN(nset_zero); TC_RUN(set_empty); return tc_end(); }
