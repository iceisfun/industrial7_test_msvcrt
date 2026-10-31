/* TC
 * id:    cpp_adjustpointer_probe
 * area:  cpp_runtime
 * funcs: __AdjustPointer
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MSVC EH implementation (ehdata.h PMD {mdisp, pdisp, vdisp})
 * note:  PROBE: internal helper __AdjustPointer(void *this, const PMD *) with a non-virtual PMD (pdisp < 0) returns
 *        (char*)this + mdisp. Semantics from MSVC EH sources, not from public docs.
 */
#include "tc.h"

typedef struct { int mdisp, pdisp, vdisp; } PMD_;
extern void *__cdecl __AdjustPointer(void *, const PMD_ *);

static char buf[64];
static char *volatile base = buf + 32;

TC_CASE(mdisp_only) { PMD_ d = { 16, -1, 0 }; CHECK_EQ_P(__AdjustPointer(base, &d), base + 16); }
TC_CASE(mdisp_negative) { PMD_ d = { -8, -1, 0 }; CHECK_EQ_P(__AdjustPointer(base, &d), base - 8); }
TC_CASE(mdisp_zero) { PMD_ d = { 0, -1, 0 }; CHECK_EQ_P(__AdjustPointer(base, &d), base); }

int main(void) { tc_begin(); TC_RUN(mdisp_only); TC_RUN(mdisp_negative); TC_RUN(mdisp_zero); return tc_end(); }
