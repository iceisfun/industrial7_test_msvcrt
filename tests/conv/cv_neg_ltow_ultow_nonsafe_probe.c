/* TC
 * id:    cv_neg_ltow_ultow_nonsafe_probe
 * area:  conv
 * funcs: _ltow _ultow _i64tow _ui64tow _itow
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _itoa, _itow family (non-_s versions have no documented error returns)
 * note:  OBSERVED (Win10 22H2 19045 ucrtbase, run twice; class C for radix 0/1/NULL): _ltow/_ultow/_i64tow/_ui64tow/_itow do not validate the radix or the
 *        buffer. Radix 37 is accepted (buffer L"5", handler not called); radix 0 -> 0xC0000094 (divide by zero); radix 1 -> 0xC0000005 (runaway loop);
 *        NULL buffer -> 0xC0000005. The original probe expected handler once + empty string + NULL-safe return. Each case runs in a child process.
 */
#include "tc.h"
#include <stdlib.h>

static volatile int vrad;
static int child_main(const char *n)
{
    wchar_t w[80];
    tc_iph_install();
    if (n[0] == 'r' && n[1] == '3') {   /* "r37": every call returns its buffer holding L"5", no handler call, no errno */
        wchar_t *r;
        tc_iph_count = 0; vrad = 37;
        w[0] = L'z'; r = _ltow(5, w, vrad); if (r != w || w[0] != L'5' || w[1]) return 80;
        w[0] = L'z'; r = _ultow(5, w, vrad); if (r != w || w[0] != L'5' || w[1]) return 81;
        w[0] = L'z'; r = _i64tow(5, w, vrad); if (r != w || w[0] != L'5' || w[1]) return 82;
        w[0] = L'z'; r = _ui64tow(5, w, vrad); if (r != w || w[0] != L'5' || w[1]) return 83;
        w[0] = L'z'; r = _itow(5, w, vrad); if (r != w || w[0] != L'5' || w[1]) return 84;
        return tc_iph_count == 0 ? 77 : 85;
    }
    if (n[0] == 'r') {   /* "r0" / "r1": one call per function selected by n[2] */
        vrad = n[1] - '0';
        switch (n[2]) {
        case 'a': _ltow(5, w, vrad); break;
        case 'b': _ultow(5, w, vrad); break;
        case 'c': _i64tow(5, w, vrad); break;
        case 'd': _ui64tow(5, w, vrad); break;
        default:  _itow(5, w, vrad); break;
        }
        return 77;
    }
    { wchar_t *volatile np = NULL; _ultow(5, np, 10); }
    return 77;
}
static void expect(const char *name, DWORD want)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, want);
}
/* observed Win10 22H2 ucrtbase: no radix/buffer validation in the non-_s wide routines. */
TC_CASE(radix_37_accepted) { expect("r37", 77); }
TC_CASE(radix_0_divide_by_zero)
{
    expect("r0a", 0xC0000094u); expect("r0b", 0xC0000094u); expect("r0c", 0xC0000094u); expect("r0d", 0xC0000094u); expect("r0e", 0xC0000094u);
}
TC_CASE(radix_1_access_violation)
{
    expect("r1a", 0xC0000005u); expect("r1b", 0xC0000005u); expect("r1c", 0xC0000005u); expect("r1d", 0xC0000005u); expect("r1e", 0xC0000005u);
}
TC_CASE(null_buffer_access_violation) { expect("nullbuf", 0xC0000005u); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(radix_37_accepted); TC_RUN(radix_0_divide_by_zero); TC_RUN(radix_1_access_violation); TC_RUN(null_buffer_access_violation);
    return tc_end();
}
