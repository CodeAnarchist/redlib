#pragma once

#include "types.h"
#include "graphic_types.h"
#include "keyboard_input.h"
#include "mouse_input.h"
#include "string/string.h"
#include "net/network_types.h"
#include "ui/draw/draw.h"
#include "files/fs.h"
#include "net/socket_types.h"
#include "files/system_module.h"
#include "signals/signal_types.h"

#ifdef __cplusplus
extern "C" {
#endif
enum {
    EXEC_MODE_DEFAULT = 0,
    EXEC_MODE_KEEP_FOCUS = 1,
};

extern void printl(const char *str);
int print(const char *fmt, ...);
#ifndef CROSS
void put(const char *fmt, ...);
#else
#define put(...) printf(__VA_ARGS__)
#endif

extern void serial_transmit(u8);

void* malloc(size_t size);//NOTE: malloc can return a faulty address for allocations smaller than 0x1000. Use zalloc/allocate until this is fixed, or allocate full pages (0x1000) if you wish to manage memory manually
void free_sized(void *ptr, size_t size);//NOTE: this function should be used in conjunction with malloc. For zalloc, use release(void*) instead

extern void* page_alloc(size_t size);
extern void page_free(void* ptr);

extern bool read_key(keypress *kp);
extern bool read_event(kbd_event *event);
extern void get_mouse_status(mouse_data *in);

extern void msleep(uint64_t time);
extern __attribute__((noreturn)) void halt(int32_t exit_code);
extern int32_t exec(const char* prog_name, int argc, const char* argv[], u32 mode);
extern int32_t kill_process(uint16_t pid);

extern void request_draw_ctx(draw_ctx*);
static inline void request_app_ctx(draw_ctx* ctx){ request_draw_ctx(ctx); }
extern void begin_drawing(draw_ctx *);
extern void commit_draw_ctx(draw_ctx*);
extern void resize_draw_ctx(draw_ctx*, u32 width, u32 height);
extern void destroy_draw_ctx(draw_ctx *ctx);

extern bool should_close_ctx();

extern uint32_t gpu_char_size(uint32_t scale);

extern uint64_t get_time();

extern socket_handle_t socket_create(protocol_t protocol, const SocketOptions* extra);
extern int32_t socket_bind(socket_handle_t handle, const SockBindSpec* spec, uint16_t port);
extern int32_t socket_connect(socket_handle_t handle, const net_l4_endpoint* dst);
extern int32_t socket_listen(socket_handle_t handle, int32_t backlog);
extern socket_handle_t socket_accept(socket_handle_t handle);
extern int64_t socket_send(socket_handle_t handle, void *packet, size_t size);
extern int64_t socket_send_to(socket_handle_t handle, const net_l4_endpoint* dst, void *packet, size_t size);
extern int64_t socket_receive(socket_handle_t handle, void *packet, size_t size, net_l4_endpoint* out_src);
extern int32_t socket_close(socket_handle_t handle);
extern int32_t socket_set_option(socket_handle_t handle, int32_t opt, const void* value, uint32_t len);
extern int32_t socket_get_option(socket_handle_t handle, int32_t opt, void* value, uint32_t* len);

extern FS_RESULT openf(const char* path, file* descriptor);
extern size_t readf(file *descriptor, char* buf, size_t size);
extern size_t writef(file *descriptor, const char* buf, size_t size);
extern size_t sreadf(const char* path, void* buf, size_t size);
extern size_t swritef(const char* path, const void* buf, size_t size, bool append);
extern void closef(file *descriptor);
extern bool statf(const char *path, fs_stat *out_stat);
extern bool truncatef(file*, size_t);
// extern bool load_fsmodule(system_module *mod);
// extern bool unload_fsmodule();

extern bool send_signal(signal_types type, u16 proc_id);
extern bool handle_signal(signal_types type, signal_handler handler);

void seek(file *descriptor, int64_t offset, SEEK_TYPE type);
void* realloc_sized(void* old_ptr, size_t old_size, size_t new_size);

void *calloc(size_t nitems, size_t size);

size_t dir_list(const char *path, void *buf, size_t size, u64 *offset);

int system(const char *command);
int system_focus(const char *command, u32 focus_mode);

void in_case_of_js_break_glass();

void register_behavior();

static inline void yield(){
    msleep(0);
}

#ifdef __cplusplus
}
#endif
