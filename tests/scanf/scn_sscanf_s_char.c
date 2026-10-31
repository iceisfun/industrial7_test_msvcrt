/* TC
 * id:    scn_sscanf_s_char
 * area:  scanf
 * funcs: sscanf_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs scanf_s: for %c the size argument is the buffer size in characters; %c never appends a NUL
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;
TC_CASE(c_size1) { char c = 'Z'; int n; v = "xyz"; n = sscanf_s((const char *)v, "%c", &c, (unsigned)1); CHECK_EQ_I(n, 1); CHECK_EQ_I(c, 'x'); }
TC_CASE(c5_size5) { char a[8]; int n, i, pos = -1; for (i = 0; i < 8; i++) a[i] = 'Z'; v = "hello world"; n = sscanf_s((const char *)v, "%5c%n", a, (unsigned)5, &pos); CHECK_EQ_I(n, 1); CHECK_MEM(a, "hello", 5); CHECK_EQ_I(a[5], 'Z'); CHECK_EQ_I(pos, 5); }
TC_CASE(c_no_ws_skip) { char c = 'Z'; int n; v = " q"; n = sscanf_s((const char *)v, "%c", &c, (unsigned)1); CHECK_EQ_I(n, 1); CHECK_EQ_I(c, ' '); }
int main(void) { tc_begin(); TC_RUN(c_size1); TC_RUN(c5_size5); TC_RUN(c_no_ws_skip); return tc_end(); }
