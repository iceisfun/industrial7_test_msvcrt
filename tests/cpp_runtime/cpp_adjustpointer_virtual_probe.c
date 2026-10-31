/* TC
 * id:    cpp_adjustpointer_virtual_probe
 * area:  cpp_runtime
 * funcs: __AdjustPointer
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MSVC EH implementation (ehdata.h PMD {mdisp, pdisp, vdisp})
 * note:  PROBE: virtual-base PMD (pdisp >= 0): result = this + mdisp + pdisp + *(int*)(*(char**)(this + pdisp) + vdisp).
 */
#include "tc.h"

typedef struct { int mdisp, pdisp, vdisp; } PMD_;
extern void *__cdecl __AdjustPointer(void *, const PMD_ *);

static int vbtable[4];
static union { void *align; char c[64]; } obj;

static char *setup(void)
{
    char *p = obj.c;
    vbtable[0] = 0; vbtable[1] = 24; vbtable[2] = 0; vbtable[3] = 0;   /* entry at byte offset vdisp=4 is 24 */
    *(void **)(p + 8) = (char *)vbtable;                              /* vbptr at pdisp=8 */
    return p;
}

TC_CASE(vbase_adjust)
{
    PMD_ d = { 0, 8, 4 }; char *volatile p = setup();
    CHECK_EQ_P(__AdjustPointer(p, &d), p + 8 + 24);
}
TC_CASE(vbase_with_mdisp)
{
    PMD_ d = { 16, 8, 4 }; char *volatile p = setup();
    CHECK_EQ_P(__AdjustPointer(p, &d), p + 16 + 8 + 24);
}

int main(void) { tc_begin(); TC_RUN(vbase_adjust); TC_RUN(vbase_with_mdisp); return tc_end(); }
