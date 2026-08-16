/*
 * Shim for the frozen libsdm-color.so blob's sdm::Debug::GetProperty(const
 * char*, int*) call. Upstream's DebugHandler refactor dropped that overload
 * as a directly-callable static-class symbol, routing int-typed property
 * reads through virtual dispatch on DebugHandler instead (Debug::Get()->
 * GetProperty(name, value)). The blob still imports the old mangled symbol
 * directly, so bionic's RTLD_NOW dlopen() rejects it outright.
 *
 * Forces this free function onto the blob's expected mangled name via the
 * asm() linker-name extension rather than reintroducing the old symbol into
 * the (unforked) display HAL's Debug class declaration.
 */

#include <utils/debug.h>

extern "C" bool sdm_debug_get_property_int_shim(const char *property_name, int *value)
    asm("_ZN3sdm5Debug11GetPropertyEPKcPi");

bool sdm_debug_get_property_int_shim(const char *property_name, int *value) {
  sdm::DebugHandler *handler = sdm::Debug::Get();
  if (!handler) {
    return false;
  }
  return handler->GetProperty(property_name, value) == sdm::kErrorNone;
}
