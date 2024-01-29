#include <link.h>

#include <absl/strings/match.h>

#include "dlfcn_ex.h"
#include "helpers.h"


static void *libc_base = nullptr;


void resolve_libc() {
    if (libc_base) return;

    auto callback = [](dl_phdr_info *info, size_t size, void *data) -> int {
        if (absl::EndsWith(info->dlpi_name, "/libc.so")) {
            libc_base = (void *) info->dlpi_addr;
            LOGD("found libc_base: %p", libc_base);
            return 1;
        }
        return 0;
    };

    dl_iterate_phdr(callback,nullptr);
}


void *dlopen_ex(const char *filename, int flag) {
    resolve_libc();

    static void *(*__loader_dlopen)(const char* filename, int flags, const void* caller_addr) = nullptr;

    if (__loader_dlopen == nullptr) {
        void *libdl = REQUIRE_NONNULL(
            dlopen("libdl.so", RTLD_LAZY),
            "dlopen failed: %s", dlerror()
        );

        __loader_dlopen = (decltype(__loader_dlopen)) REQUIRE_NONNULL(
            dlsym(libdl, "__loader_dlopen"),
            "dlsym failed: %s", dlerror()
        );
    }

    return __loader_dlopen(filename, flag, libc_base);
}
