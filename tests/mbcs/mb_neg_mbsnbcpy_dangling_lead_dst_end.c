/* TC
 * id:    mb_neg_mbsnbcpy_dangling_lead_dst_end
 * area:  mbcs
 * funcs: _mbsnbcpy _mbsncpy _setmbcp _getmbcp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _mbsnbcpy ("if the last byte copied is a lead byte, it is replaced with a null"), _mbsncpy
 * note:  cp932. The source "ab" + lone lead 0x82 + NUL is placed so the NUL is the last byte before a PAGE_NOACCESS page:
 *        any read past the terminator faults. Dangling lead must never be copied as the final byte of the result.
 *        The _mbsnbcpy_s variant is in mb_probe_mbsnbcpy_s_dangling_lead (return value/state undocumented).
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

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
TC_CASE(nbcpy_dangling_lead_no_overread)
{
    unsigned char dst[16]; int i;
    if (!guarded_src) TC_SKIP("guard page unavailable");
    for (i = 0; i < 16; i++) dst[i] = 0xCC;
    _mbsnbcpy(dst, guarded_src, 3);
    CHECK_EQ_U(dst[0], 'a'); CHECK_EQ_U(dst[1], 'b'); CHECK_EQ_U(dst[2], 0);
    for (i = 0; i < 16; i++) dst[i] = 0xCC;
    _mbsnbcpy(dst, guarded_src, 10);
    CHECK_EQ_U(dst[0], 'a'); CHECK_EQ_U(dst[1], 'b'); CHECK_EQ_U(dst[2], 0);
}
TC_CASE(ncpy_dangling_lead_no_overread)
{
    unsigned char dst[16]; int i;
    if (!guarded_src) TC_SKIP("guard page unavailable");
    for (i = 0; i < 16; i++) dst[i] = 0xCC;
    _mbsncpy(dst, guarded_src, 10);
    CHECK_EQ_U(dst[0], 'a'); CHECK_EQ_U(dst[1], 'b'); CHECK_EQ_U(dst[2], 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    old_cp = _getmbcp(); _setmbcp(932);
    make_src();
    TC_RUN(nbcpy_dangling_lead_no_overread); TC_RUN(ncpy_dangling_lead_no_overread);
    _setmbcp(old_cp);
    return tc_end();
}
