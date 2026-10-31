/* TC
 * id:    exp_neg_nosuch_names
 * area:  exports
 * kind:  negative
 * funcs: strlen
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs GetProcAddress (returns NULL for an unknown name; export names are case sensitive)
 * note:  GetLastError()==ERROR_PROC_NOT_FOUND is not asserted here (undocumented detail; see exp_neg_probe_lasterror).
 */
#include "tc.h"

TC_CASE(nonexistent_name)
{
    FARPROC p;
    p = tc_crt_proc("tc_no_such_function_xyz");
    CHECK(p == NULL);
}
TC_CASE(case_sensitive)
{
    CHECK(tc_crt_proc("strlen") != NULL);   /* control */
    CHECK(tc_crt_proc("STRLEN") == NULL);
    CHECK(tc_crt_proc("Strlen") == NULL);
}
TC_CASE(decorations_and_spaces)
{
    CHECK(tc_crt_proc("strlen ") == NULL);
    CHECK(tc_crt_proc(" strlen") == NULL);
    CHECK(tc_crt_proc("_strlen") == NULL);
    CHECK(tc_crt_proc("_strlen@4") == NULL);
    CHECK(tc_crt_proc("strlen\\n") == NULL);
}
TC_CASE(wrong_dll_contents)
{
    CHECK(tc_crt_proc("CreateFileA") == NULL);   /* kernel32 export is not a CRT export */
    CHECK(tc_crt_proc("main") == NULL);
}
int main(void) { tc_begin(); TC_RUN(nonexistent_name); TC_RUN(case_sensitive); TC_RUN(decorations_and_spaces); TC_RUN(wrong_dll_contents); return tc_end(); }
