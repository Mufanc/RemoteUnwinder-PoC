#pragma once

#include <map>
#include <string>

#include <absl/strings/str_cat.h>

#include "dlfcn_ex.h"
#include "helpers.h"

#ifdef __LP64__
    #define LIBRARY_PATH "/system/lib64"
#else
    #define LIBRARY_PATH "/system/lib"
#endif


class DynamicLibrary {
private:
    void *handle = nullptr;
    std::map<std::string, void *> symbol_cache;

public:
    explicit DynamicLibrary(std::string_view libname) {
        auto filename = absl::StrCat(LIBRARY_PATH, "/lib", libname, ".so");

        LOGD("dlopen: %s", filename.c_str());

        handle = REQUIRE_NONNULL(
            dlopen_ex(filename.c_str(), RTLD_LAZY),
            "dlopen failed: %s", dlerror()
        );
    }

    template<typename F, typename... Args>
    typename std::invoke_result<F, Args...>::type call(const std::string &symbol, Args... args) {
        auto ptr = symbol_cache.find(symbol);
        void *func;

        if (ptr == symbol_cache.end()) {
            LOGD("dlsym: %s", symbol.c_str());

            func = REQUIRE_NONNULL(
                dlsym(handle, symbol.c_str()),
                "dlsym failed: %s", dlerror()
            );

            symbol_cache[symbol] = func;
        } else {
            func = ptr->second;
        }

        return ((F) func)(args...);
    }
};
