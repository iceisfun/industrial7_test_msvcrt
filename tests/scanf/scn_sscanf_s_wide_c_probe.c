/* TC
 * id:    scn_sscanf_s_wide_c_probe
 * area:  scanf
 * funcs: sscanf_s
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs scanf_s: %C in a narrow function reads a wide character
 * note:  Probes: %C with sscanf_s and size 1 stores one wchar_t (the size argument counted in wchar_t elements, not bytes)
 *        and returns 1. The size unit for wide destinations in narrow functions is not clearly documented.
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;
TC_CASE(C_wide_size1) { wchar_t w = L'Z'; int n; v = "A"; n = sscanf_s((const char *)v, "%C", &w, (unsigned)1); CHECK_EQ_I(n, 1); CHECK_EQ_U(w, L'A'); }
int main(void) { tc_begin(); TC_RUN(C_wide_size1); return tc_end(); }
