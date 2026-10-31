/* TC
 * id:    mb_probe_mbsnbcpy_s_dangling_lead
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * procs: _mbsnbcpy_s
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   none (MS docs do not say how a lone lead byte before the terminator is handled by the _s form)
 * note:  PROBE: _mbsnbcpy_s(dst, 16, "ab" + lone lead 0x82 + NUL, 10), with the NUL as the last byte before a
 *        PAGE_NOACCESS page. Guess: no over-read, "ab" is copied, the result is terminated at index 2 or 3, and bytes
 *        from index 4 on are untouched. A different outcome (error return that clears dst[0]) is plausible.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

typedef int (__cdecl *nbcpy_s_t)(unsigned char *, size_t, const unsigned char *, size_t);
static nbcpy_s_t p_nbcpy_s;
static int old_cp;
static unsigned char *guarded_src;

static int make_src(void)
{
    SYSTEM_INFO si; unsigned char *base; DWORD old; size_t pg;
    GetSystemInfo(&si); pg = si.dwPageSize;
    base = (unsigned char *)VirtualAlloc(NULL, pg * 2, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!base) return 0;
    if (!VirtualProtect(base + pg, pg, PAGE_NOACCESS, &old)) return 0;
    guarded_src = base + pg - 4;
    guarded_src[0] = 'a'; guarded_src[1] = 'b'; guarded_src[2] = 0x82; guarded_src[3] = 0;
    return 1;
}
TC_CASE(terminated_probe)
{
    unsigned char dst[16]; int i;
    if (!guarded_src) TC_SKIP("guard page unavailable");
    if (!p_nbcpy_s) TC_SKIP("not exported");
    for (i = 0; i < 16; i++) dst[i] = 0xCC;
    p_nbcpy_s(dst, sizeof dst, guarded_src, 10);
    CHECK_EQ_U(dst[0], 'a'); CHECK_EQ_U(dst[1], 'b');
    CHECK(dst[2] == 0 || dst[3] == 0);
}
TC_CASE(sentinel_probe)
{
    unsigned char dst[16]; int i;
    if (!guarded_src) TC_SKIP("guard page unavailable");
    if (!p_nbcpy_s) TC_SKIP("not exported");
    for (i = 0; i < 16; i++) dst[i] = 0xCC;
    p_nbcpy_s(dst, sizeof dst, guarded_src, 10);
    for (i = 4; i < 16; i++) CHECK_EQ_U(dst[i], 0xCC);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_nbcpy_s = (nbcpy_s_t)tc_crt_proc("_mbsnbcpy_s");
    old_cp = _getmbcp(); _setmbcp(932);
    make_src();
    TC_RUN(terminated_probe); TC_RUN(sentinel_probe);
    _setmbcp(old_cp);
    return tc_end();
}
