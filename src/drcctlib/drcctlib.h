#ifndef _DRCCTLIB_H_
#define _DRCCTLIB_H_

#include "dr_api.h"
#include "drcctlib_global_share.h"
#include "drcctlib_filter_func_list.h"

#include <vector>

enum {
    INSTR_STATE_CLIENT_INTEREST = 0x01,
    INSTR_STATE_CALL_DIRECT = 0x02,
    INSTR_STATE_CALL_IN_DIRECT = 0x04,
    INSTR_STATE_RETURN = 0x08,
    INSTR_STATE_MEM_ACCESS = 0X10,
    INSTR_STATE_THREAD_ROOT_VIRTUAL = 0x20,
#ifdef ARM32_CCTLIB
    INSTR_STATE_BB_START_NOP = 0X40
#endif
};

enum {
    DRCCTLIB_DEFAULT = 0x00,
    DRCCTLIB_CACHE_MODE = 0x01,
    DRCCTLIB_COLLECT_DATA_CENTRIC_MESSAGE = 0x02,
    DRCCTLIB_CACHE_MEMEORY_ACCESS_ADDR = 0x04,
    DRCCTLIB_SAVE_HPCTOOLKIT_FILE = 0x08,
    DRCCTLIB_CACHE_EXCEPTION = 0x10
};

// stack config
typedef struct _thread_stack_config_t {
    int thread_id;
    void *stack_base;
    void *stack_end;
} thread_stack_config_t;

typedef struct _instr_instrument_msg_t {
    instrlist_t *bb;
    instr_t *instr;
    bool interest_start;
    int32_t slot;
    int32_t state;
    struct _instr_instrument_msg_t *next;
} instr_instrument_msg_t;

typedef struct _context_t {
    char func_name[MAXIMUM_SYMNAME];
    char file_path[MAXIMUM_PATH];
    char code_asm[DISASM_CACHE_SIZE];
    context_handle_t ctxt_hndl;
    int line_no;
    app_pc ip;
    struct _context_t *pre_ctxt;
} context_t;

typedef struct _mem_ref_msg_t {
    int64_t index;
    int64_t slot;
    app_pc addr;
} mem_ref_msg_t;

enum { UNKNOWN_OBJECT, STACK_OBJECT, DYNAMIC_OBJECT, STATIC_OBJECT };

// The handle representing a data object
typedef struct _data_handle_t {
    uint8_t object_type;
    union {
        context_handle_t path_handle;
        int32_t sym_name;
    };
} data_handle_t;

DR_EXPORT
bool
drcctlib_init_ex(bool (*filter)(instr_t *), file_t file,
                 void (*func1)(void *, instr_instrument_msg_t *, void *), void *data1,
                 void (*func2)(void *, int32_t, int32_t, void *), void *data2,
                 void (*func3)(void *, context_handle_t, int32_t, int32_t,
                               mem_ref_msg_t *, void **),
                 void *data3,
                 void (*func4)(void *, context_handle_t, int32_t, mem_ref_msg_t *,
                               void **),
                 void *data4, char flag);

DR_EXPORT
void
drcctlib_exit(void);

DR_EXPORT
void
drcctlib_register_instr_filter(bool (*filter)(instr_t *));

DR_EXPORT
void
drcctlib_register_client_cb(
    void (*func_instr_analysis)(void *, instr_instrument_msg_t *, void *),
    void *analysis_data, void (*func_insert_bb_start)(void *, int32_t, int32_t, void *),
    void *insert_data,
    void (*func_insert_bb_post)(void *, context_handle_t, int32_t, int32_t,
                                mem_ref_msg_t *, void **),
    void *insert_bb_data,
    void (*func_insert_ins_post)(void *, context_handle_t, int32_t, mem_ref_msg_t *,
                                 void **),
    void *insert_ins_data);

DR_EXPORT
void
drcctlib_config_log_file(file_t file);

DR_EXPORT
file_t
drcctlib_get_log_file();

DR_EXPORT
int
drcctlib_get_per_thread_data_id();

DR_EXPORT
void
drcctlib_print_ctxt_hndl_msg(file_t file, context_handle_t ctxt_hndl, bool print_asm,
                             bool print_file_path);

DR_EXPORT
void
drcctlib_print_full_cct(file_t file, context_handle_t ctxt_hndl, bool print_asm,
                        bool print_file_path, int max_depth);

DR_EXPORT
context_t *
drcctlib_get_full_cct(context_handle_t ctxt_hndl, int max_depth);

DR_EXPORT
context_handle_t
drcctlib_get_context_handle(int32_t slot);

DR_EXPORT
context_handle_t
drcctlib_get_bb_start_context_handle();

DR_EXPORT
void
drcctlib_get_context_handle_in_reg(void *drcontext, instrlist_t *ilist, instr_t *where,
                                   int32_t slot, reg_id_t store_reg, reg_id_t addr_reg);

// new cache api
DR_EXPORT
context_handle_t
drcctlib_get_context_handle_cache(void *drcontext, int32_t slot);

DR_EXPORT
context_handle_t
drcctlib_get_caller_handle(context_handle_t ctxt_hndl);

DR_EXPORT
context_handle_t
drcctlib_get_global_context_handle_num();

DR_EXPORT
bool
drcctlib_ctxt_hndl_is_valid(context_handle_t ctxt_hndl);

DR_EXPORT
app_pc
drcctlib_get_pc(context_handle_t ctxt_hndl);

DR_EXPORT
int32_t
drcctlib_get_state(context_handle_t ctxt_hndl);

DR_EXPORT
bool
have_same_caller_prefix(context_handle_t ctxt_hndl1, context_handle_t ctxt_hndl2);

DR_EXPORT
bool
has_same_call_path(context_handle_t ctxt_hndl1, context_handle_t ctxt_hndl2);

DR_EXPORT
thread_stack_config_t
drcctlib_get_thread_stack_config(void *drcontext);

DR_EXPORT
data_handle_t
drcctlib_get_data_hndl_ignore_stack_data(void *drcontext, void *address);

DR_EXPORT
data_handle_t
drcctlib_get_data_hndl_runtime(void *drcontext, void *address);

DR_EXPORT
char *
drcctlib_get_str_from_strpool(int index);

typedef struct _HPCRunCCT_t {
    std::vector<context_handle_t> ctxt_hndl_list;
    std::vector<uint64_t> metric_list;
} HPCRunCCT_t;

DR_EXPORT
void
init_hpcrun_format(const char *app_name, bool metric_cct);

DR_EXPORT
int
hpcrun_create_metric(const char *name);

DR_EXPORT
int
write_thread_all_cct_hpcrun_format(void *drcontext);

DR_EXPORT
int
build_thread_custom_cct_hpurun_format(std::vector<HPCRunCCT_t *> &run_cct_list,
                                      void *drcontext);

DR_EXPORT
int
write_thread_custom_cct_hpurun_format(void *drcontext);

DR_EXPORT
int
build_progress_custom_cct_hpurun_format(std::vector<HPCRunCCT_t *> &run_cct_list);

DR_EXPORT
int
write_progress_custom_cct_hpurun_format();

#endif // _DRCCTLIB_H_
