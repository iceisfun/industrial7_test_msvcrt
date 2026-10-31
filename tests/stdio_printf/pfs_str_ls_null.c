/* TC
 * id:    pfs_str_ls_null
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs printf type field characters
 * note:  NULL string pointers print (null) in narrow output
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const wchar_t *volatile wnull = NULL;

TC_CASE(ls) { char b[32]; int n = sprintf(b, "%ls", wnull); CHECK_STR(b, "(null)"); CHECK_EQ_I(n, 6); }
TC_CASE(S)  { char b[32]; sprintf(b, "%S", wnull);  CHECK_STR(b, "(null)"); }
TC_CASE(ws) { char b[32]; sprintf(b, "%ws", wnull); CHECK_STR(b, "(null)"); }
TC_CASE(width) { char b[32]; int n = sprintf(b, "[%8ls]", wnull); CHECK_STR(b, "[  (null)]"); CHECK_EQ_I(n, 10); }

int main(void) { tc_begin(); TC_RUN(ls); TC_RUN(S); TC_RUN(ws); TC_RUN(width); return tc_end(); }
