/* TC
 * id:    scn_neg_sscanf_errno_untouched_probe
 * area:  scanf
 * kind:  negative
 * funcs: sscanf swscanf
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.2 (matching failure and EOF are not errors); MS docs errno
 * note:  Probes that ordinary input mismatches and EOF in scanf do not touch errno (neither set nor cleared).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static volatile const char *vin;
static volatile const wchar_t *vwin;

TC_CASE(d_mismatch_errno_0) { int a = 1, r; char s[8] = "", c = 0; errno = 0; vin = "xyz"; r = sscanf((const char *)vin, "%d", &a); CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, 0);
    errno = 0; r = sscanf((const char *)vin, "%[0-9]", s); CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, 0);
    errno = 0; vwin = L"xyz"; r = swscanf((const wchar_t *)vwin, L"%d", &a); CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, 0); (void)c; }
TC_CASE(eof_errno_0) { int a = 1, r; char c = 0, s[8]; errno = 0; vin = ""; r = sscanf((const char *)vin, "%d", &a); CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, 0);
    errno = 0; r = sscanf((const char *)vin, "%c", &c); CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, 0);
    errno = 0; r = sscanf((const char *)vin, "%s", s); CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, 0); }
TC_CASE(preset_errno_preserved) { int a = 1, r; errno = ENOENT; vin = "xyz"; r = sscanf((const char *)vin, "%d", &a); CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, ENOENT);
    errno = ENOENT; vin = "12"; r = sscanf((const char *)vin, "%d", &a); CHECK_EQ_I(r, 1); CHECK_EQ_I(errno, ENOENT); }
TC_CASE(float_mismatch_errno_0) { double d = 1.0; int r; errno = 0; vin = "abc"; r = sscanf((const char *)vin, "%lf", &d); CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, 0); CHECK_BITS_D(d, 1.0); }

int main(void) { tc_begin(); TC_RUN(d_mismatch_errno_0); TC_RUN(eof_errno_0); TC_RUN(preset_errno_preserved); TC_RUN(float_mismatch_errno_0); return tc_end(); }
