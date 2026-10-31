/* TC
 * id:    scn_sscanf_empty_format
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p16
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(empty_fmt) { int r = sscanf("abc", ""); CHECK_EQ_I(r, 0); }
TC_CASE(literal_fmt) { int r = sscanf("abc", "abc"); CHECK_EQ_I(r, 0); }
TC_CASE(ws_only_fmt) { int r = sscanf("  abc", " "); CHECK_EQ_I(r, 0); }

int main(void) { tc_begin(); TC_RUN(empty_fmt); TC_RUN(literal_fmt); TC_RUN(ws_only_fmt); return tc_end(); }
