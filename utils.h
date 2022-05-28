#pragma once
#include <Windows.h>
#include <utility>

template <typename T>
constexpr T GetVFunc(void *pClass, size_t index) noexcept {
    if (!pClass) {
        return nullptr;
    }
    return (*(static_cast<T**>(pClass)))[index];
}

template<unsigned int vtable_index, typename T, typename... Args>
inline auto InvokeVFuncThisCall(T *obj, Args&&... args) {
    using Fn = T(__thiscall*)(void*, decltype(args)...);
    return GetVFunc<Fn>(obj, vtable_index)(obj, std::forward<Args>(args)...);
}

template <typename T>
static T* GetInterface(const char* name, HMODULE hModule) {
    if (!hModule) {
        return nullptr;
    }
    using CreateInterfaceFn = T*(*)(const char *pName, int *pReturnCode);
    auto createInterface = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(hModule, "CreateInterface"));
    return createInterface(name, nullptr);
}

