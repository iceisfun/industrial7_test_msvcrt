/* TC
 * id:    pfm_3_conv_z_flags_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcrt msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf Type Field Characters (Z: ANSI_STRING / UNICODE_STRING passed by address); ucrtbase rejects %Z (pfm_1_neg_z_ucrt_probe)
 * note:  PROBE: %Z prints Length bytes of an ANSI_STRING; width pads like a string ('-' left-justifies); the '+', ' ' and '#' flags and a
 *        flag-only '0' (no width) have no visible effect.
 */
#include "tc.h"
#include <stdio.h>

typedef struct { USHORT Length, MaximumLength; char *Buffer; } TC_ANSI;
static char buf[8] = { 'a', 'b', 'c', 'X', 'X', 'X', 'X', 'X' };
static TC_ANSI as = { 3, 8, buf };
static TC_ANSI * volatile pas = &as;
static const char * volatile vf;

static void fmt1(const char *f, char *b) { vf = f; sprintf(b, vf, pas); }

TC_CASE(minus_width) { char b[48]; fmt1("[%-12Z]", b); CHECK_STR(b, "[abc         ]"); }
TC_CASE(width)       { char b[48]; fmt1("[%12Z]", b);  CHECK_STR(b, "[         abc]"); }
TC_CASE(plus)        { char b[48]; fmt1("[%+Z]", b);   CHECK_STR(b, "[abc]"); }
TC_CASE(space)       { char b[48]; fmt1("[% Z]", b);   CHECK_STR(b, "[abc]"); }
TC_CASE(hash)        { char b[48]; fmt1("[%#Z]", b);   CHECK_STR(b, "[abc]"); }
TC_CASE(zero)        { char b[48]; fmt1("[%0Z]", b);   CHECK_STR(b, "[abc]"); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(minus_width); TC_RUN(width); TC_RUN(plus); TC_RUN(space); TC_RUN(hash); TC_RUN(zero);
    return tc_end();
}
