/* TC
 * id:    str_strlwr_s_basic
 * area:  string
 * funcs: _strlwr_s _strupr_s
 * procs: 
 * crt:   >=100
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _strlwr_s, _strupr_s

 */
#include "tc.h"
#include <string.h>
#include <errno.h>
TC_CASE(success) { char b[16] = "HeLLo"; CHECK_EQ_I(_strlwr_s(b, sizeof b), 0); CHECK_STR(b, "hello"); CHECK_EQ_I(_strupr_s(b, sizeof b), 0); CHECK_STR(b, "HELLO"); }
TC_CASE(exact_size) { char b[6] = "AbCdE"; CHECK_EQ_I(_strlwr_s(b, 6), 0); CHECK_STR(b, "abcde"); CHECK_EQ_I(_strupr_s(b, 6), 0); CHECK_STR(b, "ABCDE"); }
TC_CASE(empty) { char b[1] = ""; CHECK_EQ_I(_strlwr_s(b, 1), 0); CHECK_EQ_I(_strupr_s(b, 1), 0); CHECK_EQ_U(b[0], 0); }
int main(void) { tc_begin(); TC_RUN(success); TC_RUN(exact_size); TC_RUN(empty); return tc_end(); }
