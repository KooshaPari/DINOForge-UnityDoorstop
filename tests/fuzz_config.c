/*
 * Fuzz test harness for DINOForge-UnityDoorstop config parser.
 *
 * Feeds arbitrary byte sequences into parse_target_assembly_string()
 * and init_config_defaults() to discover crashes, undefined behaviour,
 * and memory-safety issues.
 *
 * Build (standalone, no fuzzer):
 *   gcc -std=c11 -I. -I./src tests/fuzz_config.c \
 *       src/config/common.c src/nix/config.c src/nix/util.c \
 *       -o tests/fuzz_config
 *
 * Build with libFuzzer (requires clang):
 *   clang -std=c11 -fsanitize=fuzzer,address,undefined -I. -I./src \
 *       tests/fuzz_config.c src/config/common.c src/nix/config.c \
 *       src/nix/util.c -o tests/fuzz_config
 *
 * Run:
 *   ./tests/fuzz_config          # standalone: reads stdin
 *   ./tests/fuzz_config corpus/  # libFuzzer mode
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config/config.h"
#include "crt.h"

extern Config config;

/* ------------------------------------------------------------------ */
/*  fuzzer entry point (libFuzzer)                                     */
/* ------------------------------------------------------------------ */
#ifdef __AFL_HAVE_MANUAL_CONTROL
#include "alloc-inl.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput(const unsigned char *data, size_t size) {
    /* Require at least 1 byte and cap to a reasonable length. */
    if (size < 1 || size > 4096)
        return 0;

    /* 1. Fuzz parse_target_assembly_string (mutates a copy). */
    {
        char *buf = (char *)malloc(size + 1);
        if (!buf)
            return 0;
        memcpy(buf, data, size);
        buf[size] = '\0';

        init_config_defaults();
        parse_target_assembly_string(buf);
        /* If we survived, clean up. */
        free(config.target_assemblies);
        config.target_assemblies = NULL;
        config.num_assemblies = 0;
        free(buf);
    }

    /* 2. Fuzz load_config (reads environment variables).
     *    set a handful of DOORSTOP_* env vars to exercise the
     *    configuration path with fuzz-controlled values. */
    {
        char env_enabled[]     = "DOORSTOP_ENABLED";
        char env_target[]      = "DOORSTOP_TARGET_ASSEMBLY";
        char env_boot[]        = "DOORSTOP_BOOT_CONFIG_OVERRIDE";
        char env_mono_debug[]  = "DOORSTOP_MONO_DEBUG_ENABLED";
        char env_mono_addr[]   = "DOORSTOP_MONO_DEBUG_ADDRESS";

        /* Build a null-terminated copy for the target assembly env var. */
        char *env_buf = (char *)malloc(size + 1);
        if (!env_buf)
            return 0;
        memcpy(env_buf, data, size);
        env_buf[size] = '\0';

        setenv(env_enabled,     "1",           1);
        setenv(env_target,      env_buf,       1);
        setenv(env_boot,        "",            1);
        setenv(env_mono_debug,  "0",           1);
        setenv(env_mono_addr,   "127.0.0.1:1", 1);

        init_config_defaults();
        load_config();

        free(config.target_assemblies);
        config.target_assemblies = NULL;
        config.num_assemblies = 0;

        unsetenv(env_enabled);
        unsetenv(env_target);
        unsetenv(env_boot);
        unsetenv(env_mono_debug);
        unsetenv(env_mono_addr);

        free(env_buf);
    }

    /* 3. Fuzz clean_up_config for idempotency / crash safety. */
    {
        init_config_defaults();
        clean_up_config();
        clean_up_config();
    }

    return 0;
}

#ifdef __cplusplus
}
#endif

/* ------------------------------------------------------------------ */
/*  standalone driver (reads stdin)                                    */
/* ------------------------------------------------------------------ */
int main(int argc, char **argv) {
#ifdef __AFL_HAVE_MANUAL_CONTROL
    __AFL_INIT();
    while (__AFL_LOOP(10000)) {
        unsigned char buf[4096];
        int len = read(0, buf, sizeof(buf));
        if (len > 0)
            LLVMFuzzerTestOneInput(buf, (size_t)len);
    }
    return 0;
#else
    /* libFuzzer provides its own main; standalone reads from stdin. */
    if (argc > 1) {
        fprintf(stderr,
            "Usage: %s < input_file  (or link with libFuzzer)\n",
            argv[0]);
        return 1;
    }

    unsigned char buf[4096];
    size_t n = fread(buf, 1, sizeof(buf) - 1, stdin);
    if (n > 0) {
        return LLVMFuzzerTestOneInput(buf, n);
    }
    fprintf(stderr, "No input read from stdin.\n");
    return 1;
#endif
}
