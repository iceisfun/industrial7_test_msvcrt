/* TC
 * id:    scn_neg_snscanf_guard_page
 * area:  scanf
 * funcs: _snscanf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snscanf (count bounds the characters read; source need not be NUL terminated)
 * note:  The source sits flush against the end of a committed page followed by a PAGE_NOACCESS page; any read of byte count crashes (AV = fail).
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile size_t cnt;
static char *base;
static size_t pg;

static char *place(const char *s)
{
    size_t n = strlen(s); char *p = base + pg - n;
    memcpy(p, s, n); cnt = n; return p;
}

TC_CASE(setup) { CHECK(base != NULL); }
TC_CASE(d)  { int v = 0, r; char *p; if (!base) TC_SKIP("no mem"); p = place("12345"); r = _snscanf(p, cnt, "%d", &v); CHECK_EQ_I(r, 1); CHECK_EQ_I(v, 12345); }
TC_CASE(s)  { char o[16] = ""; int r; char *p; if (!base) TC_SKIP("no mem"); p = place("abcde"); r = _snscanf(p, cnt, "%s", o); CHECK_EQ_I(r, 1); CHECK_STR(o, "abcde"); }
TC_CASE(c)  { char o[3] = {0,0,0}; int r; char *p; if (!base) TC_SKIP("no mem"); p = place("xyz"); r = _snscanf(p, cnt, "%c%c%c", &o[0], &o[1], &o[2]); CHECK_EQ_I(r, 3); CHECK(o[0]=='x' && o[1]=='y' && o[2]=='z'); }
TC_CASE(c_more_than_available) { char o[4] = {'-','-','-','-'}; int r; char *p; if (!base) TC_SKIP("no mem"); p = place("xy"); r = _snscanf(p, cnt, "%c%c%c", &o[0], &o[1], &o[2]); CHECK_EQ_I(r, 2); CHECK(o[2]=='-'); }

int main(void)
{
    SYSTEM_INFO si; DWORD old;
    tc_begin();
    GetSystemInfo(&si); pg = si.dwPageSize;
    base = (char *)VirtualAlloc(NULL, pg * 2, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (base && !VirtualProtect(base + pg, pg, PAGE_NOACCESS, &old)) { VirtualFree(base, 0, MEM_RELEASE); base = NULL; }
    TC_RUN(setup); TC_RUN(d); TC_RUN(s); TC_RUN(c); TC_RUN(c_more_than_available);
    return tc_end();
}
