/* TC
 * id:    pff_neg_swprintf_legacy_overload_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: swprintf _snwprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _snwprintf (returns negative when more than count chars are needed), swprintf (C99 form takes a count)
 * note:  Observed on Windows 10 22H2: (1) _snwprintf(NULL,0,L"%f",1.0) is tolerated (no crash, nothing written, no handler call) and returns 8, the
 *        would-be length, NOT -1; (2) _snwprintf(wbuf,0,...) leaves the buffer untouched; (3) the C99-style swprintf(wbuf,5,L"%f",1.0) returns -1 on
 *        truncation, never writes past 5 wchar_t, and NUL-terminates wbuf[4].
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

struct wb { wchar_t b[16]; wchar_t canary[8]; };
static void arm(struct wb *t) { int i; for (i = 0; i < 16; i++) t->b[i] = L'Q'; for (i = 0; i < 8; i++) t->canary[i] = (wchar_t)0x5a5a; }
static int canary_ok(const struct wb *t) { int i; for (i = 0; i < 8; i++) if (t->canary[i] != (wchar_t)0x5a5a) return 0; return 1; }

TC_CASE(null_count0)
{
    const wchar_t * volatile f = L"%f"; int r;
    r = _snwprintf(NULL, 0, f, 1.0);
    CHECK_EQ_I(r, 8);
}
TC_CASE(count0_untouched)
{
    struct wb t; const wchar_t * volatile f = L"%f"; int i, same = 1; arm(&t);
    (void)_snwprintf(t.b, 0, f, 1.0);
    for (i = 0; i < 16; i++) if (t.b[i] != L'Q') same = 0;
    CHECK(same); CHECK(canary_ok(&t));
}
TC_CASE(swprintf_c99_trunc)
{
    struct wb t; const wchar_t * volatile f = L"%f"; int r; arm(&t);
    r = swprintf(t.b, 5, f, 1.0);
    CHECK_EQ_I(r, -1); CHECK(canary_ok(&t)); CHECK_EQ_I(t.b[4], 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(null_count0); TC_RUN(count0_untouched); TC_RUN(swprintf_c99_trunc);
    return tc_end();
}
