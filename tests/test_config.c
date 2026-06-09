#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../src/config/config.h"
#include "../src/crt.h"

/* Minimal test harness for DINOForge-UnityDoorstop */
#define TEST(name) static void test_##name(void)
#define RUN_TEST(name) do { \
    printf("  Running test_" #name " ... "); \
    test_##name(); \
    printf("PASS\n"); \
} while(0)

TEST(config_defaults) {
    init_config_defaults();
    assert(config.enabled == FALSE);
    assert(config.ignore_disabled_env == FALSE);
    assert(config.redirect_output_log == FALSE);
    assert(config.mono_debug_enabled == FALSE);
    assert(config.mono_debug_suspend == FALSE);
    assert(config.num_assemblies == 0);
    assert(config.assembly_index == 0);
    assert(config.target_assemblies == NULL);
    assert(config.target_assembly == NULL);
    assert(config.boot_config_override == NULL);
    assert(config.mono_dll_search_path_override == NULL);
    assert(config.clr_corlib_dir == NULL);
    assert(config.clr_runtime_coreclr_path == NULL);
    assert(config.mono_debug_address == NULL);
    cleanup_config();
}

TEST(config_cleanup_idempotent) {
    init_config_defaults();
    cleanup_config();
    /* cleanup_config() should be safe to call twice */
    cleanup_config();
}

TEST(config_parse_target_assembly_simple) {
    init_config_defaults();
    char_t *assembly = strdup("TestAssembly.dll");
    parse_target_assembly_string(assembly);
    assert(config.num_assemblies == 1);
    assert(config.target_assemblies != NULL);
    assert(strcmp(config.target_assemblies[0], "TestAssembly.dll") == 0);
    cleanup_config();
    free(assembly);
}

TEST(config_parse_target_assembly_multiple) {
    init_config_defaults();
    char_t *assemblies = strdup("A.dll;B.dll;C.dll");
    parse_target_assembly_string(assemblies);
    assert(config.num_assemblies == 3);
    assert(config.target_assemblies != NULL);
    assert(strcmp(config.target_assemblies[0], "A.dll") == 0);
    assert(strcmp(config.target_assemblies[1], "B.dll") == 0);
    assert(strcmp(config.target_assemblies[2], "C.dll") == 0);
    cleanup_config();
    free(assemblies);
}

int main(void) {
    printf("DINOForge-UnityDoorstop C Test Suite\n");
    printf("=====================================\n\n");

    RUN_TEST(config_defaults);
    RUN_TEST(config_cleanup_idempotent);
    RUN_TEST(config_parse_target_assembly_simple);
    RUN_TEST(config_parse_target_assembly_multiple);

    printf("\nAll tests passed!\n");
    return 0;
}
