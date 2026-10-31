/* TC
 * id:    str_strlwr_upr_basic
 * area:  string
 * funcs: _strlwr _strupr
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _strlwr, _strupr

 */
#include "tc.h"
#include <string.h>
#include <errno.h>
static char *(*volatile p_lwr)(char *) = _strlwr;
static char *(*volatile p_upr)(char *) = _strupr;
TC_CASE(lower) { char b[16] = "HeLLo WoRLD"; p_lwr(b); CHECK_STR(b, "hello world"); }
TC_CASE(upper) { char b[16] = "HeLLo WoRLD"; p_upr(b); CHECK_STR(b, "HELLO WORLD"); }
TC_CASE(unchanged) { char b[24] = "123 !@#[`{~_-"; char c[24] = "123 !@#[`{~_-"; p_lwr(b); CHECK_STR(b, c); p_upr(b); CHECK_STR(b, c); }
TC_CASE(empty) { char b[4] = ""; CHECK_STR(p_lwr(b), ""); CHECK_STR(p_upr(b), ""); }
TC_CASE(same_ptr) { char b[8] = "aB"; CHECK_EQ_P(p_lwr(b), b); CHECK_EQ_P(p_upr(b), b); }
int main(void) { tc_begin(); TC_RUN(lower); TC_RUN(upper); TC_RUN(unchanged); TC_RUN(empty); TC_RUN(same_ptr); return tc_end(); }
