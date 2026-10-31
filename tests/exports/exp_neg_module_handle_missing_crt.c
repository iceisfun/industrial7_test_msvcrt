/* TC
 * id:    exp_neg_module_handle_missing_crt
 * area:  exports
 * kind:  negative
 * funcs:
 * procs: malloc
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs LoadLibrary, GetModuleHandle (NULL on failure; ERROR_MOD_NOT_FOUND for missing module)
 * note:  Names of CRTs that never existed or were never loaded must not resolve.
 */
#include "tc.h"

TC_CASE(loadlibrary_nonexistent)
{
    HMODULE m; SetLastError(0);
    m = LoadLibraryA("msvcr999.dll");
    CHECK(m == NULL);
    CHECK_EQ_I(GetLastError(), ERROR_MOD_NOT_FOUND);
    SetLastError(0);
    m = LoadLibraryA("ucrtbase_nonexistent.dll");
    CHECK(m == NULL);
    CHECK_EQ_I(GetLastError(), ERROR_MOD_NOT_FOUND);
}
TC_CASE(getmodulehandle_not_loaded)
{
    HMODULE m; SetLastError(0);
    m = GetModuleHandleA("msvcr70.dll");
    CHECK(m == NULL);
    CHECK_EQ_I(GetLastError(), ERROR_MOD_NOT_FOUND);
}
TC_CASE(getmodulehandle_empty_and_dot)
{
    HMODULE m;
    SetLastError(0);
    m = GetModuleHandleA("no_such_crt_tc.");
    CHECK(m == NULL);
    CHECK_EQ_I(GetLastError(), ERROR_MOD_NOT_FOUND);
    SetLastError(0);
    m = GetModuleHandleA("tc_no_such_dir\\" TC_CRT_DLL);
    CHECK(m == NULL);
}
TC_CASE(control)
{
    CHECK(GetModuleHandleA(TC_CRT_DLL) != NULL);
}
int main(void)
{
    tc_begin();
    TC_RUN(loadlibrary_nonexistent); TC_RUN(getmodulehandle_not_loaded);
    TC_RUN(getmodulehandle_empty_and_dot); TC_RUN(control);
    return tc_end();
}
