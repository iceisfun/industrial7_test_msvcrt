/* TC
 * id:    pfm_3_conv_z_length_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcrt msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf Type Field Characters: "Z ... Use hZ for ANSI_STRING, and lZ or wZ for UNICODE_STRING"; ucrtbase rejects %Z (pfm_1_neg_z_ucrt_probe)
 * note:  PROBE: %hZ with an ANSI_STRING and %lZ with a UNICODE_STRING print the buffer contents (Length bytes / Length/2 wchar_t) in a narrow
 *        sprintf. %wZ is covered by pfm_2_conv_z_length_probe. The legacy-CRT export set varies, hence no ucrtbase.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

typedef struct { USHORT Length, MaximumLength; char *Buffer; } TC_ANSI;
typedef struct { USHORT Length, MaximumLength; wchar_t *Buffer; } TC_UNI;
static char abuf[8] = { 'h', 'i', 'j', 'X', 'X', 'X', 'X', 'X' };
static wchar_t ubuf[6] = { L'w', L'x', L'y', L'X', L'X', L'X' };
static TC_ANSI as = { 3, 8, abuf };
static TC_UNI us = { 3 * sizeof(wchar_t), sizeof ubuf, ubuf };
static void * volatile pas = &as;
static void * volatile pus = &us;
static const char * volatile vf;

TC_CASE(hZ)       { char b[48]; vf = "[%hZ]";   sprintf(b, vf, pas); CHECK_STR(b, "[hij]"); }
TC_CASE(lZ)       { char b[48]; vf = "[%lZ]";   sprintf(b, vf, pus); CHECK_STR(b, "[wxy]"); }
TC_CASE(hZ_width) { char b[48]; vf = "[%-5hZ]"; sprintf(b, vf, pas); CHECK_STR(b, "[hij  ]"); }
TC_CASE(lZ_width) { char b[48]; vf = "[%5lZ]";  sprintf(b, vf, pus); CHECK_STR(b, "[  wxy]"); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(hZ); TC_RUN(lZ); TC_RUN(hZ_width); TC_RUN(lZ_width);
    return tc_end();
}
