/* TC
 * id:    pfm_2_scprintf_z_probe
 * area:  stdio_printf
 * funcs: _scprintf
 * crt:   <140
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf type field characters (%Z: ANSI_STRING / UNICODE_STRING structure)
 * note:  PROBES: user-mode pre-ucrt CRTs honour %Z with a pointer to an ANSI_STRING {Length, MaximumLength, Buffer}
 *        and count Length bytes (here "abc" -> 3, and "[%Z]" -> 5). Docs list the type but are terse; the exact count is the guess.
 */
#include "tc.h"
#include <stdio.h>

struct tc_ansi { USHORT Length, MaximumLength; char *Buffer; };

TC_CASE(z_ansi)
{
    static char txt[] = "abc";
    struct tc_ansi a; const char * volatile f = "[%Z]";
    a.Length = 3; a.MaximumLength = 4; a.Buffer = txt;
    CHECK_EQ_I(_scprintf(f, &a), 5);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(z_ansi); return tc_end(); }
