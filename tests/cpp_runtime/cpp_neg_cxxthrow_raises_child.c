/* TC

 * id:    cpp_neg_cxxthrow_raises_child
 * area:  cpp_runtime
 * kind:  negative
 * funcs: _CxxThrowException
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * conf:  observed
 * ref:   SEH exception code 0xE06D7363 ('msc' | 0xE0000000), widely documented in KB articles and SEH references
 * note:  The child installs a vectored handler, throws with a zeroed _ThrowInfo and encodes each observed fact as a bit of its exit
 *        code (0x80 = handler ran). ucrtbase excluded: _CxxThrowException lives in vcruntime140, not ucrtbase.
 *        Bits: 1 code, 2 magic 0x19930520, 4 object ptr, 8 throwinfo ptr, 16 NumberParameters==4 (x64), 32 valid image base, 64 noncontinuable.
 */
#include "tc.h"
#include <errno.h>
extern void __cdecl _CxxThrowException(void *, void *);
static int obj;
static unsigned char throwinfo[64];   /* zeroed */

static LONG CALLBACK veh(PEXCEPTION_POINTERS ep)
{
    PEXCEPTION_RECORD r = ep->ExceptionRecord;
    DWORD bits = 0x80;
    if (r->ExceptionCode != 0xE06D7363u) return EXCEPTION_CONTINUE_SEARCH;
    bits |= 1;
    if (r->ExceptionInformation[0] == 0x19930520u) bits |= 2;
    if (r->ExceptionInformation[1] == (ULONG_PTR)&obj) bits |= 4;
    if (r->ExceptionInformation[2] == (ULONG_PTR)throwinfo) bits |= 8;
#ifdef _WIN64
    if (r->NumberParameters == 4) {
        MEMORY_BASIC_INFORMATION mbi;
        bits |= 16;
        if (r->ExceptionInformation[3] &&
            VirtualQuery((void *)r->ExceptionInformation[3], &mbi, sizeof mbi) &&
            (ULONG_PTR)mbi.AllocationBase == r->ExceptionInformation[3]) bits |= 32;
    }
#endif
    if (r->ExceptionFlags & 1) bits |= 64;
    ExitProcess(bits);
    return EXCEPTION_CONTINUE_SEARCH;
}
static DWORD code; static int ran;
static void run_child(void) { ran = tc_spawn_self("throw", &code, 20000); }

TC_CASE(raises_msc_code)  { run_child(); CHECK(ran); CHECK(code & 0x80); CHECK(code & 1); }
TC_CASE(magic)            { run_child(); CHECK(ran); CHECK(code & 0x80); CHECK(code & 2); }
TC_CASE(object_ptr)       { run_child(); CHECK(ran); CHECK(code & 0x80); CHECK(code & 4); }
TC_CASE(throwinfo_ptr)    { run_child(); CHECK(ran); CHECK(code & 0x80); CHECK(code & 8); }
TC_CASE(x64_imagebase)
{
    if (!TC_X64) TC_SKIP("x64 only");
    run_child(); CHECK(ran); CHECK(code & 0x80); CHECK(code & 16); CHECK(code & 32);
}
TC_CASE(noncontinuable)   { run_child(); CHECK(ran); CHECK(code & 0x80); CHECK(code & 64); }

int main(void)
{
    tc_begin();
    if (tc_child_name() != NULL) {
        AddVectoredExceptionHandler(1, veh);
        _CxxThrowException(&obj, throwinfo);
        return 77;
    }
    TC_RUN(raises_msc_code); TC_RUN(magic); TC_RUN(object_ptr); TC_RUN(throwinfo_ptr); TC_RUN(x64_imagebase); TC_RUN(noncontinuable);
    return tc_end();
}
