/* TC
 * id:    pfm_2_conv_z_length_probe
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   msvcrt msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf type field Z / wZ (ANSI_STRING, UNICODE_STRING); ucrtbase rejects %Z (pfm_1_neg_z_ucrt_probe)
 * note:  PROBE: %Z prints Length bytes of ANSI_STRING.Buffer (not up to NUL); %wZ prints Length/2 wchar_t of UNICODE_STRING.Buffer.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

typedef struct { USHORT Length, MaximumLength; char *Buffer; } TC_ANSI;
typedef struct { USHORT Length, MaximumLength; wchar_t *Buffer; } TC_UNI;

TC_CASE(Z_length) {
    char b[32]; char buf[8] = { 'a','b','c','d','X','X','X','X' }; TC_ANSI a; TC_ANSI * volatile pa = &a;
    a.Length = 2; a.MaximumLength = 8; a.Buffer = buf;
    sprintf(b, "[%Z]", pa); CHECK_STR(b, "[ab]");
}
TC_CASE(Z_zero_length) {
    char b[32]; char buf[4] = { 'a','b','c','d' }; TC_ANSI a; TC_ANSI * volatile pa = &a;
    a.Length = 0; a.MaximumLength = 4; a.Buffer = buf;
    sprintf(b, "[%Z]", pa); CHECK_STR(b, "[]");
}
TC_CASE(wZ_narrow_out) {
    char b[32]; wchar_t buf[6] = { L'a', L'b', L'c', L'd', L'X', L'X' }; TC_UNI u; TC_UNI * volatile pu = &u;
    u.Length = 2 * sizeof(wchar_t); u.MaximumLength = sizeof buf; u.Buffer = buf;
    sprintf(b, "[%wZ]", pu); CHECK_STR(b, "[ab]");
}
TC_CASE(wZ_wide_out) {
    wchar_t b[32]; wchar_t buf[6] = { L'a', L'b', L'c', L'd', L'X', L'X' }; TC_UNI u; TC_UNI * volatile pu = &u;
    u.Length = 2 * sizeof(wchar_t); u.MaximumLength = sizeof buf; u.Buffer = buf;
    _snwprintf(b, 31, L"[%wZ]", pu); CHECK_WSTR(b, L"[ab]");
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(Z_length); TC_RUN(Z_zero_length); TC_RUN(wZ_narrow_out); TC_RUN(wZ_wide_out);
    return tc_end();
}
