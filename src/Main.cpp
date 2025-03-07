#include "Handler.hpp"
#include "Misc.hpp"
#include "Pool.hpp"
#include "Wrapper.hpp"
#include "target/PlatformTarget.hpp"

#include <TulipHook.hpp>

using namespace tulip::hook;

geode::Result<HandlerHandle> tulip::hook::createHandler(void* address, HandlerMetadata const& metadata) noexcept {
	return Pool::get().createHandler(address, metadata);
}

geode::Result<> tulip::hook::removeHandler(HandlerHandle const& handler) noexcept {
	return Pool::get().removeHandler(handler);
}

HookHandle tulip::hook::createHook(HandlerHandle const& handler, void* function, HookMetadata const& metadata) noexcept {
	return Pool::get().getHandler(handler).createHook(function, metadata);
}

void tulip::hook::removeHook(HandlerHandle const& handler, HookHandle const& hook) noexcept {
	return Pool::get().getHandler(handler).removeHook(hook);
}

void tulip::hook::updateHookMetadata(
	HandlerHandle const& handler, HookHandle const& hook, HookMetadata const& metadata
) noexcept {
	return Pool::get().getHandler(handler).updateHookMetadata(hook, metadata);
}

geode::Result<> tulip::hook::writeMemory(void* destination, void const* source, size_t size) noexcept {
	return Target::get().writeMemory(destination, source, size);
}

geode::Result<void*> tulip::hook::followJumps(void* address) noexcept {
	return Misc::followJumps(address);
}

geode::Result<void*> tulip::hook::createWrapper(void* address, WrapperMetadata const& metadata) noexcept {
	return Wrapper::get().createWrapper(address, metadata);
}

geode::Result<void*> tulip::hook::createReverseWrapper(void* address, WrapperMetadata const& metadata) noexcept {
	return Wrapper::get().createReverseWrapper(address, metadata);
}

std::shared_ptr<CallingConvention> tulip::hook::createConvention(TulipConvention convention) noexcept {
	switch (convention) {
#if defined(TULIP_HOOK_WINDOWS) && defined(TULIP_HOOK_X86)
		case TulipConvention::Cdecl: return CdeclConvention::create();
		case TulipConvention::Thiscall: return ThiscallConvention::create();
		case TulipConvention::Fastcall: return FastcallConvention::create();
		case TulipConvention::Optcall: return OptcallConvention::create();
		case TulipConvention::Membercall: return MembercallConvention::create();
		case TulipConvention::Stdcall: return StdcallConvention::create();
#endif
#if defined(TULIP_HOOK_WINDOWS) && defined(TULIP_HOOK_X64)
		case TulipConvention::Thiscall: return ThiscallConvention::create();
#endif
		case TulipConvention::Default:
		default:
#if defined(TULIP_HOOK_MACOS) && defined(TULIP_HOOK_X64)
			return SystemVConvention::create();
#elif defined(TULIP_HOOK_WINDOWS) && defined(TULIP_HOOK_X64)
			return Windows64Convention::create();
#else
			return DefaultConvention::create();
#endif
	}
}
