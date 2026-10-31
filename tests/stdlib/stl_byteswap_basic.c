/* TC
 * id:    stl_byteswap_basic
 * area:  stdlib
 * funcs:
 * procs: _byteswap_ushort _byteswap_ulong _byteswap_uint64
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _byteswap_ushort, _byteswap_ulong, _byteswap_uint64
 * note:  These are compiler intrinsics; resolved from the DLL at run time so the CRT export is what runs.
 *        msvcr80/msvcrt exports uncertain, so excluded.
 */
#include "tc.h"

typedef unsigned short (__cdecl *bs16_t)(unsigned short);
typedef unsigned long (__cdecl *bs32_t)(unsigned long);
typedef unsigned long long (__cdecl *bs64_t)(unsigned long long);
static bs16_t p16; static bs32_t p32; static bs64_t p64;

TC_CASE(ushort)
{
    if (!p16) TC_SKIP("_byteswap_ushort not exported");
    CHECK_EQ_U(p16(0x1234), 0x3412);
    CHECK_EQ_U(p16(0x1221), 0x2112);
    CHECK_EQ_U(p16(0), 0);
    CHECK_EQ_U(p16(0xFFFF), 0xFFFF);
    CHECK_EQ_U(p16(0x00FF), 0xFF00);
}
TC_CASE(ulong)
{
    if (!p32) TC_SKIP("_byteswap_ulong not exported");
    CHECK_EQ_U(p32(0x12345678UL), 0x78563412UL);
    CHECK_EQ_U(p32(0), 0);
    CHECK_EQ_U(p32(0xFFFFFFFFUL), 0xFFFFFFFFUL);
    CHECK_EQ_U(p32(0x000000FFUL), 0xFF000000UL);
}
TC_CASE(uint64)
{
    if (!p64) TC_SKIP("_byteswap_uint64 not exported");
    CHECK_EQ_U(p64(0x0102030405060708ULL), 0x0807060504030201ULL);
    CHECK_EQ_U(p64(0), 0);
    CHECK_EQ_U(p64(0xFFFFFFFFFFFFFFFFULL), 0xFFFFFFFFFFFFFFFFULL);
    CHECK_EQ_U(p64(0xFFULL), 0xFF00000000000000ULL);
}
TC_CASE(double_swap_identity)
{
    unsigned long long v = 0x0123456789ABCDEFULL;
    if (!p16 || !p32 || !p64) TC_SKIP("missing export");
    CHECK_EQ_U(p16(p16(0xBEEF)), 0xBEEF);
    CHECK_EQ_U(p32(p32(0xDEADBEEFUL)), 0xDEADBEEFUL);
    CHECK_EQ_U(p64(p64(v)), v);
}
TC_CASE(ushort_param_truncation)
{
    volatile unsigned int wide = 0xABCD1234u;
    if (!p16) TC_SKIP("_byteswap_ushort not exported");
    CHECK_EQ_U(p16((unsigned short)wide), 0x3412);
}
int main(void)
{
    tc_begin();
    p16 = (bs16_t)tc_crt_proc("_byteswap_ushort");
    p32 = (bs32_t)tc_crt_proc("_byteswap_ulong");
    p64 = (bs64_t)tc_crt_proc("_byteswap_uint64");
    TC_RUN(ushort); TC_RUN(ulong); TC_RUN(uint64); TC_RUN(double_swap_identity); TC_RUN(ushort_param_truncation);
    return tc_end();
}
