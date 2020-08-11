#include "common.h"

void* inject_trampoline_impl(lcd_trampoline_handle* handle, void* impl) {
	handle->hidden_args = impl;
	return handle->trampoline;
}

