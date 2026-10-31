/* TC
 * id:    sec_exports_present_procs
 * area:  secure_crt
 * funcs:
 * procs: strcpy_s strcat_s strncpy_s strncat_s memcpy_s memmove_s wcscpy_s wcscat_s wcsncpy_s wcsncat_s _itoa_s strtok_s fopen_s _splitpath_s _makepath_s getenv_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs requirements tables for the _s functions (VS2005+)
 */
#include "tc.h"

#define EX(n) CHECK(tc_crt_proc(n) != NULL)
TC_CASE(string_exports) { EX("strcpy_s"); EX("strcat_s"); EX("strncpy_s"); EX("strncat_s"); EX("strtok_s"); }
TC_CASE(memory_exports) { EX("memcpy_s"); EX("memmove_s"); }
TC_CASE(wide_exports) { EX("wcscpy_s"); EX("wcscat_s"); EX("wcsncpy_s"); EX("wcsncat_s"); }
TC_CASE(conversion_and_path_exports) { EX("_itoa_s"); EX("_splitpath_s"); EX("_makepath_s"); }
TC_CASE(file_and_env_exports) { EX("fopen_s"); EX("getenv_s"); }
int main(void)
{
    tc_begin(); TC_RUN(string_exports); TC_RUN(memory_exports); TC_RUN(wide_exports);
    TC_RUN(conversion_and_path_exports); TC_RUN(file_and_env_exports); return tc_end();
}
