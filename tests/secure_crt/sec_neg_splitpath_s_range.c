/* TC
 * id:    sec_neg_splitpath_s_range
 * area:  secure_crt
 * funcs: _splitpath_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _splitpath_s (ERANGE when a component buffer is too small)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

static const char *volatile v_path = "C:\\dir\\name.ext";   /* drive "C:", dir "\dir\", fname "name", ext ".ext" */

TC_CASE(drive_too_small_ERANGE) { char dr[2], di[16], fn[16], ex[16]; int r; r = _splitpath_s(v_path, dr, 2, di, 16, fn, 16, ex, 16); CHECK_EQ_I(r, ERANGE); }
TC_CASE(dir_too_small_ERANGE) { char dr[8], di[5], fn[16], ex[16]; int r; r = _splitpath_s(v_path, dr, 8, di, 5, fn, 16, ex, 16); CHECK_EQ_I(r, ERANGE); }
TC_CASE(fname_too_small_ERANGE) { char dr[8], di[16], fn[4], ex[16]; int r; r = _splitpath_s(v_path, dr, 8, di, 16, fn, 4, ex, 16); CHECK_EQ_I(r, ERANGE); }
TC_CASE(ext_too_small_ERANGE) { char dr[8], di[16], fn[16], ex[4]; int r; r = _splitpath_s(v_path, dr, 8, di, 16, fn, 16, ex, 4); CHECK_EQ_I(r, ERANGE); }
TC_CASE(errno_ERANGE) { char dr[8], di[16], fn[4], ex[16]; errno = 0; _splitpath_s(v_path, dr, 8, di, 16, fn, 4, ex, 16); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(handler_called) { char dr[8], di[16], fn[4], ex[16]; tc_iph_count = 0; _splitpath_s(v_path, dr, 8, di, 16, fn, 4, ex, 16); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(drive_too_small_ERANGE); TC_RUN(dir_too_small_ERANGE); TC_RUN(fname_too_small_ERANGE);
    TC_RUN(ext_too_small_ERANGE); TC_RUN(errno_ERANGE); TC_RUN(handler_called);
    return tc_end();
}
