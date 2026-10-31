/* TC
 * id:    dat_fmode_wtext_u8_ucrt
 * area:  data_exports
 * funcs: _get_fmode _set_fmode
 * crt:   >=140
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _set_fmode (lists _O_TEXT, _O_BINARY, _O_WTEXT, _O_U16TEXT, _O_U8TEXT)
 * note:  PROBE: whether _set_fmode on ucrtbase accepts the wide/Unicode translation modes and stores them unchanged.
 *        Older CRTs reject these, hence ucrt only and a separate test.
 */
#include "tc.h"
#include <stdlib.h>
#include <fcntl.h>

#define WTEXT   0x10000
#define U16TEXT 0x20000
#define U8TEXT  0x40000

typedef int *(__cdecl *pf_t)(void);
static pf_t tc_crt_proc_fmode(void) { return (pf_t)tc_crt_proc("__p__fmode"); }
static int cur(void) { int m = -99; _get_fmode(&m); return m; }
static void rt(int mode)
{
    int orig = cur();
    CHECK(tc_iph_install());
    tc_iph_count = 0;
    CHECK_EQ_I(_set_fmode(mode), 0);
    CHECK_EQ_I(cur(), mode);
    CHECK_EQ_I(tc_iph_count, 0);
    { pf_t f = tc_crt_proc_fmode(); if (f) *f() = orig; }
}
TC_CASE(wtext_roundtrip)   { rt(WTEXT); }
TC_CASE(u16text_roundtrip) { rt(U16TEXT); }
TC_CASE(u8text_roundtrip)  { rt(U8TEXT); }

int main(void)
{
    tc_begin();
    TC_RUN(wtext_roundtrip); TC_RUN(u16text_roundtrip); TC_RUN(u8text_roundtrip);
    return tc_end();
}
