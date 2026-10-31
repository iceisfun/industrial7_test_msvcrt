/* TC
 * id:    pfm_3_flag_zero_minus_override
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf flag directives (0 ignored if - present; 0 ignored for integer conversions when precision given); C11 7.21.6.1p6 (the 0 flag is ignored if the - flag appears; for d i o u x X the 0 flag is ignored if a precision is specified)
 * note:  The "0 ignored when a precision is given" cases are asserted only for ucrtbase here; pre-UCRT behaviour is probed in
 *        pfm_3_flag_zero_prec_old_probe. "%05s" (0 flag on strings, undefined in ISO C) is probed in pfm_2_flag_zero_string_char_probe, not here.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile int v42 = 42, v7 = 7, vm7 = -7;
static volatile unsigned u255 = 255;
static volatile double d15 = 1.5;

TC_CASE(minus_beats_zero_int)
{
    char b[64];
    sprintf(b, "[%-05d][%0-5d][%-05i][%-05u]", v42, v42, v42, u255);
    CHECK_STR(b, "[42   ][42   ][42   ][255  ]");
}
TC_CASE(minus_beats_zero_hex)
{
    char b[64];
    sprintf(b, "[%-06x][%0-6X][%-#06o]", u255, u255, u255);
    CHECK_STR(b, "[ff    ][FF    ][0377  ]");
}
#ifdef TC_UCRT
TC_CASE(zero_ignored_with_prec)
{
    char b[96];
    sprintf(b, "[%05.3d][%05.3d][%08.3x][%06.2u]", v7, vm7, u255, v7);
    CHECK_STR(b, "[  007][ -007][     0ff][    07]");
}
TC_CASE(zero_with_prec_zero_value)
{
    char b[64];
    sprintf(b, "[%05.0d][%05.d]", 0, 0);
    CHECK_STR(b, "[     ][     ]");
}
#endif
TC_CASE(minus_beats_zero_float)
{
    char b[96];
    sprintf(b, "[%-09f][%0-9f][%-09.1f]", d15, d15, d15);
    CHECK_STR(b, "[1.500000 ][1.500000 ][1.5      ]");
}
TC_CASE(zero_works_alone)
{
    char b[64];
    sprintf(b, "[%05d][%05d][%09.3f]", v42, vm7, d15);
    CHECK_STR(b, "[00042][-0007][00001.500]");
}
TC_CASE(wide_variants)
{
    wchar_t b[96];
    _snwprintf(b, 96, L"[%-05d][%-09f][%05d]", v42, d15, vm7);
    CHECK_WSTR(b, L"[42   ][1.500000 ][-0007]");
}

int main(void)
{
    tc_begin();
    TC_RUN(minus_beats_zero_int); TC_RUN(minus_beats_zero_hex);
#ifdef TC_UCRT
    TC_RUN(zero_ignored_with_prec); TC_RUN(zero_with_prec_zero_value);
#endif
    TC_RUN(minus_beats_zero_float); TC_RUN(zero_works_alone); TC_RUN(wide_variants);
    return tc_end();
}
