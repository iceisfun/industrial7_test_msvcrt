/* TC
 * id:    exp_neg_getprocaddress_bogus_name
 * area:  exports
 * funcs: 
 * procs: strlen malloc memcpy
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   Win32 GetProcAddress (returns NULL; GetLastError ERROR_PROC_NOT_FOUND=127 is observed behaviour)
 * note:  Export names are case-sensitive; x64 has no leading-underscore decoration so '_strlen' and '_malloc' are absent; C++ mangled names for C functions are absent.
 */
#include "tc.h"


static FARPROC try_name(const char *n) { SetLastError(0); return tc_crt_proc(n); }

TC_CASE(control_real_names_resolve)
{
    CHECK(try_name("strlen") != NULL);
    CHECK(try_name("malloc") != NULL);
    CHECK(try_name("memcpy") != NULL);
}
TC_CASE(bogus_name_null_and_error_127)
{
    FARPROC p = try_name("tc_no_such_export_xyz");
    CHECK(p == NULL);
    CHECK_EQ_I(GetLastError(), ERROR_PROC_NOT_FOUND);
}
TC_CASE(empty_like_trailing_space_name)
{
    CHECK(try_name("strlen ") == NULL);
    CHECK(try_name(" strlen") == NULL);
    CHECK(try_name("mem cpy") == NULL);
}
TC_CASE(uppercase_variant_case_sensitive)
{
    CHECK(try_name("STRLEN") == NULL);
    CHECK(try_name("Malloc") == NULL);
    CHECK(try_name("MEMCPY") == NULL);
}
TC_CASE(underscore_prefixed_strlen_absent)
{
    CHECK(try_name("_strlen") == NULL);
    CHECK(try_name("_malloc") == NULL);
}
TC_CASE(cpp_mangled_absent)
{
    CHECK(try_name("?strlen@@YA_KPEBD@Z") == NULL);
    CHECK(try_name("?malloc@@YAPEAX_K@Z") == NULL);
}
int main(void)
{
    tc_begin();
    TC_RUN(control_real_names_resolve); TC_RUN(bogus_name_null_and_error_127); TC_RUN(empty_like_trailing_space_name);
    TC_RUN(uppercase_variant_case_sensitive); TC_RUN(underscore_prefixed_strlen_absent); TC_RUN(cpp_mangled_absent);
    return tc_end();
}
