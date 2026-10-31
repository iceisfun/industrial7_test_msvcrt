/* TC
 * id:    pfi_neg_probe_i16_i33_i8
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf width/size specifiers: only I, I32, I64 are documented
 * note:  Probes what happens for undocumented size prefixes %I16d, %I8d, %I33d. Asserted only: the call does not crash
 *        (each runs in a child with a counting invalid-parameter handler) and that a -1 result comes with exactly one handler call.
 *        Exit codes: 20 = result != -1, 21 = -1 with iph==1, 22 = -1 with iph!=1.
 */
#include "tc.h"
#include <stdio.h>

static const char *fmts[] = { "%I16d", "%I8d", "%I33d" };

static void run(int idx)
{
    DWORD code = 0; char nm[8]; int ran;
    nm[0] = 'f'; nm[1] = (char)('0' + idx); nm[2] = 0;
    ran = tc_spawn_self(nm, &code, 20000);
    CHECK(ran);
    CHECK(code == 20 || code == 21);
}
TC_CASE(I16) { run(0); }
TC_CASE(I8)  { run(1); }
TC_CASE(I33) { run(2); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char b[256]; volatile int v = 5; int r; volatile const char *f = fmts[c[1] - '0'];
        tc_iph_install(); tc_iph_count = 0;
        r = sprintf(b, (const char *)f, v);
        if (r != -1) return 20;
        return tc_iph_count == 1 ? 21 : 22;
    }
    TC_RUN(I16); TC_RUN(I8); TC_RUN(I33);
    return tc_end();
}
