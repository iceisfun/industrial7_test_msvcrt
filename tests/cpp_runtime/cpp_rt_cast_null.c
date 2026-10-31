/* TC
 * id:    cpp_rt_cast_null
 * area:  cpp_runtime
 * funcs: __RTCastToVoid __RTDynamicCast
 * crt:   all
 * tier:  p2
 * conf:  inferred
 * ref:   C++ [expr.dynamic.cast]/4: a null pointer value yields the null pointer value of the target type
 * note:  Signature (void *inptr, LONG VfDelta, void *SrcType, void *TargetType, BOOL isReference); the type descriptor
 *        arguments are NULL because they are unused for a null input.
 */
#include "tc.h"

extern void *__cdecl __RTCastToVoid(void *);
extern void *__cdecl __RTDynamicCast(void *, LONG, void *, void *, BOOL);

static void *volatile v_null = NULL;
static volatile LONG v_delta = 0;

TC_CASE(casttovoid_null) { CHECK(__RTCastToVoid(v_null) == NULL); }
TC_CASE(dyncast_null_ptr) { CHECK(__RTDynamicCast(v_null, v_delta, NULL, NULL, FALSE) == NULL); }
TC_CASE(dyncast_null_after_other_args) { v_delta = 12; CHECK(__RTDynamicCast(v_null, v_delta, NULL, NULL, FALSE) == NULL); }

int main(void) { tc_begin(); TC_RUN(casttovoid_null); TC_RUN(dyncast_null_ptr); TC_RUN(dyncast_null_after_other_args); return tc_end(); }
