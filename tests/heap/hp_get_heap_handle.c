/* TC
 * id:    hp_get_heap_handle
 * area:  heap
 * funcs: _get_heap_handle malloc free
 * crt:   all
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs _get_heap_handle (returns the Win32 heap handle used by the CRT)
 * note:  Only that a non-zero handle is returned and that HeapValidate accepts the whole heap. Whether malloc
 *        blocks live in that heap is probed in hp_probe_get_heap_handle_block.
 */
#include "tc.h"
#include <malloc.h>

TC_CASE(nonnull) { CHECK(_get_heap_handle() != 0); }
TC_CASE(validate_heap) { HANDLE h = (HANDLE)_get_heap_handle(); if (!h) return; CHECK(HeapValidate(h, 0, NULL)); }
int main(void) { tc_begin(); TC_RUN(nonnull); TC_RUN(validate_heap); return tc_end(); }
