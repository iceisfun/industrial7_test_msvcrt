/* TC
 * id:    pfs_probe_str_ls_ws_upper_prefix
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf type field characters
 * note:  PROBE: %lS and %wS in narrow sprintf print a wide string (S already means wide; does an explicit l/w prefix change that?). Expected abc on all.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const wchar_t *volatile wabc = L"abc";

TC_CASE(lS) { char b[16]; int n = sprintf(b, "[%lS]", wabc); CHECK_STR(b, "[abc]"); CHECK_EQ_I(n, 5); }
TC_CASE(wS) { char b[16]; int n = sprintf(b, "[%wS]", wabc); CHECK_STR(b, "[abc]"); CHECK_EQ_I(n, 5); }

int main(void) { tc_begin(); TC_RUN(lS); TC_RUN(wS); return tc_end(); }
