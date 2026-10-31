/* TC
 * id:    cv_neg_ltow_ultow_nonsafe_probe
 * area:  conv
 * funcs: _ltow _ultow _i64tow _ui64tow _itow
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _itoa, _itow family (non-_s versions have no documented error returns)
 * note:  PROBES: with the invalid-parameter handler installed, an out-of-range radix (1, 37, 0) makes the function fire the handler once,
 *        return its buffer argument and leave an empty string; a NULL buffer fires the handler and returns NULL without crashing (child process).
 */
#include "tc.h"
#include <stdlib.h>

static int radices[] = { 1, 37, 0 };
static int child_main(const char *n);

static int child_main(const char *n)
{
    wchar_t *r;
    tc_iph_install();
    if (n[0] == 'r') {   /* "radix": returns 77 if every call returned its buffer, emptied it and fired the handler once */
        size_t i;
        for (i = 0; i < 3; i++) {   /* first pass: handler counts only (most likely behaviour) */
            wchar_t w[80]; int n0 = tc_iph_count;
            _ltow(5, w, radices[i]); _ultow(5, w, radices[i]); _i64tow(5, w, radices[i]);
            _ui64tow(5, w, radices[i]); _itow(5, w, radices[i]);
            if (tc_iph_count - n0 != 5) return 90 + (int)i;
        }
        for (i = 0; i < 3; i++) {   /* second pass: return value and emptied buffer (more speculative) */
            wchar_t w[80];
            w[0] = L'z'; r = _ltow(5, w, radices[i]); if (r != w || w[0]) return 80 + (int)i;
            w[0] = L'z'; r = _ultow(5, w, radices[i]); if (r != w || w[0]) return 80 + (int)i;
            w[0] = L'z'; r = _i64tow(5, w, radices[i]); if (r != w || w[0]) return 80 + (int)i;
            w[0] = L'z'; r = _ui64tow(5, w, radices[i]); if (r != w || w[0]) return 80 + (int)i;
            w[0] = L'z'; r = _itow(5, w, radices[i]); if (r != w || w[0]) return 80 + (int)i;
        }
        return 77;
    }
    r = _ultow(5, NULL, 10);
    return r == NULL ? 77 : 78;
}
TC_CASE(bad_radix)
{
    DWORD code = 0; int ran = tc_spawn_self("radix", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 77);   /* 80+i: radix[i] result/buffer wrong; 90+i: handler count wrong */
}
TC_CASE(null_buffer)
{
    DWORD code = 0; int ran = tc_spawn_self("nullbuf", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 77);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(bad_radix); TC_RUN(null_buffer);
    return tc_end();
}
