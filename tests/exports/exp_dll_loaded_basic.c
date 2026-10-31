/* TC
 * id:    exp_dll_loaded_basic
 * area:  exports
 * funcs: strlen malloc free
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   Win32 GetModuleHandle/GetProcAddress; MS docs strlen, malloc, free
 * note:  Sanity control for all other exp_ tests.
 */
#include "tc.h"

#include <stdlib.h>
#include <string.h>

typedef size_t (__cdecl *strlen_t)(const char *);
typedef void *(__cdecl *malloc_t)(size_t);
typedef void (__cdecl *free_t)(void *);

TC_CASE(module_handle_nonnull) { CHECK(GetModuleHandleA(TC_CRT_DLL) != NULL); }
TC_CASE(strlen_resolves_nonnull) { CHECK(tc_crt_proc("strlen") != NULL); }
TC_CASE(malloc_free_resolve_nonnull) { CHECK(tc_crt_proc("malloc") != NULL); CHECK(tc_crt_proc("free") != NULL); }
TC_CASE(getprocaddress_matches_import_behaviour)
{
    strlen_t p_strlen = (strlen_t)tc_crt_proc("strlen");
    malloc_t p_malloc = (malloc_t)tc_crt_proc("malloc");
    free_t p_free = (free_t)tc_crt_proc("free");
    static const char s[] = "abcde";
    volatile const char *vs = s;
    void *m;
    if (!p_strlen || !p_malloc || !p_free) TC_SKIP("exports missing");
    CHECK_EQ_U(p_strlen((const char *)vs), strlen((const char *)vs));
    CHECK_EQ_U(p_strlen((const char *)vs), 5);
    m = p_malloc(16);
    CHECK(m != NULL);
    if (m) { ((char *)m)[0] = 1; free(m); }  /* freed by the import-bound free: same heap */
    m = malloc(16);
    CHECK(m != NULL);
    if (m) p_free(m);
}

int main(void)
{
    volatile size_t keep = strlen("x");
    tc_begin(); (void)keep;
    TC_RUN(module_handle_nonnull); TC_RUN(strlen_resolves_nonnull);
    TC_RUN(malloc_free_resolve_nonnull); TC_RUN(getprocaddress_matches_import_behaviour);
    return tc_end();
}
