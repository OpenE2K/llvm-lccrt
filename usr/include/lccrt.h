#ifndef LCCRT_H
#define LCCRT_H
/**
 * lccrt.h - пользовательский интерфейс (динамической) компиляции.
 *
 * Copyright (c) 1992-2021 AO "MCST". All rights reserved.
 *
 * \defgroup lccrt_h
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define LCCRT_VERSION "0.0.8"

typedef struct lccrt_mem_r *lccrt_mem_ptr;
typedef struct lccrt_err_r *lccrt_err_ptr;
typedef struct lccrt_context_r *lccrt_context_ptr;
typedef struct lccrt_plugin_r *lccrt_plugin_ptr;
typedef struct lccrt_module_r *lccrt_module_ptr;
typedef struct lccrt_function_r *lccrt_function_ptr;
typedef struct lccrt_function_jit_info_r *lccrt_function_jit_info_ptr;
typedef struct lccrt_var_r *lccrt_var_ptr;
typedef struct lccrt_varinit_r *lccrt_varinit_ptr;
typedef struct lccrt_type_r *lccrt_type_ptr;
typedef struct lccrt_oper_r *lccrt_oper_ptr;
typedef struct lccrt_oper_iterator_r *lccrt_oper_iterator_ptr;
typedef struct lccrt_metadata_struct_descr_r *lccrt_metadata_struct_descr_ptr;
typedef struct lccrt_metadata_r *lccrt_metadata_ptr;
typedef struct lccrt_fs_r *lccrt_fs_ptr;

typedef struct lccrt_irr_jit_region_r *lccrt_irr_jit_region_ptr;
typedef struct lccrt_irr_execute_context_r *lccrt_irr_execute_context_ptr;
typedef struct lccrt_irr_code_r *lccrt_irr_code_ptr;
typedef struct lccrt_irr_node_r *lccrt_irr_node_ptr;
typedef struct lccrt_irr_oper_r *lccrt_irr_oper_ptr;
typedef struct lccrt_irr_oper_iter_r *lccrt_irr_oper_iter_ptr;
typedef struct lccrt_irr_arg_r *lccrt_irr_arg_ptr;
typedef struct lccrt_irr_reg_r *lccrt_irr_reg_ptr;
typedef struct lccrt_irr_label_r *lccrt_irr_label_ptr;
typedef struct lccrt_irr_reloc_r *lccrt_irr_reloc_ptr;
typedef struct lccrt_irr_reloc_unit_r *lccrt_irr_reloc_unit_ptr;
typedef lccrt_irr_node_ptr (* lccrt_irr_resolve_virt_addr_func_t)( void *, uint64_t);
typedef uint64_t lccrt_irr_hex_t;

#define LCCRT_PLUGIN_LIBRARY_NAME         lccrt_plugin_library_name
#define LCCRT_PLUGIN_LCCRT_VERSION        lccrt_plugin_lccrt_version
#define LCCRT_PLUGIN_TARGETS              lccrt_plugin_targets

#define LCCRT_ASM_FUNC_COMPILE            lccrt_asm_compile
#define LCCRT_ASM_FUNC_GETTOOL            lccrt_asm_gettool

#define LCCRT_IRROBJ_FUNC_NODE_COMPILE    lccrt_irrobj_node_compile
#define LCCRT_IRROBJ_FUNC_NODE_DECOMPILE  lccrt_irrobj_node_decompile
#define LCCRT_IRROBJ_FUNC_RELOC_PATCH     lccrt_irrobj_reloc_patch
#define LCCRT_IRROBJ_FUNC_CODE_EXEC       lccrt_irrobj_code_exec
#define LCCRT_IRROBJ_FUNC_GETCONF         lccrt_irrobj_getconf

typedef int (*lccrt_asm_write_t)( void *write_info, char *data, uint64_t length);

/**
 * Структура с настройками процесса компиляции.
 */
typedef struct
{
    const char *target; /* результирующая платформа */
    void *write_info; /* выходной поток */
    lccrt_asm_write_t write; /* функция записи в выходной поток */
    const char *out_type; /* тип выходной информации ("asm", "obj") */
    int opt_level; /* уровень оптимизации */
    int8_t is_pic; /* флаг PIC-режима */
    int8_t pie_level; /* только для PIC-режима: 0 - not pie, 1 - small pie, 2 - large pie */
    int8_t is_jit; /* включить режим компиляции */
    int8_t dbg_level; /* уровень отладочной информации в формате dwarf2 */
    int8_t function_sections; /* включить опцию -ffunction-sections */
    int8_t data_sections; /* включить опцию -fdata-sections */
    int8_t asm_verbose; /* включить опцию -fverbose-asm */
    const char *eh_personality; /* название personality-функции */
    const char *cpu_arch; /* архитектура процессора */
    const char *cflags; /* дополнительные опции компиляции, которые могут быть не доступны в виде
                           самостоятельных параметров */
} lccrt_asm_compile_config_t;

typedef int (*lccrt_asm_compile_t)( lccrt_module_ptr m, lccrt_asm_compile_config_t *cnf);
typedef const char *(*lccrt_asm_gettool_t)( const char *targ, const char *type, const char *name);

typedef lccrt_irr_code_ptr (*lccrt_irrobj_node_compile_t)( lccrt_irr_node_ptr n);
typedef int (*lccrt_irrobj_node_decompile_t)( lccrt_irr_node_ptr n);
typedef int (*lccrt_irrobj_reloc_patch_t)( lccrt_irr_reloc_ptr reloc, lccrt_irr_code_ptr code);
typedef int (*lccrt_irrobj_code_exec_t)( lccrt_irr_code_ptr code, lccrt_irr_execute_context_ptr ec);
typedef const char *(*lccrt_irrobj_getconf_t)( const char *targ, const char *name);

extern int LCCRT_ASM_FUNC_COMPILE( lccrt_module_ptr m, lccrt_asm_compile_config_t *cnf);
extern const char *LCCRT_ASM_FUNC_GETTOOL( const char *targ, const char *type, const char *name);

/**
 * Типы данных для функций выделения/освобождения памяти.
 */
typedef void * (*lccrt_alloc_t)( void *pool, uint64_t size);
typedef void * (*lccrt_realloc_t)( void *pool, void *ptr, uint64_t size);
typedef void (*lccrt_free_t)( void *pool, void *ptr);

/**
 * Данные менеджера (аллокатора) памяти.
 */
typedef struct lccrt_mem_r
{
    void *pool; /*!< собственные данные менеджера (первый параметр методов) */
    lccrt_alloc_t palloc; /*!< функция выделения памяти */
    lccrt_realloc_t prealloc; /*!< функция увелечения размера памяти */
    lccrt_free_t pfree; /*!< функция освобождения памяти */
} lccrt_mem_t;

extern void lccrt_mem_init( lccrt_mem_t *mem, void *pool,
                            lccrt_alloc_t alloc_func, lccrt_realloc_t realloc_func,
                            lccrt_free_t free_func);

/**
 * Тип данных для выдачи события о возникновении ошибки.
 */
typedef void (*lccrt_errmsg_t)( void *data, int errid, const char *fmt, ...);

/**
 * Данные для менеджера вывода сообщений об ошибках и сообщений с предупреждениями.
 */
typedef struct lccrt_err_r
{
    void *data; /*!< собственные данные менеджера (первый параметр методов) */
    lccrt_errmsg_t error_func; /*!< функция вывода сообщения об ошибке */
    lccrt_errmsg_t warning_func; /*!< функция вывода сообщения с предупреждением */
} lccrt_err_t;

extern void lccrt_err_init( lccrt_err_t *mem, void *data,
                            lccrt_errmsg_t error_func, lccrt_errmsg_t warning_func);
extern lccrt_err_ptr lccrt_err_new( lccrt_err_ptr err, void *data, lccrt_errmsg_t err_func,
                                    lccrt_errmsg_t warning_func);
extern lccrt_err_ptr lccrt_err_delete( lccrt_err_ptr err);

typedef enum
{
    LCCRT_HASH_KEY_INTPTR,
    LCCRT_HASH_KEY_STRING,
    LCCRT_HASH_KEY_LAST
} lccrt_hash_key_type_t;

extern int lccrt_exec_with_fork( const char *path, char *argv[], pid_t *wpid, int fds[2]);

/**
 * Тип размещения переменной.
 */
typedef enum
{
    LCCRT_VAR_LOC_GLOB = 0, /* глобальная переменная (модуля) */
    LCCRT_VAR_LOC_EXT, /* декларация глобальной переменной */
    LCCRT_VAR_LOC_CONSTARG, /* глобальная переменная для константных аргументов */
    LCCRT_VAR_LOC_LOCAL, /* локальная переменная (функции) */
    LCCRT_VAR_LOC_ARG, /* параметр функции */
    LCCRT_VAR_LOC_SYSARG, /* (специальный) параметр функции */
    LCCRT_VAR_LOC_ASM, /* основной параметр (описание) asm-вставки */
    LCCRT_VAR_LOC_LAST
} lccrt_var_loc_t;

#define lccrt_loc_is_ext( l) ((l) == LCCRT_VAR_LOC_EXT)
#define lccrt_loc_is_constarg( l) ((l) == LCCRT_VAR_LOC_CONSTARG)
#define lccrt_loc_is_global( l) \
( \
  ((l) == LCCRT_VAR_LOC_GLOB) \
  || lccrt_loc_is_constarg( l) \
  || lccrt_loc_is_ext( l) \
)
#define lccrt_loc_is_local( l) \
( \
  ((l) == LCCRT_VAR_LOC_LOCAL) \
  || ((l) == LCCRT_VAR_LOC_ARG) \
  || ((l) == LCCRT_VAR_LOC_SYSARG) \
  || lccrt_loc_is_asm( l) \
)
#define lccrt_loc_is_asm( l) \
( \
  ((l) == LCCRT_VAR_LOC_ASM) \
)

/**
 * Тип старшинства при линковке.
 */
typedef enum
{
    LCCRT_LINK_BND_NO = 0,
    LCCRT_LINK_BND_GLOBAL,
    LCCRT_LINK_BND_WEAK,
    LCCRT_LINK_BND_LOCAL,
    LCCRT_LINK_BND_LAST
} lccrt_link_bind_t;

/**
 * Тип видимости при линковке.
 */
typedef enum
{
    LCCRT_LINK_VIS_DEFAULT = 0,
    LCCRT_LINK_VIS_INTERNAL,
    LCCRT_LINK_VIS_HIDDEN,
    LCCRT_LINK_VIS_PROTECTED,
    LCCRT_LINK_VIS_LAST
} lccrt_link_visibility_t;

/**
 * TLS-тип линковки.
 */
typedef enum
{
    LCCRT_LINK_TLS_NO = 0,
    LCCRT_LINK_TLS_DYNAMIC_G,
    LCCRT_LINK_TLS_DYNAMIC_L,
    LCCRT_LINK_TLS_EXEC_I,
    LCCRT_LINK_TLS_EXEC_L,
    LCCRT_LINK_TLS_LAST
} lccrt_link_tls_t;

typedef uint64_t lccrt_link_t;

/**
 * Разновидности флагов инициализации для функции.
 */
typedef enum lccrt_function_init_type_r
{
    LCCRT_FUNC_INIT_NONE,
    LCCRT_FUNC_INIT_CTOR,
    LCCRT_FUNC_INIT_DTOR,
    LCCRT_FUNC_INIT_LAST
} lccrt_function_init_type_t;

/**
 * Названия IR-типов данных.
 */
typedef enum lccrt_type_name_r
{
    LCCRT_TYPE_VOID,
    LCCRT_TYPE_BOOL,
    LCCRT_TYPE_INT,
    LCCRT_TYPE_FLOAT,
    LCCRT_TYPE_PTR,
    LCCRT_TYPE_ARRAY,
    LCCRT_TYPE_VLA_ARRAY,
    LCCRT_TYPE_VECTOR,
    LCCRT_TYPE_STRUCT,
    LCCRT_TYPE_FIELD,
    LCCRT_TYPE_FUNC,
    LCCRT_TYPE_ELLIPSIS,
    LCCRT_TYPE_NAME,
    LCCRT_TYPE_LAST
} lccrt_type_name_t;

/**
 * Нумерация целых IR-типов данных.
 */
typedef enum lccrt_type_name_int_r
{
    LCCRT_INT_SUBTYPE_8 = 0,
    LCCRT_INT_SUBTYPE_16,
    LCCRT_INT_SUBTYPE_32,
    LCCRT_INT_SUBTYPE_64,
    LCCRT_INT_SUBTYPE_128,
    LCCRT_INT_SUBTYPE_LAST
} lccrt_type_name_int_t;

/**
 * Нумерация плавающих IR-типов данных.
 */
typedef enum lccrt_type_name_float_r
{
    LCCRT_FLOAT_SUBTYPE_32 = 0,
    LCCRT_FLOAT_SUBTYPE_64,
    LCCRT_FLOAT_SUBTYPE_80,
    LCCRT_FLOAT_SUBTYPE_128,
    LCCRT_FLOAT_SUBTYPE_LAST
} lccrt_type_name_float_t;

/**
 * Виды инициализаторов.
 */
typedef enum
{
    LCCRT_VARINIT_HEX, /* битовое представление скаляра (не более 64-бит) */
    LCCRT_VARINIT_STR, /* строка */
    LCCRT_VARINIT_ARR, /* массив (в том числе структура как массив собственных полей) */
    LCCRT_VARINIT_ZERO, /* инициализация типа нулями */
    LCCRT_VARINIT_ADDR_VAR, /* адрес переменной */
    LCCRT_VARINIT_ADDR_FUNC, /* адрес функции */
    LCCRT_VARINIT_LAST
} lccrt_varinit_inittype_t;

/**
 * Названия операций.
 */
typedef enum
{
    LCCRT_OPER_LABEL,
    LCCRT_OPER_CMP,
    LCCRT_OPER_BRANCH,
    LCCRT_OPER_BRANCHIF,
    LCCRT_OPER_SWITCH,
    LCCRT_OPER_RET,
    LCCRT_OPER_RETVAL,
    LCCRT_OPER_CALLPROC,
    LCCRT_OPER_CALLFUNC,
    LCCRT_OPER_INVOKEPROC,
    LCCRT_OPER_INVOKEFUNC,
    LCCRT_OPER_LANDINGPAD,
    LCCRT_OPER_VA_ARG,
    LCCRT_OPER_ALLOCA,
    LCCRT_OPER_BITCAST,
    LCCRT_OPER_SELECT,
    LCCRT_OPER_MOVE,
    LCCRT_OPER_VARPTR,
    LCCRT_OPER_ELEMPTR,
    LCCRT_OPER_ELEMREAD,
    LCCRT_OPER_ELEMWRITE,
    LCCRT_OPER_SHUFFLE,
    LCCRT_OPER_LOAD,
    LCCRT_OPER_STORE,
    LCCRT_OPER_SEXT,
    LCCRT_OPER_ZEXT,
    LCCRT_OPER_TRUNC,
    LCCRT_OPER_FPTOFP,
    LCCRT_OPER_FPTOUI,
    LCCRT_OPER_FPTOSI,
    LCCRT_OPER_UITOFP,
    LCCRT_OPER_SITOFP,
    LCCRT_OPER_ADD,
    LCCRT_OPER_SUB,
    LCCRT_OPER_MUL,
    LCCRT_OPER_UDIV,
    LCCRT_OPER_SDIV,
    LCCRT_OPER_UMOD,
    LCCRT_OPER_SMOD,
    LCCRT_OPER_SHL,
    LCCRT_OPER_SHR,
    LCCRT_OPER_SAR,
    LCCRT_OPER_AND,
    LCCRT_OPER_OR,
    LCCRT_OPER_XOR,
    LCCRT_OPER_FADD,
    LCCRT_OPER_FMUL,
    LCCRT_OPER_FSUB,
    LCCRT_OPER_FDIV,
    LCCRT_OPER_FMOD,
    LCCRT_OPER_LAST
} lccrt_oper_name_t;

/**
 * Операция формирует результат в переменной.
 */
#define lccrt_oper_name_is_res( n) \
( ((n) == LCCRT_OPER_CMP) \
  || ((n) == LCCRT_OPER_CALLFUNC) \
  || ((n) == LCCRT_OPER_INVOKEFUNC) \
  || ((n) == LCCRT_OPER_LANDINGPAD) \
  || ((n) == LCCRT_OPER_VA_ARG) \
  || ((n) == LCCRT_OPER_ALLOCA) \
  || ((n) == LCCRT_OPER_BITCAST) \
  || ((n) == LCCRT_OPER_SELECT) \
  || ((n) == LCCRT_OPER_MOVE) \
  || ((n) == LCCRT_OPER_VARPTR) \
  || ((n) == LCCRT_OPER_ELEMPTR) \
  || ((n) == LCCRT_OPER_ELEMREAD) \
  || ((n) == LCCRT_OPER_SHUFFLE) \
  || ((n) == LCCRT_OPER_LOAD) \
  || ((n) == LCCRT_OPER_SEXT) \
  || ((n) == LCCRT_OPER_ZEXT) \
  || ((n) == LCCRT_OPER_TRUNC) \
  || ((n) == LCCRT_OPER_FPTOFP) \
  || ((n) == LCCRT_OPER_FPTOUI) \
  || ((n) == LCCRT_OPER_FPTOSI) \
  || ((n) == LCCRT_OPER_UITOFP) \
  || ((n) == LCCRT_OPER_SITOFP) \
  || ((n) == LCCRT_OPER_ADD) \
  || ((n) == LCCRT_OPER_SUB) \
  || ((n) == LCCRT_OPER_MUL) \
  || ((n) == LCCRT_OPER_UDIV) \
  || ((n) == LCCRT_OPER_SDIV) \
  || ((n) == LCCRT_OPER_UMOD) \
  || ((n) == LCCRT_OPER_SMOD) \
  || ((n) == LCCRT_OPER_SHL) \
  || ((n) == LCCRT_OPER_SHR) \
  || ((n) == LCCRT_OPER_SAR) \
  || ((n) == LCCRT_OPER_AND) \
  || ((n) == LCCRT_OPER_OR) \
  || ((n) == LCCRT_OPER_XOR) \
  || ((n) == LCCRT_OPER_FADD) \
  || ((n) == LCCRT_OPER_FSUB) \
  || ((n) == LCCRT_OPER_FMUL) \
  || ((n) == LCCRT_OPER_FDIV) \
  || ((n) == LCCRT_OPER_FMOD) \
)

/**
 * Детализация типа сравнения.
 */
typedef enum lccrt_cmp_name_r
{
    LCCRT_CMP_EQ,
    LCCRT_CMP_NE,
    /* Знаковые целочисленные сравнения. */
    LCCRT_CMP_LT_I,
    LCCRT_CMP_LE_I,
    LCCRT_CMP_GT_I,
    LCCRT_CMP_GE_I,
    /* Беззнаковые целочисленные сравнения. */
    LCCRT_CMP_LT_U,
    LCCRT_CMP_LE_U,
    LCCRT_CMP_GT_U,
    LCCRT_CMP_GE_U,
    /* Плавающие сравнения с проверкой упорядоченности. */
    LCCRT_CMP_FO,
    LCCRT_CMP_EQ_FO,
    LCCRT_CMP_NE_FO,
    LCCRT_CMP_LT_FO,
    LCCRT_CMP_LE_FO,
    LCCRT_CMP_GT_FO,
    LCCRT_CMP_GE_FO,
    /* Плавающие сравнения без проверки упорядоченности. */
    LCCRT_CMP_FU,
    LCCRT_CMP_EQ_FU,
    LCCRT_CMP_NE_FU,
    LCCRT_CMP_LT_FU,
    LCCRT_CMP_LE_FU,
    LCCRT_CMP_GT_FU,
    LCCRT_CMP_GE_FU,
    LCCRT_CMP_LAST
} lccrt_cmp_name_t;

/**
 * Аргумент операции.
 */
typedef union lccrt_arg_r
{
    lccrt_oper_ptr op; /* аргумент-операция */
    lccrt_var_ptr v; /* аргумент-переменная */
} lccrt_arg_t;

/**
 * Значение 'val', для которого нужно сделать переход на 'dst'.
 */
typedef struct lccrt_switch_alts_r
{
    lccrt_varinit_ptr val; /* значение */
    lccrt_oper_ptr dst; /* точка перехода */
} lccrt_switch_alts_t;

typedef lccrt_context_ptr lccrt_ctx_ptr;
typedef lccrt_plugin_ptr lccrt_plg_ptr;
typedef lccrt_module_ptr lccrt_m_ptr;
typedef lccrt_function_ptr lccrt_f_ptr;
typedef lccrt_function_jit_info_ptr lccrt_fji_ptr;
typedef lccrt_type_ptr lccrt_t_ptr;
typedef lccrt_var_ptr lccrt_v_ptr;
typedef lccrt_varinit_ptr lccrt_vi_ptr;
typedef lccrt_oper_ptr lccrt_o_ptr;
typedef lccrt_oper_iterator_ptr lccrt_oi_ptr;
typedef lccrt_metadata_struct_descr_ptr lccrt_mdsd_ptr;
typedef lccrt_metadata_ptr lccrt_md_ptr;

typedef lccrt_hash_key_type_t lccrt_hkt_t;
typedef struct lccrt_hash_entry_r *lccrt_hash_entry_ptr;
typedef struct lccrt_hash_r *lccrt_hash_ptr;
typedef lccrt_hash_entry_ptr lccrt_he_ptr;
typedef lccrt_hash_ptr lccrt_h_ptr;

extern lccrt_h_ptr lccrt_hash_new( lccrt_ctx_ptr ctx, lccrt_hkt_t type);
extern void lccrt_hash_delete( lccrt_h_ptr ht);
extern int64_t lccrt_hash_length( lccrt_h_ptr ht);
extern lccrt_he_ptr lccrt_hash_push( lccrt_h_ptr ht, uintptr_t key, int *is_new);
extern lccrt_he_ptr lccrt_hash_find( lccrt_h_ptr ht, uintptr_t key);
extern uintptr_t lccrt_hash_remove( lccrt_he_ptr he);
extern uintptr_t lccrt_hash_get_key( lccrt_he_ptr he);
extern uintptr_t lccrt_hash_get( lccrt_he_ptr he);
extern uintptr_t lccrt_hash_set( lccrt_he_ptr he, uintptr_t data);
extern lccrt_he_ptr lccrt_hash_first( lccrt_h_ptr ht);
extern lccrt_he_ptr lccrt_hash_next( lccrt_he_ptr he);

/**
 * \defgroup tyCtxFns Методы структуры lccrt_context_t
 *
 * Список основных методов:
 *     - \ref lccrt_context_new
 *     - \ref lccrt_context_delete
 *     - \ref lccrt_context_get_plugin_asm
 *     - \ref lccrt_context_get_plugin_irrobj
 *     - \ref lccrt_context_find_plugin_irrobj_for_arch
 *     - \ref lccrt_context_get_toolchain
 *
 */
extern lccrt_ctx_ptr lccrt_context_new( lccrt_mem_ptr mem, lccrt_err_ptr err);
extern void lccrt_context_delete( lccrt_ctx_ptr ctx);
extern const char *lccrt_context_get_toolchain( lccrt_ctx_ptr ctx, const char *asm_lib,
                                                const char *targ, const char *tool_type,
                                                const char *tool_name);
extern lccrt_plg_ptr lccrt_context_get_plugin_asm( lccrt_ctx_ptr ctx, const char *plg_name);
extern lccrt_plg_ptr lccrt_context_get_plugin_irrobj( lccrt_ctx_ptr ctx, const char *plg_name);
extern lccrt_plg_ptr lccrt_context_find_plugin_irrobj_for_arch( lccrt_ctx_ptr ctx,
                                                                const char *arch);
extern lccrt_err_t lccrt_context_get_err_info( lccrt_ctx_ptr ctx);
extern lccrt_err_t lccrt_context_set_err_info( lccrt_ctx_ptr ctx, lccrt_err_t err);
#define lccrt_context_error( ctx, id, fmt, ...) \
({ \
    lccrt_err_t e = lccrt_context_get_err_info( ctx); \
    if ( e.error_func ) \
    { \
        e.error_func( e.data, id, fmt, ##__VA_ARGS__); \
    } \
    0; \
})

extern lccrt_m_ptr lccrt_module_new( lccrt_ctx_ptr ctx, const char *mname, int is_m32);
extern void lccrt_module_delete( lccrt_m_ptr m);
extern void lccrt_module_rename( lccrt_m_ptr m, const char *new_name);
extern lccrt_ctx_ptr lccrt_module_get_context( lccrt_m_ptr m);
extern const char *lccrt_module_get_name( lccrt_m_ptr m);
extern void *lccrt_module_get_funcs_list( lccrt_m_ptr m);
extern lccrt_t_ptr lccrt_module_get_first_type( lccrt_m_ptr m);
extern lccrt_v_ptr lccrt_module_get_first_var( lccrt_m_ptr m);
extern lccrt_f_ptr lccrt_module_get_first_func( lccrt_m_ptr m);
extern const char *lccrt_module_get_inline_asm( lccrt_m_ptr m);
extern void lccrt_module_set_inline_asm( lccrt_m_ptr m, const char *asm_text);
extern int lccrt_module_apply_jit_support( lccrt_m_ptr m);
extern int lccrt_module_compile_asm( lccrt_m_ptr m, const char *asm_lib,
                                     lccrt_asm_compile_config_t *cnf);
extern int lccrt_module_is_jit( lccrt_m_ptr m);
extern int lccrt_module_is_ptr32( lccrt_module_ptr m);
extern lccrt_f_ptr lccrt_module_find_function( lccrt_m_ptr m, const char *name);
extern intptr_t lccrt_module_new_metadata( lccrt_m_ptr m, const char *name, lccrt_mdsd_ptr descr);
extern intptr_t lccrt_module_find_metadata( lccrt_m_ptr m, const char *name);
extern lccrt_md_ptr lccrt_module_get_metadata( lccrt_m_ptr m, intptr_t meta);
extern int lccrt_module_print( lccrt_asm_compile_config_t *acc, lccrt_module_ptr m, int fd);
extern int lccrt_module_print_stdout( lccrt_asm_compile_config_t *acc, lccrt_module_ptr m);
extern lccrt_m_ptr lccrt_module_load( lccrt_ctx_ptr ctx, int fd, lccrt_asm_compile_config_t *acc);

extern lccrt_mdsd_ptr lccrt_metadata_new_struct_descr( lccrt_m_ptr m, int num_flds,
                                                       const char *flds[]);
extern lccrt_md_ptr lccrt_metadata_init_fld_int( lccrt_md_ptr meta, const char *fld, uint64_t v);
extern lccrt_md_ptr lccrt_metadata_init_fld_float( lccrt_md_ptr meta, const char *fld, double v);
extern lccrt_md_ptr lccrt_metadata_init_fld_str( lccrt_md_ptr meta, const char *fld, const char *v);
extern lccrt_md_ptr lccrt_metadata_init_fld_struct( lccrt_md_ptr meta, const char *fld,
                                                    lccrt_mdsd_ptr descr);
extern lccrt_md_ptr lccrt_metadata_init_fld_array( lccrt_md_ptr meta, const char *fld, int num_elems);
extern lccrt_md_ptr lccrt_metadata_init_elem_int( lccrt_md_ptr meta, int elem, uint64_t v);
extern lccrt_md_ptr lccrt_metadata_init_elem_float( lccrt_md_ptr meta, int elem, double v);
extern lccrt_md_ptr lccrt_metadata_init_elem_str( lccrt_md_ptr meta, int elem, const char *v);
extern lccrt_md_ptr lccrt_metadata_init_elem_struct( lccrt_md_ptr meta, int elem,
                                                     lccrt_mdsd_ptr descr);
extern lccrt_md_ptr lccrt_metadata_init_elem_array( lccrt_md_ptr meta, int elem, int num_elems);
extern uint64_t lccrt_metadata_get_fld_int( lccrt_md_ptr md, const char *fld);
extern double lccrt_metadata_get_fld_float( lccrt_md_ptr md, const char *fld);
extern const char *lccrt_metadata_get_fld_str( lccrt_md_ptr md, const char *fld);
extern lccrt_md_ptr lccrt_metadata_get_fld_struct( lccrt_md_ptr md, const char *fld);
extern lccrt_md_ptr lccrt_metadata_get_fld_array( lccrt_md_ptr md, const char *fld);
extern uint64_t lccrt_metadata_get_elem_int( lccrt_md_ptr md, int elem);
extern double lccrt_metadata_get_elem_float( lccrt_md_ptr md, int elem);
extern const char *lccrt_metadata_get_elem_str( lccrt_md_ptr md, int elem);
extern lccrt_md_ptr lccrt_metadata_get_elem_struct( lccrt_md_ptr md, int elem);
extern lccrt_md_ptr lccrt_metadata_get_elem_array( lccrt_md_ptr md, int elem);

extern lccrt_f_ptr lccrt_function_new( lccrt_m_ptr m, lccrt_t_ptr fsig,
                                       const char *fname, const char *asm_fname,
                                       lccrt_link_t link, int is_declaration,
                                       int is_builtin);
extern lccrt_m_ptr lccrt_function_get_module( lccrt_f_ptr func);
extern const char *lccrt_function_get_comdat( lccrt_f_ptr func);
extern void lccrt_function_set_comdat( lccrt_f_ptr func, const char *comdat);
extern lccrt_v_ptr lccrt_function_get_arg( lccrt_f_ptr func, int arg_num);
extern lccrt_v_ptr lccrt_function_get_sysarg( lccrt_f_ptr func, int arg_num);
extern void lccrt_function_set_arg( lccrt_f_ptr func, int k_arg, lccrt_v_ptr arg);
extern void lccrt_function_add_arg( lccrt_f_ptr func, int k_arg, lccrt_v_ptr arg);
extern void lccrt_function_set_sysarg( lccrt_f_ptr func, int k_arg, lccrt_v_ptr arg);
extern void lccrt_function_add_sysarg( lccrt_f_ptr func, int k_arg, lccrt_v_ptr arg);
extern lccrt_link_t lccrt_function_get_link( lccrt_f_ptr func);
extern void lccrt_function_set_link( lccrt_f_ptr func, lccrt_link_t li);
extern void lccrt_function_set_section( lccrt_f_ptr func, const char *section);
extern void lccrt_function_set_init_type( lccrt_f_ptr func, lccrt_function_init_type_t init_type);
extern void lccrt_function_set_init_priority( lccrt_f_ptr func, int priority);
extern int lccrt_function_get_attr_does_not_throw( lccrt_f_ptr func);
extern int lccrt_function_set_attr_does_not_throw( lccrt_f_ptr func, int value);
extern int lccrt_function_get_attr_extern_inline( lccrt_f_ptr func);
extern int lccrt_function_set_attr_extern_inline( lccrt_f_ptr func, int value);
extern int lccrt_function_get_attr_used( lccrt_f_ptr func);
extern int lccrt_function_set_attr_used( lccrt_f_ptr func, int value);
extern int lccrt_function_set_declaration( lccrt_f_ptr func, int is_declaration);
extern lccrt_f_ptr lccrt_function_get_next_func( lccrt_f_ptr func);
extern lccrt_t_ptr lccrt_function_get_type( lccrt_f_ptr func);
extern lccrt_v_ptr lccrt_function_get_arg( lccrt_f_ptr func, int arg_num);
extern const char *lccrt_function_get_name( lccrt_f_ptr func);
extern const char *lccrt_function_get_asm_name( lccrt_f_ptr func);
extern const char *lccrt_function_get_section( lccrt_f_ptr func);
extern lccrt_function_init_type_t lccrt_function_get_init_type( lccrt_f_ptr func);
extern int lccrt_function_get_init_priority( lccrt_f_ptr func);
extern lccrt_fji_ptr lccrt_function_get_jit_info( lccrt_f_ptr func);
extern int lccrt_function_get_num_args( lccrt_f_ptr func);
extern int lccrt_function_get_num_sysargs( lccrt_f_ptr func);
extern lccrt_v_ptr lccrt_function_get_first_var( lccrt_f_ptr func);
extern lccrt_o_ptr lccrt_function_get_first_oper( lccrt_f_ptr func);
extern int lccrt_function_is_var_arg( lccrt_f_ptr f);
extern int lccrt_function_is_declaration( lccrt_f_ptr func);
extern int lccrt_function_is_builtin( lccrt_f_ptr func);
extern int lccrt_function_is_used( lccrt_f_ptr func);
extern int lccrt_function_is_profgen( lccrt_f_ptr func);
extern void lccrt_function_set_metadata( lccrt_f_ptr func, intptr_t meta, lccrt_md_ptr value);
extern lccrt_md_ptr lccrt_function_get_metadata( lccrt_f_ptr func, intptr_t meta);

extern lccrt_v_ptr lccrt_fji_get_profgen_tls( lccrt_fji_ptr fji);
extern lccrt_v_ptr lccrt_fji_get_jit_entry( lccrt_fji_ptr fji);

extern lccrt_type_name_t lccrt_type_get_name( lccrt_t_ptr type);
extern lccrt_t_ptr lccrt_type_get_next_type( lccrt_t_ptr type);
extern lccrt_t_ptr lccrt_type_get_parent( lccrt_t_ptr type);
extern uint64_t lccrt_type_get_num_args( lccrt_t_ptr type);
extern lccrt_t_ptr lccrt_type_get_arg( lccrt_t_ptr type, int arg_num);
extern uint64_t lccrt_type_get_bytealign( lccrt_t_ptr type);
extern uint64_t lccrt_type_get_bytesize( lccrt_t_ptr type);
extern uint64_t lccrt_type_get_byteshift( lccrt_t_ptr type);
extern uint64_t lccrt_type_get_bitsubshift( lccrt_t_ptr type);
extern uint64_t lccrt_type_get_bitsubsize( lccrt_t_ptr type);
extern lccrt_v_ptr lccrt_type_get_vla_size( lccrt_t_ptr type);
extern int lccrt_type_get_sign( lccrt_t_ptr type);
extern int lccrt_type_is_void( lccrt_t_ptr type);
extern int lccrt_type_is_bool( lccrt_t_ptr type);
extern int lccrt_type_is_int( lccrt_t_ptr type);
extern int lccrt_type_is_float( lccrt_t_ptr type);
extern int lccrt_type_is_pointer( lccrt_t_ptr type);
extern int lccrt_type_is_typename( lccrt_t_ptr type);
extern int lccrt_type_is_ellipsis( lccrt_t_ptr type);
extern int lccrt_type_is_field( lccrt_t_ptr type);
extern int lccrt_type_is_struct( lccrt_t_ptr type);
extern int lccrt_type_is_union( lccrt_t_ptr type);
extern int lccrt_type_is_array( lccrt_t_ptr type);
extern int lccrt_type_is_vector( lccrt_t_ptr type);
extern int lccrt_type_is_function( lccrt_t_ptr type);
extern int lccrt_type_is_class_elems( lccrt_t_ptr type);
extern int lccrt_type_is_class_struct( lccrt_t_ptr type);
extern int lccrt_type_is_function_var_arg( lccrt_t_ptr type);
extern lccrt_type_name_int_t lccrt_type_int_find_by_bitsize( uint64_t bitsize);
extern lccrt_type_name_float_t lccrt_type_float_find_by_bitsize( uint64_t bitsize);

extern lccrt_t_ptr lccrt_type_make( lccrt_m_ptr m, int type_name,
                                    lccrt_t_ptr parent, lccrt_t_ptr *args,
                                    uint64_t num_args, lccrt_v_ptr vnum,
                                    const char *sym_name, int is_sign, uint64_t bytealign,
                                    uint64_t bytesize, uint64_t byteshift, int bitsubshift,
                                    int bitsubsize, int is_union);
extern int lccrt_type_print( lccrt_t_ptr type, int is_ln);
extern void lccrt_type_set_parent( lccrt_t_ptr ty, lccrt_t_ptr pt);
extern void lccrt_type_set_arg( lccrt_t_ptr ty, int arg_num, lccrt_t_ptr arg);
extern void lccrt_type_set_typename_bytesize( lccrt_t_ptr ty, uint64_t bytesize);
extern void lccrt_type_set_typename_bytealign( lccrt_t_ptr ty, uint64_t bytealign);
extern lccrt_m_ptr lccrt_type_get_module( lccrt_t_ptr ty);
extern lccrt_t_ptr lccrt_type_make_void( lccrt_m_ptr m);
extern lccrt_t_ptr lccrt_type_make_bool( lccrt_m_ptr m);
extern lccrt_t_ptr lccrt_type_make_ellipsis( lccrt_m_ptr m);
extern lccrt_t_ptr lccrt_type_make_int( lccrt_m_ptr m, uint64_t bytesize, int sign);
extern lccrt_t_ptr lccrt_type_make_float( lccrt_m_ptr m, uint64_t bytesize);
extern lccrt_t_ptr lccrt_type_make_intptr( lccrt_m_ptr m);
extern lccrt_t_ptr lccrt_type_make_pintptr( lccrt_m_ptr m);
extern lccrt_t_ptr lccrt_type_make_pvoid( lccrt_m_ptr m);
extern lccrt_t_ptr lccrt_type_make_pbyte( lccrt_m_ptr m);
extern lccrt_t_ptr lccrt_type_make_pchar( lccrt_m_ptr m);
#define lccrt_type_make_u8( m)   lccrt_type_make_int( m, 1, 0)
#define lccrt_type_make_u16( m)  lccrt_type_make_int( m, 2, 0)
#define lccrt_type_make_u32( m)  lccrt_type_make_int( m, 4, 0)
#define lccrt_type_make_u64( m)  lccrt_type_make_int( m, 8, 0)
#define lccrt_type_make_u128( m) lccrt_type_make_int( m, 16, 0)
#define lccrt_type_make_f32( m)  lccrt_type_make_float( m, 4)
#define lccrt_type_make_f64( m)  lccrt_type_make_float( m, 8)
extern lccrt_t_ptr lccrt_type_make_ptr_type( lccrt_t_ptr parent);
extern lccrt_t_ptr lccrt_type_make_func( lccrt_t_ptr ret_type, int num_args,
                                         lccrt_t_ptr *args);
extern lccrt_t_ptr lccrt_type_make_array( lccrt_t_ptr elem, uint64_t num_elems);
extern lccrt_t_ptr lccrt_type_make_vector( lccrt_t_ptr elem, uint64_t num_elems);
extern lccrt_t_ptr lccrt_type_make_vla( lccrt_t_ptr elem, lccrt_v_ptr vnum);
extern lccrt_t_ptr lccrt_type_make_field( lccrt_t_ptr parent, uint64_t bytealign, uint64_t byteshift,
                                          int bitsubshift, int bitsubsize);
extern lccrt_t_ptr lccrt_type_make_struct( lccrt_m_ptr m, uint64_t bytealign, uint64_t bytesize,
                                           int num_flds, lccrt_t_ptr *flds, int is_union);
extern lccrt_t_ptr lccrt_type_make_typename( lccrt_m_ptr m, const char *name,
                                             lccrt_t_ptr type);
#define lccrt_type_make_ptypename( m, name) \
    ( \
      lccrt_type_make_ptr_type( lccrt_type_make_typename( m, name, 0)) \
    )

extern lccrt_link_t lccrt_link_get( lccrt_link_bind_t bind, lccrt_link_visibility_t vis,
                                    lccrt_link_tls_t tls, int is_cnst, int is_alias);
extern lccrt_link_tls_t lccrt_link_get_tls( lccrt_link_t link);
extern lccrt_link_bind_t lccrt_link_get_bnd( lccrt_link_t link);
extern lccrt_link_visibility_t lccrt_link_get_vis( lccrt_link_t link);
extern int lccrt_link_is_alias( lccrt_link_t link);
extern int lccrt_link_is_const( lccrt_link_t link);

extern lccrt_v_ptr lccrt_var_new( void *holder, lccrt_var_loc_t loc,
                                  lccrt_t_ptr type, const char *name,
                                  const char *asm_name, lccrt_link_t link,
                                  unsigned align);
extern void lccrt_var_delete( lccrt_v_ptr var);
extern lccrt_v_ptr lccrt_var_new_local( lccrt_f_ptr f, lccrt_t_ptr type,
                                        const char *name);
extern lccrt_v_ptr lccrt_var_new_asm( lccrt_f_ptr f, lccrt_t_ptr type,
                                      const char *asm_text, const char *asm_cnstr);
extern lccrt_v_ptr lccrt_var_new_declaration( lccrt_m_ptr m, lccrt_t_ptr type,
                                              const char *name, lccrt_link_t link);
extern lccrt_v_ptr lccrt_var_new_constarg( lccrt_m_ptr m, lccrt_t_ptr type, lccrt_vi_ptr vi);
extern lccrt_v_ptr lccrt_var_new_constarg_hex( lccrt_m_ptr m, lccrt_t_ptr type, uint64_t value);
extern lccrt_v_ptr lccrt_var_new_constarg_str( lccrt_m_ptr m, uint64_t s_len, const char *s);

extern const char *lccrt_var_get_comdat( lccrt_v_ptr var);
extern const char *lccrt_var_get_section( lccrt_v_ptr v);
extern lccrt_link_t lccrt_var_get_link( lccrt_v_ptr var);
extern int lccrt_var_get_attr_common( lccrt_v_ptr var);
extern int lccrt_var_get_attr_used( lccrt_v_ptr var);
extern const char *lccrt_var_get_name( lccrt_v_ptr var);
extern const char *lccrt_var_get_asm_name( lccrt_v_ptr var);
extern lccrt_link_t lccrt_var_get_link( lccrt_v_ptr var);
extern unsigned lccrt_var_get_align( lccrt_v_ptr var);
extern lccrt_var_loc_t lccrt_var_get_loc( lccrt_v_ptr var);

extern void lccrt_var_set_comdat( lccrt_v_ptr var, const char *comdat);
extern void lccrt_var_set_section( lccrt_v_ptr v, const char *section_name);
extern lccrt_link_t lccrt_var_set_link( lccrt_v_ptr var, lccrt_link_t li);
extern int lccrt_var_set_attr_common( lccrt_v_ptr var, int value);
extern int lccrt_var_set_attr_used( lccrt_v_ptr var, int value);

extern lccrt_m_ptr lccrt_var_get_module( lccrt_v_ptr var);
extern lccrt_t_ptr lccrt_var_get_type( lccrt_v_ptr var);
extern uint64_t lccrt_var_get_bytesize( lccrt_v_ptr var);
extern lccrt_vi_ptr lccrt_var_get_init_value( lccrt_v_ptr var);
extern lccrt_v_ptr lccrt_var_get_next_var( lccrt_v_ptr var);
extern uint64_t lccrt_var_get_constarg_hex64( lccrt_v_ptr v);
extern int64_t lccrt_var_get_constarg_int64( lccrt_v_ptr v);
extern const char *lccrt_var_get_constarg_str( lccrt_v_ptr v);
extern lccrt_f_ptr lccrt_var_get_constarg_func( lccrt_v_ptr v);
extern lccrt_t_ptr lccrt_var_expand_array( lccrt_v_ptr var, lccrt_t_ptr type);
extern void lccrt_var_set_init_value( lccrt_v_ptr v, lccrt_vi_ptr iv);
extern void lccrt_var_set_init_value_reduce( lccrt_v_ptr v, lccrt_vi_ptr iv);

extern int lccrt_var_is_local( lccrt_v_ptr v);
extern int lccrt_var_is_global( lccrt_v_ptr v);
extern int lccrt_var_is_const( lccrt_var_ptr var);
extern int lccrt_var_is_constarg( lccrt_v_ptr v);
extern int lccrt_var_is_constarg_hex( lccrt_v_ptr v);
extern int lccrt_var_is_constarg_int( lccrt_v_ptr v);
extern int lccrt_var_is_constarg_addr_var( lccrt_v_ptr v);
extern int lccrt_var_is_constarg_addr_func( lccrt_v_ptr v);

extern lccrt_vi_ptr lccrt_varinit_new_zero( lccrt_t_ptr type);
extern lccrt_vi_ptr lccrt_varinit_new_scalar( lccrt_t_ptr type, uint64_t value);
extern lccrt_vi_ptr lccrt_varinit_new_str( lccrt_t_ptr type, uint64_t value_len,
                                           const char *value);
extern lccrt_vi_ptr lccrt_varinit_new_array( lccrt_t_ptr type, uint64_t num_init_elems,
                                             lccrt_vi_ptr *init_data);
extern lccrt_vi_ptr lccrt_varinit_new_addr_var( lccrt_v_ptr var, uint64_t shift);
extern lccrt_vi_ptr lccrt_varinit_new_addr_func( lccrt_f_ptr func, uint64_t shift);

extern lccrt_t_ptr lccrt_varinit_get_type( lccrt_vi_ptr vi);
extern lccrt_varinit_inittype_t lccrt_varinit_get_inittype( lccrt_vi_ptr vi);
extern uint64_t lccrt_varinit_get_num_elems( lccrt_vi_ptr vi);
extern uint64_t lccrt_varinit_get_hex64( lccrt_vi_ptr vi);
extern uint64_t lccrt_varinit_get_zero_or_hex64( lccrt_vi_ptr vi);
extern const char *lccrt_varinit_get_str( lccrt_vi_ptr vi);
extern lccrt_vi_ptr lccrt_varinit_get_elem( lccrt_vi_ptr vi, uint64_t elem_num);
extern lccrt_v_ptr lccrt_varinit_get_addr_var( lccrt_vi_ptr vi);
extern lccrt_f_ptr lccrt_varinit_get_addr_func( lccrt_vi_ptr vi);
extern int lccrt_varinit_is_hex( lccrt_vi_ptr vi);
extern int lccrt_varinit_is_zero( lccrt_vi_ptr vi);
extern int lccrt_varinit_is_zero_or_hex( lccrt_vi_ptr vi);
extern int lccrt_varinit_is_array( lccrt_vi_ptr vi);
extern int lccrt_varinit_is_str( lccrt_vi_ptr vi);
extern int lccrt_varinit_is_addr_var( lccrt_vi_ptr vi);
extern int lccrt_varinit_is_addr_func( lccrt_vi_ptr vi);

extern lccrt_oi_ptr lccrt_oper_iterator_new( lccrt_ctx_ptr ctx);
extern lccrt_oi_ptr lccrt_oper_iterator_delete( lccrt_oi_ptr iter);
extern lccrt_oi_ptr lccrt_oper_iterator_set( lccrt_oi_ptr iter, lccrt_o_ptr oper);
extern lccrt_oi_ptr lccrt_oper_iterator_set_prev( lccrt_oi_ptr iter, lccrt_o_ptr oper);
extern lccrt_o_ptr lccrt_oper_iterator_get_prev( lccrt_oi_ptr iter);
extern lccrt_o_ptr lccrt_oper_iterator_get_next( lccrt_oi_ptr iter);
extern lccrt_v_ptr lccrt_oper_iterator_get_res( lccrt_oper_ptr iter);
#define lccrt_oper_iterator_get_next_res( i) \
( \
  lccrt_oper_iterator_get_res( lccrt_oper_iterator_get_next( i)) \
)
extern lccrt_o_ptr lccrt_oper_iterator_shift( lccrt_oi_ptr i);

extern lccrt_oper_name_t lccrt_oper_get_name( lccrt_o_ptr oper);
extern lccrt_t_ptr lccrt_oper_get_type( lccrt_o_ptr oper);
extern int lccrt_oper_get_num_args( lccrt_o_ptr oper);
extern int lccrt_oper_get_num_sysargs( lccrt_o_ptr oper);
extern lccrt_o_ptr lccrt_oper_get_arg_oper( lccrt_o_ptr oper, int arg_num);
extern lccrt_v_ptr lccrt_oper_get_arg_var( lccrt_o_ptr oper, int arg_num);
extern void lccrt_oper_set_arg_var( lccrt_o_ptr oper, int arg_num, lccrt_v_ptr arg);
extern void lccrt_oper_set_arg_oper( lccrt_o_ptr oper, int arg_num, lccrt_o_ptr arg);
extern lccrt_o_ptr lccrt_oper_get_next( lccrt_o_ptr oper);
extern lccrt_v_ptr lccrt_oper_get_res( lccrt_o_ptr oper);
extern void lccrt_oper_set_res( lccrt_o_ptr oper, lccrt_v_ptr res);
extern int lccrt_oper_is_atomic( lccrt_o_ptr oper);
extern int lccrt_oper_is_volatile( lccrt_o_ptr oper);
extern int lccrt_oper_is_cleanup( lccrt_o_ptr oper);
extern lccrt_o_ptr lccrt_oper_invoke_get_normal( lccrt_o_ptr oper);
extern lccrt_o_ptr lccrt_oper_invoke_get_unwind( lccrt_o_ptr oper);
extern void lccrt_oper_invoke_set_normal( lccrt_o_ptr oper, lccrt_o_ptr normal);
extern void lccrt_oper_invoke_set_unwind( lccrt_o_ptr oper, lccrt_o_ptr unwind);

extern int lccrt_oper_name_is_arg_var( lccrt_oper_name_t name, int arg_num);
extern int lccrt_oper_name_is_arg_oper( lccrt_oper_name_t name, int arg_num);

extern lccrt_o_ptr lccrt_oper_new( lccrt_f_ptr f, int oper_name, lccrt_t_ptr type,
                                   int num_args, lccrt_arg_t *args, int num_sysargs,
                                   lccrt_v_ptr res, lccrt_oi_ptr i);
extern int lccrt_oper_print( lccrt_o_ptr oper, int is_ln);
extern int lccrt_oper_snprint( lccrt_o_ptr op, int is_ln, char *s, int l);
extern void lccrt_oper_set_volatile( lccrt_oper_ptr oper, int value);
extern void lccrt_oper_set_atomic( lccrt_oper_ptr oper, int value);
extern void lccrt_oper_set_cleanup( lccrt_oper_ptr oper, int value);
extern int lccrt_oper_is_branch( lccrt_oper_ptr o);
extern int lccrt_oper_is_switch( lccrt_oper_ptr o);
extern int lccrt_oper_is_call( lccrt_oper_ptr o);
extern int lccrt_oper_is_invoke( lccrt_oper_ptr o);
extern int lccrt_oper_is_landingpad( lccrt_oper_ptr o);
extern int lccrt_oper_is_label( lccrt_oper_ptr o);
extern int lccrt_oper_is_ret( lccrt_oper_ptr o);
extern int lccrt_oper_is_retval( lccrt_oper_ptr o);
extern void lccrt_oper_delete( lccrt_o_ptr o);
extern lccrt_f_ptr lccrt_oper_get_function( lccrt_o_ptr oper);
extern lccrt_t_ptr lccrt_oper_get_res_type( lccrt_o_ptr oper);
extern const char *lccrt_oper_get_label( lccrt_o_ptr oper);
extern lccrt_o_ptr lccrt_oper_new_label( lccrt_f_ptr func, const char *name,
                                         lccrt_oi_ptr iter);
extern lccrt_o_ptr lccrt_oper_new_branch( lccrt_f_ptr func, lccrt_oper_ptr dst,
                                          lccrt_oi_ptr iter);
extern lccrt_o_ptr lccrt_oper_new_branchif( lccrt_f_ptr func, lccrt_v_ptr predct,
                                            lccrt_o_ptr dst_true, lccrt_o_ptr dst_false,
                                            lccrt_oi_ptr iter);
extern lccrt_o_ptr lccrt_oper_new_switch( lccrt_f_ptr f, lccrt_v_ptr pa, int num_alts,
                                          lccrt_switch_alts_t *alts, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_va_arg( lccrt_f_ptr f, lccrt_v_ptr pa, lccrt_t_ptr pt,
                                          lccrt_v_ptr pr, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_alloca( lccrt_f_ptr f, lccrt_v_ptr pa,
                                          lccrt_v_ptr pr, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_cmp( lccrt_f_ptr f, lccrt_varinit_ptr pn,
                                       lccrt_v_ptr pa, lccrt_v_ptr pb,
                                       lccrt_v_ptr pres, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_load( lccrt_f_ptr f, lccrt_v_ptr pa, lccrt_v_ptr pres,
                                        lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_store( lccrt_f_ptr f, lccrt_v_ptr pa, lccrt_v_ptr pb,
                                         lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_sext( lccrt_f_ptr f, lccrt_v_ptr a, lccrt_t_ptr t,
                                        lccrt_v_ptr r, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_zext( lccrt_f_ptr f, lccrt_v_ptr a, lccrt_t_ptr t,
                                        lccrt_v_ptr r, lccrt_oi_ptr i);
extern lccrt_o_ptr lccrt_oper_new_trunc( lccrt_f_ptr f, lccrt_v_ptr a, lccrt_t_ptr t,
                                         lccrt_v_ptr r, lccrt_oi_ptr i);
extern lccrt_o_ptr lccrt_oper_new_fptofp( lccrt_f_ptr f, lccrt_v_ptr a, lccrt_t_ptr t,
                                          lccrt_v_ptr r, lccrt_oi_ptr i);
extern lccrt_o_ptr lccrt_oper_new_fptoui( lccrt_f_ptr f, lccrt_v_ptr a, lccrt_t_ptr t,
                                          lccrt_v_ptr r, lccrt_oi_ptr i);
extern lccrt_o_ptr lccrt_oper_new_fptosi( lccrt_f_ptr f, lccrt_v_ptr a, lccrt_t_ptr t,
                                          lccrt_v_ptr r, lccrt_oi_ptr i);
extern lccrt_o_ptr lccrt_oper_new_uitofp( lccrt_f_ptr f, lccrt_v_ptr a, lccrt_t_ptr t,
                                          lccrt_v_ptr r, lccrt_oi_ptr i);
extern lccrt_o_ptr lccrt_oper_new_sitofp( lccrt_f_ptr f, lccrt_v_ptr a, lccrt_t_ptr t,
                                          lccrt_v_ptr r, lccrt_oi_ptr i);
extern lccrt_o_ptr lccrt_oper_new_bitcast( lccrt_f_ptr f, lccrt_v_ptr a, lccrt_t_ptr t,
                                           lccrt_v_ptr r, lccrt_oi_ptr i);
extern lccrt_o_ptr lccrt_oper_new_select( lccrt_f_ptr f, lccrt_v_ptr a, lccrt_v_ptr b,
                                          lccrt_v_ptr c, lccrt_v_ptr r, lccrt_oi_ptr i);
extern lccrt_o_ptr lccrt_oper_new_move( lccrt_f_ptr f, lccrt_v_ptr a, lccrt_v_ptr r,
                                        lccrt_oi_ptr i);
extern lccrt_o_ptr lccrt_oper_new_varptr( lccrt_f_ptr f, lccrt_v_ptr a, lccrt_v_ptr r,
                                          lccrt_oi_ptr i);
extern lccrt_o_ptr lccrt_oper_new_add( lccrt_f_ptr f,
                                       lccrt_v_ptr pa, lccrt_v_ptr pb,
                                       lccrt_v_ptr pr, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_sub( lccrt_f_ptr f,
                                       lccrt_v_ptr pa, lccrt_v_ptr pb,
                                       lccrt_v_ptr pr, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_mul( lccrt_f_ptr f,
                                       lccrt_v_ptr pa, lccrt_v_ptr pb,
                                       lccrt_v_ptr pr, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_udiv( lccrt_f_ptr f,
                                        lccrt_v_ptr pa, lccrt_v_ptr pb,
                                        lccrt_v_ptr pr, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_sdiv( lccrt_f_ptr f,
                                        lccrt_v_ptr pa, lccrt_v_ptr pb,
                                        lccrt_v_ptr pr, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_umod( lccrt_f_ptr f,
                                        lccrt_v_ptr pa, lccrt_v_ptr pb,
                                        lccrt_v_ptr pr, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_smod( lccrt_f_ptr f,
                                        lccrt_v_ptr pa, lccrt_v_ptr pb,
                                        lccrt_v_ptr pr, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_shl( lccrt_f_ptr f,
                                       lccrt_v_ptr pa, lccrt_v_ptr pb,
                                       lccrt_v_ptr pr, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_shr( lccrt_f_ptr f,
                                       lccrt_v_ptr pa, lccrt_v_ptr pb,
                                       lccrt_v_ptr pr, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_sar( lccrt_f_ptr f,
                                       lccrt_v_ptr pa, lccrt_v_ptr pb,
                                       lccrt_v_ptr pr, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_and( lccrt_f_ptr f,
                                       lccrt_v_ptr pa, lccrt_v_ptr pb,
                                       lccrt_v_ptr pr, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_or( lccrt_f_ptr f,
                                      lccrt_v_ptr pa, lccrt_v_ptr pb,
                                      lccrt_v_ptr pr, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_xor( lccrt_f_ptr f,
                                       lccrt_v_ptr pa, lccrt_v_ptr pb,
                                       lccrt_v_ptr pr, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_fadd( lccrt_f_ptr f,
                                        lccrt_v_ptr pa, lccrt_v_ptr pb,
                                        lccrt_v_ptr pr, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_fsub( lccrt_f_ptr f,
                                        lccrt_v_ptr pa, lccrt_v_ptr pb,
                                        lccrt_v_ptr pr, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_fmul( lccrt_f_ptr f,
                                        lccrt_v_ptr pa, lccrt_v_ptr pb,
                                        lccrt_v_ptr pr, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_fdiv( lccrt_f_ptr f,
                                        lccrt_v_ptr pa, lccrt_v_ptr pb,
                                        lccrt_v_ptr pr, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_fmod( lccrt_f_ptr f,
                                        lccrt_v_ptr pa, lccrt_v_ptr pb,
                                        lccrt_v_ptr pr, lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_return( lccrt_f_ptr f, lccrt_v_ptr pa,
                                          lccrt_oi_ptr pi);
extern lccrt_o_ptr lccrt_oper_new_call( lccrt_f_ptr func, lccrt_t_ptr sig, int num_args,
                                        lccrt_v_ptr *args, int num_sysargs,
                                        lccrt_v_ptr *sysargs, lccrt_v_ptr res,
                                        lccrt_oi_ptr iter);
extern lccrt_o_ptr lccrt_oper_new_invoke( lccrt_f_ptr func, lccrt_t_ptr sig, int num_args,
                                          lccrt_v_ptr *args, int num_sysargs,
                                          lccrt_v_ptr *sysargs, lccrt_o_ptr normal,
                                          lccrt_o_ptr unwind, lccrt_v_ptr res,
                                          lccrt_oi_ptr iter);
extern lccrt_o_ptr lccrt_oper_new_landingpad( lccrt_f_ptr func, int num_args,
                                              lccrt_v_ptr *args, lccrt_t_ptr t,
                                              lccrt_v_ptr res, lccrt_oi_ptr iter);
extern lccrt_o_ptr lccrt_oper_new_elemptr( lccrt_f_ptr func,
                                           int num_args, lccrt_v_ptr *args, lccrt_v_ptr res,
                                           lccrt_oi_ptr iter);
extern lccrt_o_ptr lccrt_oper_new_elemread( lccrt_f_ptr func, int num_args,
                                            lccrt_v_ptr *args, lccrt_v_ptr res,
                                            lccrt_oi_ptr iter);
extern lccrt_o_ptr lccrt_oper_new_elemwrite( lccrt_f_ptr func, int num_args,
                                             lccrt_v_ptr *args, lccrt_oi_ptr iter);
extern lccrt_o_ptr lccrt_oper_new_shuffle( lccrt_f_ptr func, lccrt_v_ptr pa, lccrt_v_ptr pb,
                                           lccrt_v_ptr pm, lccrt_v_ptr pr, lccrt_oi_ptr iter);
extern void lccrt_oper_set_metadata( lccrt_o_ptr oper, intptr_t meta, lccrt_md_ptr value);
extern lccrt_md_ptr lccrt_oper_get_metadata( lccrt_o_ptr oper, intptr_t meta);

extern lccrt_fs_ptr lccrt_fs_new_file( lccrt_ctx_ptr ctx, FILE *file);
extern lccrt_fs_ptr lccrt_fs_open_file( lccrt_ctx_ptr ctx, const char *name, int is_load);
extern lccrt_fs_ptr lccrt_fs_new_buffer( lccrt_ctx_ptr ctx, char *buff, uint64_t buff_size);
extern int lccrt_fs_delete( lccrt_fs_ptr f);
extern char *lccrt_fs_get_buffer( lccrt_fs_ptr f);
extern void lccrt_asm_print_module( lccrt_fs_ptr fs, lccrt_module_ptr m);

typedef lccrt_irr_jit_region_ptr lccrt_irr_jr_ptr;
typedef lccrt_irr_execute_context_ptr lccrt_irr_ec_ptr;
typedef lccrt_irr_code_ptr lccrt_irr_c_ptr;
typedef lccrt_irr_node_ptr lccrt_irr_n_ptr;
typedef lccrt_irr_oper_ptr lccrt_irr_o_ptr;
typedef lccrt_irr_oper_iter_ptr lccrt_irr_oi_ptr;
typedef lccrt_irr_arg_ptr lccrt_irr_a_ptr;
typedef lccrt_irr_reg_ptr lccrt_irr_r_ptr;
typedef lccrt_irr_label_ptr lccrt_irr_l_ptr;
typedef lccrt_irr_reloc_ptr lccrt_irr_rl_ptr;
typedef lccrt_irr_reloc_unit_ptr lccrt_irr_rlu_ptr;
typedef lccrt_irr_resolve_virt_addr_func_t lccrt_irr_rva_func_t;

#define LCCRT_IRR_TYPE_REGS_MAX (256ULL)
#define LCCRT_IRR_ARG_GET_REG_TYPE( a) ((a)->data.hex / LCCRT_IRR_TYPE_REGS_MAX)
#define LCCRT_IRR_ARG_GET_REG_ADDR( a) ((a)->data.hex % LCCRT_IRR_TYPE_REGS_MAX)
#define LCCRT_IRR_ARG_IS_REG( a, rtype, raddr) \
( \
  ((a)->type == LCCRT_IRR_ARG_TYPE_REG) \
  && (LCCRT_IRR_ARG_GET_REG_TYPE( a) == rtype) \
  && (LCCRT_IRR_ARG_GET_REG_ADDR( a) == raddr) \
)

typedef enum
{
    LCCRT_IRR_REG_SYS,
    LCCRT_IRR_REG_PHYS,
    LCCRT_IRR_REG_VIRT,
    LCCRT_IRR_REG_TYPE_LAST
} lccrt_irr_reg_type_t;

typedef enum
{
    LCCRT_IRR_OPER_NOP,
    LCCRT_IRR_OPER_ASSERT,
    LCCRT_IRR_OPER_MOV_U32,
    LCCRT_IRR_OPER_MOV_U64,
    LCCRT_IRR_OPER_AND_U64,
    LCCRT_IRR_OPER_OR_U64,
    LCCRT_IRR_OPER_XOR_U64,
    LCCRT_IRR_OPER_SHL_U64,
    LCCRT_IRR_OPER_SHR_U64,
    LCCRT_IRR_OPER_SAR_U64,
    //LCCRT_IRR_OPER_SCL_U64,
    //LCCRT_IRR_OPER_SCR_U64,
    LCCRT_IRR_OPER_ADD_U64,
    LCCRT_IRR_OPER_SUB_U64,
    LCCRT_IRR_OPER_MUL_U64,
    LCCRT_IRR_OPER_SDIV_U64,
    LCCRT_IRR_OPER_UDIV_U64,
    LCCRT_IRR_OPER_SMOD_U64,
    LCCRT_IRR_OPER_UMOD_U64,
    LCCRT_IRR_OPER_EXT_S8,
    LCCRT_IRR_OPER_EXT_S16,
    LCCRT_IRR_OPER_EXT_S32,
    /* Взятие битового поля
         arg0 (int64) - источник битового поля
         arg1 (int64) - битовое смещение поля
         arg2 (int64) - битовая длина поля */
    LCCRT_IRR_OPER_GETFIELD,
    LCCRT_IRR_OPER_CONV_S32_F32,
    LCCRT_IRR_OPER_CONV_S32_F64,
    LCCRT_IRR_OPER_CONV_S64_F32,
    LCCRT_IRR_OPER_CONV_S64_F64,
    LCCRT_IRR_OPER_CONV_F32_F64,
    LCCRT_IRR_OPER_CONV_F64_F32,
    LCCRT_IRR_OPER_FADD_F32,
    LCCRT_IRR_OPER_FADD_F64,
    LCCRT_IRR_OPER_FSUB_F32,
    LCCRT_IRR_OPER_FSUB_F64,
    LCCRT_IRR_OPER_FMUL_F32,
    LCCRT_IRR_OPER_FMUL_F64,
    LCCRT_IRR_OPER_FDIV_F32,
    LCCRT_IRR_OPER_FDIV_F64,
    LCCRT_IRR_OPER_FSQRT_F64,
    LCCRT_IRR_OPER_CMPE_U64,
    LCCRT_IRR_OPER_CMPNE_U64,
    LCCRT_IRR_OPER_CMPL_U64,
    LCCRT_IRR_OPER_CMPLE_U64,
    LCCRT_IRR_OPER_CMPB_U64,
    LCCRT_IRR_OPER_CMPBE_U64,
    /* Сравнение формата float64 на "не упорядочено"
         arg0 (int64) - первый аргумент
         arg1 (int64) - второй аргумент
         res0 (int64) - (((bitcast float32/float64)arg0 vs (bitcast float32/float64)arg1)) */
    LCCRT_IRR_OPER_FCMP_U_F32,
    LCCRT_IRR_OPER_FCMP_U_F64,
    /* Сравнение формата float64 на "упорядочено"
         arg0 (int64) - первый аргумент
         arg1 (int64) - второй аргумент
         res0 (int64) - (((bitcast float32/float64)arg0 vs (bitcast float32/float64)arg1)) */
    LCCRT_IRR_OPER_FCMP_O_F32,
    LCCRT_IRR_OPER_FCMP_O_F64,
    /* Сравнение формата float64 на "равно или не упорядочено"
         arg0 (int64) - первый аргумент
         arg1 (int64) - второй аргумент
         res0 (int64) - (((bitcast float64)arg0 == (bitcast float64)arg1) || unordered) */
    LCCRT_IRR_OPER_FCMPE_U_F64,
    /* Сравнение формата float64 на "равно"
         arg0 (int64) - первый аргумент
         arg1 (int64) - второй аргумент
         res0 (int64) - (((bitcast float32/float64)arg0 == (bitcast float32/float64)arg1)) */
    LCCRT_IRR_OPER_FCMPE_O_F32,
    LCCRT_IRR_OPER_FCMPE_O_F64,
    /* Сравнение формата float64 на "равно или не упорядочено"
         arg0 (int64) - первый аргумент
         arg1 (int64) - второй аргумент
         res0 (int64) - (((bitcast float64)arg0 < (bitcast float64)arg1) || unordered) */
    LCCRT_IRR_OPER_FCMPL_U_F64,
    /* Сравнение формата float64 на "равно"
         arg0 (int64) - первый аргумент
         arg1 (int64) - второй аргумент
         res0 (int64) - (((bitcast float32/float64)arg0 < (bitcast float32/float64)arg1)) */
    LCCRT_IRR_OPER_FCMPL_O_F32,
    LCCRT_IRR_OPER_FCMPL_O_F64,
    /* Сравнение формата float64 на "равно или не упорядочено"
         arg0 (int64) - первый аргумент
         arg1 (int64) - второй аргумент
         res0 (int64) - (((bitcast float64)arg0 <= (bitcast float64)arg1) || unordered) */
    LCCRT_IRR_OPER_FCMPLE_U_F64,
    /* Сравнение формата float64 на "равно"
         arg0 (int64) - первый аргумент
         arg1 (int64) - второй аргумент
         res0 (int64) - (((bitcast float32/float64)arg0 <= (bitcast float32/float64)arg1)) */
    LCCRT_IRR_OPER_FCMPLE_O_F32,
    LCCRT_IRR_OPER_FCMPLE_O_F64,
    /* Выбор значения
         arg0 (int64) - итоговое значение, если arg2 == true
         arg1 (int64) - итоговое значение, если arg2 == false
         arg2 (int64) - предикат выбора
         res0 (int64) - (arg2 ? arg0 : arg1) */
    LCCRT_IRR_OPER_SELECT_U64,
    /* Условная перессылка, если предикат true
         arg0 (int64) - итоговое значение, если arg1 == true
         arg1 (int64) - предикат выбора
         res0 (int64) - (if ( arg1 ) res0 = arg1;) */
    LCCRT_IRR_OPER_MOVTHEN_U64,
    /* Условная перессылка, если предикат false
         arg0 (int64) - итоговое значение, если arg1 == false
         arg1 (int64) - предикат выбора
         res0 (int64) - (if ( !arg1 ) res0 = arg1;) */
    LCCRT_IRR_OPER_MOVELSE_U64,
    LCCRT_IRR_OPER_LD_U8,
    LCCRT_IRR_OPER_LD_U16,
    LCCRT_IRR_OPER_LD_U32,
    LCCRT_IRR_OPER_LD_U64,
    LCCRT_IRR_OPER_LD_S8,
    LCCRT_IRR_OPER_LD_S16,
    LCCRT_IRR_OPER_LD_S32,
    LCCRT_IRR_OPER_ST_U8,
    LCCRT_IRR_OPER_ST_U16,
    LCCRT_IRR_OPER_ST_U32,
    LCCRT_IRR_OPER_ST_U64,
    /* Безусловный переход по литералу "ленивой" линковки
         arg0 (reloc) - литерал перехода */
    LCCRT_IRR_OPER_BRANCH_RELOC,
    /* Условный переход по литералам "ленивой" линковки
         arg0 (reloc) - литерал перехода, если arg2 == true
         arg1 (reloc) - литерал перехода, если arg2 == false
         arg2 (int64) - предикат перехода */
    LCCRT_IRR_OPER_BRANCHIF_RELOC,
    /* Безусловный переход по виртуальному адресу
         arg0 (int64) - виртуальный адрес перехода */
    LCCRT_IRR_OPER_BRANCH_DYNAMIC,
    /* Выполнить вызов функции void (*)( int64, int64)
         arg0 (int64) - адрес вызываемой функции
         arg1 (int64) - 1-ый аргумент
         arg2 (int64) - 2-ой аргумент */
    LCCRT_IRR_OPER_CALL_V_U64U64,
    /* Завершить исполнение
         arg0 (int64) - возвращаемое значение (через контекст исполнения) */
    LCCRT_IRR_OPER_EXIT,
    /* Адресуемая метка */
    LCCRT_IRR_OPER_LABEL,
    /* Проверить совпадение байтовых массивов и вызвать исключение (abort),
       если массивы не равны
         arg0 (int64) - байтовый размер массивов
         arg1 (int64) - адрес первого массива
         arg2 (int64) - адрес второго массива */
    LCCRT_IRR_OPER_TESTCODE,
    /* Вызвать функцию обработчик особой ситуации
         arg0 (int64) - адрес функции обработчика
         arg1 (int64) - первый аргумент
         arg2 (int64) - второй аргумент */
    LCCRT_IRR_OPER_BREAKPOINT,
    LCCRT_IRR_OPER_LAST
} lccrt_irr_oper_name_t;

#define lccrt_irr_oper_name_is_res( n) \
( \
  (LCCRT_IRR_OPER_ASSERT < (n)) && ((n) <= LCCRT_IRR_OPER_LD_S32) \
)

#define lccrt_irr_oper_name_is_arith( n) \
( \
  (LCCRT_IRR_OPER_NOP <= (n)) && ((n) <= LCCRT_IRR_OPER_MOVELSE_U64) \
)

#define lccrt_irr_oper_name_is_load( n) \
( \
  ((LCCRT_IRR_OPER_LD_U8 <= (n)) && ((n) <= LCCRT_IRR_OPER_LD_U64)) \
)

#define lccrt_irr_oper_name_is_store( n) \
( \
  ((LCCRT_IRR_OPER_ST_U8 <= (n)) && ((n) <= LCCRT_IRR_OPER_ST_U64)) \
)

#define lccrt_irr_oper_name_is_branch( n) \
( \
  ((LCCRT_IRR_OPER_BRANCH_RELOC <= (n)) && ((n) <= LCCRT_IRR_OPER_BRANCH_DYNAMIC)) \
)

#define lccrt_irr_oper_name_is_exit( n) \
( \
  ((LCCRT_IRR_OPER_EXIT <= (n)) && ((n) <= LCCRT_IRR_OPER_EXIT)) \
)

extern lccrt_irr_jr_ptr lccrt_irr_jit_region_new( lccrt_ctx_ptr ctx, const char *arch, lccrt_plg_ptr plg);
extern lccrt_irr_c_ptr lccrt_irr_code_new( lccrt_irr_jr_ptr jr, lccrt_irr_n_ptr node);
extern lccrt_irr_n_ptr lccrt_irr_node_new( lccrt_irr_jr_ptr jr, int64_t user_ident);
extern lccrt_irr_ec_ptr lccrt_irr_exec_ctx_new( lccrt_irr_jr_ptr jr, int max_regs[LCCRT_IRR_REG_TYPE_LAST],
                                                uint64_t *regs[LCCRT_IRR_REG_TYPE_LAST]);
extern lccrt_irr_r_ptr lccrt_irr_reg_new( lccrt_irr_jr_ptr jr);
extern lccrt_irr_a_ptr lccrt_irr_arg_new( lccrt_irr_jr_ptr jr);
extern lccrt_irr_oi_ptr lccrt_irr_oper_iter_new( lccrt_irr_jr_ptr jr, lccrt_irr_n_ptr node);
extern lccrt_irr_rl_ptr lccrt_irr_reloc_new( lccrt_irr_jr_ptr jr, int64_t virt_addr, lccrt_irr_c_ptr code);
extern void lccrt_irr_reloc_delete( lccrt_irr_rl_ptr rel);
extern void lccrt_irr_exec_ctx_delete( lccrt_irr_ec_ptr ec);
extern void lccrt_irr_node_delete( lccrt_irr_n_ptr node);
extern void lccrt_irr_reg_delete( lccrt_ctx_ptr ctx, lccrt_irr_r_ptr arg);
extern void lccrt_irr_arg_delete( lccrt_ctx_ptr ctx, lccrt_irr_a_ptr arg);
extern void lccrt_irr_oper_iter_delete( lccrt_irr_oi_ptr oi);

extern lccrt_ctx_ptr lccrt_irr_jit_region_get_context( lccrt_irr_jr_ptr jr);
extern lccrt_plg_ptr lccrt_irr_jit_region_get_plugin_irrobj( lccrt_irr_jr_ptr jr);
extern lccrt_irr_n_ptr lccrt_irr_jit_region_get_virt_addr_node( lccrt_irr_jr_ptr jr, uint64_t vaddr);
extern uint64_t lccrt_irr_jit_region_get_virt_addr_data( lccrt_irr_jr_ptr jr, uint64_t vaddr);
extern void lccrt_irr_jit_region_set_virt_addr_node( lccrt_irr_jr_ptr jr, uint64_t vaddr, lccrt_irr_n_ptr node);
extern void lccrt_irr_jit_region_set_virt_addr_data( lccrt_irr_jr_ptr jr, uint64_t vaddr, uint64_t data);
extern lccrt_irr_rva_func_t lccrt_irr_jit_region_get_ra_func( lccrt_irr_jr_ptr jr);
extern void *lccrt_irr_jit_region_get_ra_data( lccrt_irr_jr_ptr jr);
extern void **lccrt_irr_jit_region_get_ra_table0( lccrt_irr_jr_ptr jr);
extern int lccrt_irr_jit_region_get_ra_level( lccrt_irr_jr_ptr jr);
extern int lccrt_irr_jit_region_get_ra_bits( lccrt_irr_jr_ptr jr);
extern int lccrt_irr_jit_region_get_ra_tail_bits( lccrt_irr_jr_ptr jr);
extern void lccrt_irr_jit_region_set_ra_func( lccrt_irr_jr_ptr jr, lccrt_irr_rva_func_t func, void *data);
extern void lccrt_irr_jit_region_set_ra_table( lccrt_irr_jr_ptr jr, void **table0, int level, int bits, int tail_bits);
extern int lccrt_irr_jit_region_is_debug_exit_all( lccrt_irr_jr_ptr jr);
extern void lccrt_irr_jit_region_set_debug_exit_all( lccrt_irr_jr_ptr jr, int value);

extern lccrt_irr_jr_ptr lccrt_irr_exec_ctx_get_jit_region( lccrt_irr_ec_ptr ec);
extern int lccrt_irr_exec_ctx_get_reg_max( lccrt_irr_ec_ptr ec, lccrt_irr_reg_type_t type);
/*extern uint64_t lccrt_irr_exec_ctx_get_reg( lccrt_irr_ec_ptr ec, lccrt_irr_reg_type_t type,
                                            int64_t num);
extern uint64_t lccrt_irr_exec_ctx_set_reg( lccrt_irr_ec_ptr ec, lccrt_irr_reg_type_t type,
                                            int64_t num, uint64_t value);*/
extern uint64_t *lccrt_irr_exec_ctx_get_reg_addr( lccrt_irr_ec_ptr ec, lccrt_irr_reg_type_t type, int64_t num);
extern int64_t lccrt_irr_exec_ctx_get_exit_value( lccrt_irr_ec_ptr ec);
extern void lccrt_irr_exec_ctx_set_exit_value( lccrt_irr_ec_ptr ec, int64_t exit_value);

extern lccrt_irr_r_ptr lccrt_irr_reg_init( lccrt_irr_r_ptr reg, lccrt_irr_reg_type_t type,
                                           int64_t num);
#define lccrt_irr_reg_init_sys( reg, num)  lccrt_irr_reg_init( reg, LCCRT_IRR_REG_SYS, num)
#define lccrt_irr_reg_init_phys( reg, num) lccrt_irr_reg_init( reg, LCCRT_IRR_REG_PHYS, num)
#define lccrt_irr_reg_init_virt( reg, num) lccrt_irr_reg_init( reg, LCCRT_IRR_REG_VIRT, num)
extern lccrt_irr_a_ptr lccrt_irr_arg_init_hex( lccrt_irr_a_ptr arg, lccrt_irr_hex_t c);
extern lccrt_irr_a_ptr lccrt_irr_arg_init_reg( lccrt_irr_a_ptr arg, lccrt_irr_r_ptr reg);
extern lccrt_irr_a_ptr lccrt_irr_arg_init_reloc( lccrt_irr_a_ptr arg, lccrt_irr_rl_ptr reloc);
extern int lccrt_irr_arg_is_hex( lccrt_irr_a_ptr arg);
extern int lccrt_irr_arg_is_reloc( lccrt_irr_a_ptr arg);
extern int lccrt_irr_arg_is_reg( lccrt_irr_a_ptr arg);
extern int lccrt_irr_arg_is_reg_type( lccrt_irr_a_ptr arg, lccrt_irr_reg_type_t type);
extern int64_t lccrt_irr_arg_get_hex( lccrt_irr_a_ptr arg);
extern lccrt_irr_rl_ptr lccrt_irr_arg_get_reloc( lccrt_irr_a_ptr arg);
extern int64_t lccrt_irr_arg_get_reg_addr( lccrt_irr_a_ptr arg);
extern lccrt_irr_reg_type_t lccrt_irr_arg_get_reg_type( lccrt_irr_a_ptr arg);
extern lccrt_irr_oi_ptr lccrt_irr_oper_iter_init( lccrt_irr_oi_ptr oi, lccrt_irr_n_ptr node);
extern lccrt_irr_oi_ptr lccrt_irr_oper_iter_init_after( lccrt_irr_oi_ptr j, lccrt_irr_n_ptr node,
                                                        lccrt_irr_o_ptr oper);
extern lccrt_irr_oi_ptr lccrt_irr_oper_iter_init_before( lccrt_irr_oi_ptr j, lccrt_irr_n_ptr node,
                                                         lccrt_irr_o_ptr oper);
extern lccrt_irr_n_ptr lccrt_irr_oper_iter_get_node( lccrt_irr_oi_ptr oi);
extern lccrt_irr_o_ptr lccrt_irr_oper_iter_get_prev( lccrt_irr_oi_ptr oi);
extern lccrt_irr_o_ptr lccrt_irr_oper_iter_get_next( lccrt_irr_oi_ptr oi);
extern lccrt_irr_oi_ptr lccrt_irr_oper_iter_shift( lccrt_irr_oi_ptr oi, int is_next);
#define lccrt_irr_oper_iter_shift_next( oi) lccrt_irr_oper_iter_shift( oi, 1)
#define lccrt_irr_oper_iter_shift_prev( oi) lccrt_irr_oper_iter_shift( oi, 0)
extern lccrt_irr_o_ptr lccrt_irr_oper_new( lccrt_irr_n_ptr node, lccrt_irr_oper_name_t name,
                                           int num_args, lccrt_irr_a_ptr args[],
                                           lccrt_irr_a_ptr res, lccrt_irr_oi_ptr iter);
extern void lccrt_irr_oper_delete( lccrt_irr_o_ptr oper);
extern lccrt_irr_oper_name_t lccrt_irr_oper_get_name( lccrt_irr_o_ptr oper);
extern int lccrt_irr_oper_get_num_args( lccrt_irr_o_ptr oper);
extern lccrt_irr_a_ptr lccrt_irr_oper_get_arg( lccrt_irr_o_ptr oper, int arg_num);
extern lccrt_irr_a_ptr lccrt_irr_oper_get_res( lccrt_irr_o_ptr oper);
extern lccrt_irr_c_ptr lccrt_irr_label_get_code( lccrt_irr_l_ptr lb);
extern void lccrt_irr_label_bind_code( lccrt_irr_l_ptr lb, lccrt_irr_c_ptr code);
extern void lccrt_irr_label_bind_node( lccrt_irr_l_ptr lb, lccrt_irr_n_ptr node);
extern lccrt_irr_jr_ptr lccrt_irr_node_get_jit_region( lccrt_irr_n_ptr node);
extern uint64_t lccrt_irr_node_get_ident( lccrt_irr_n_ptr node);
extern lccrt_irr_c_ptr lccrt_irr_node_get_code( lccrt_irr_n_ptr node);
extern void lccrt_irr_node_set_code( lccrt_irr_n_ptr node, lccrt_irr_c_ptr code);
extern uint64_t *lccrt_irr_node_get_count_addr( lccrt_irr_n_ptr node);
extern uint64_t lccrt_irr_node_get_attrs( lccrt_irr_n_ptr node);
extern uint64_t lccrt_irr_node_set_attrs( lccrt_irr_n_ptr node, uint64_t attrs);
extern int lccrt_irr_node_get_attr( lccrt_irr_n_ptr node, int index);
extern int lccrt_irr_node_set_attr( lccrt_irr_n_ptr node, int index, int attr);
extern void lccrt_irr_node_simplify( lccrt_irr_n_ptr node);
extern int lccrt_irr_code_get_num_relocs( lccrt_irr_c_ptr code);
extern lccrt_irr_jr_ptr lccrt_irr_code_get_jit_region( lccrt_irr_c_ptr code);
extern void lccrt_irr_code_set_phys_addr( lccrt_irr_c_ptr code, uint64_t addr, int64_t length);
extern uint64_t lccrt_irr_code_get_phys_addr( lccrt_irr_c_ptr code);
extern int64_t lccrt_irr_code_get_phys_len( lccrt_irr_c_ptr code);
extern void *lccrt_irr_code_get_relocs( lccrt_irr_c_ptr code);
extern void lccrt_irr_code_set_relocs( lccrt_irr_c_ptr code, void *relocs);
extern lccrt_irr_jr_ptr lccrt_irr_reloc_get_jit_region( lccrt_irr_rl_ptr reloc);
extern uint64_t lccrt_irr_reloc_get_virt_addr( lccrt_irr_rl_ptr reloc);
extern void lccrt_irr_reloc_set_phys_addr( lccrt_irr_rl_ptr reloc, uint64_t phys_addr);
extern void *lccrt_irr_reloc_get_link( lccrt_irr_rl_ptr reloc);
extern void lccrt_irr_reloc_set_link( lccrt_irr_rl_ptr reloc, void *link);
//extern void lccrt_irr_reloc_link( lccrt_irr_rl_ptr reloc, lccrt_irr_n_ptr node);
extern void lccrt_irr_reloc_unlink( lccrt_irr_rlu_ptr reloc_unit);

extern lccrt_irr_c_ptr lccrt_irr_node_compile_code( lccrt_irr_n_ptr node);
extern int lccrt_irr_node_decompile_code( lccrt_irr_n_ptr node);
extern int lccrt_irr_reloc_patch( lccrt_irr_rl_ptr reloc, lccrt_irr_c_ptr code);
extern int64_t lccrt_irr_node_execute( lccrt_irr_n_ptr node, lccrt_irr_execute_context_ptr ec);

extern int lccrt_irr_node_print( lccrt_irr_n_ptr node);
extern int lccrt_irr_oper_print( lccrt_irr_o_ptr oper, int is_ln);
extern int lccrt_irr_oper_fprint( FILE *fout, lccrt_irr_o_ptr oper, int is_ln);
extern int lccrt_irr_arg_fprint( FILE *fout, lccrt_irr_a_ptr a, int is_ln);
extern const char *lccrt_irr_oper_name_get_str( lccrt_irr_oper_name_t name);

#define lccrt_irr_emit_oper_r0a0( b, l, n) lccrt_irr_emit_oper_r1a2( b, l, n, 0, 0, 0)
#define lccrt_irr_emit_oper_r1a0( b, l, n, r) lccrt_irr_emit_oper_r1a2( b, l, n, r, 0, 0)
#define lccrt_irr_emit_oper_r0a1( b, l, n, a0) lccrt_irr_emit_oper_r1a2( b, l, n, 0, a0, 0)
#define lccrt_irr_emit_oper_r1a1( b, l, n, r, a0) lccrt_irr_emit_oper_r1a2( b, l, n, r, a0, 0)
#define lccrt_irr_emit_oper_r0a2( b, l, n, a0, a1) lccrt_irr_emit_oper_r1a2( b, l, n, 0, a0, a1)
extern int64_t lccrt_irr_emit_oper_r1a2( void *buffer, int64_t max_length, lccrt_irr_oper_name_t name,
                                         lccrt_irr_a_ptr r, lccrt_irr_a_ptr a0, lccrt_irr_a_ptr a1);
extern int64_t lccrt_irr_emit_bind_rel( void *inst, int64_t offset);

/**
 * Кодировка операции для Backend Bytecode (BB).
 *    name : 2
 *    num_args : 2
 *    arg_1
 *      type : 1
 *      value : 1,2,4,8
 *    ...
 *    arg_N
 *      ...
 *    num_attrs : 2
 *    attr_1
 *      size : 2
 *      type : 1
 *      value : ...
 *    ...
 *    attr_K
 *      ...
 * Если у операции есть результат, то он передается в качестве первого аргумента.
 * Имя операции задается значениями lccrt_bb_oper_name_t.
 * Тип аргумента задается значениями lccrt_bb_arg_type_t.
 * Если num_args = 0, то у операции нет аргументов.
 * Если num_attrs = 0, то у операции нет атрибутов.
 * Поле attr.size определяет байтовый размер поля attr.value, если attr.size = 0, то
 * это означает, что поле value отсутствует.
 * Тип атрибута задается значениями lccrt_bb_attr_type_t.
 */

/**
 * Тип аргумента операции Backend Bytecode (BB).
 */
typedef enum
{
    LCCRT_BB_ARG_TYPE_HEX8 = 0,
    LCCRT_BB_ARG_TYPE_HEX16,
    LCCRT_BB_ARG_TYPE_HEX32,
    LCCRT_BB_ARG_TYPE_HEX64,
    LCCRT_BB_ARG_TYPE_LABEL8 = 8,
    LCCRT_BB_ARG_TYPE_LABEL16,
    LCCRT_BB_ARG_TYPE_LABEL32,
    LCCRT_BB_ARG_TYPE_LABEL64,
    LCCRT_BB_ARG_TYPE_REG_SYS = 16,
    LCCRT_BB_ARG_TYPE_REG_GLOB,
    LCCRT_BB_ARG_TYPE_REG_PHYS,
    LCCRT_BB_ARG_TYPE_REG_VIRT,
} lccrt_bb_arg_type_t;

/**
 * Тип аргумента операции Backend Bytecode (BB).
 */
typedef enum
{
    LCCRT_BB_ATTR_TYPE_COUNTER,
    LCCRT_BB_ATTR_TYPE_LAST
} lccrt_bb_attr_type_t;

/**
 * Название операции Backend Bytecode (BB).
 */
typedef enum
{
    LCCRT_BB_OPER_NOP,
    /* Самая первая операция. */
    LCCRT_BB_OPER_START,
    /* Самая последняя операция. */
    LCCRT_BB_OPER_STOP,
    LCCRT_BB_OPER_MOV_U32,
    LCCRT_BB_OPER_MOV_U64,
    LCCRT_BB_OPER_EXT_S8,
    LCCRT_BB_OPER_EXT_S16,
    LCCRT_BB_OPER_EXT_S32,
    LCCRT_BB_OPER_CONV_S32_F32,
    LCCRT_BB_OPER_CONV_S32_F64,
    LCCRT_BB_OPER_CONV_S64_F32,
    LCCRT_BB_OPER_CONV_S64_F64,
    LCCRT_BB_OPER_CONV_F32_F64,
    LCCRT_BB_OPER_CONV_F64_F32,
    LCCRT_BB_OPER_AND_U64,
    LCCRT_BB_OPER_OR_U64,
    LCCRT_BB_OPER_XOR_U64,
    LCCRT_BB_OPER_SHL_U64,
    LCCRT_BB_OPER_SHR_U64,
    LCCRT_BB_OPER_SAR_U64,
    LCCRT_BB_OPER_ADD_U64,
    LCCRT_BB_OPER_SUB_U64,
    LCCRT_BB_OPER_MUL_U64,
    LCCRT_BB_OPER_SDIV_U64,
    LCCRT_BB_OPER_UDIV_U64,
    LCCRT_BB_OPER_SMOD_U64,
    LCCRT_BB_OPER_UMOD_U64,
    /* Взятие битового поля
         arg0 (int64) - источник битового поля
         arg1 (int64) - битовое смещение поля
         arg2 (int64) - битовая длина поля */
    LCCRT_BB_OPER_GETFIELD,
    LCCRT_BB_OPER_FADD_F32,
    LCCRT_BB_OPER_FADD_F64,
    LCCRT_BB_OPER_FSUB_F32,
    LCCRT_BB_OPER_FSUB_F64,
    LCCRT_BB_OPER_FMUL_F32,
    LCCRT_BB_OPER_FMUL_F64,
    LCCRT_BB_OPER_FDIV_F32,
    LCCRT_BB_OPER_FDIV_F64,
    LCCRT_BB_OPER_FSQRT_F64,
    LCCRT_BB_OPER_CMPE_U64,
    LCCRT_BB_OPER_CMPNE_U64,
    LCCRT_BB_OPER_CMPL_U64,
    LCCRT_BB_OPER_CMPLE_U64,
    LCCRT_BB_OPER_CMPB_U64,
    LCCRT_BB_OPER_CMPBE_U64,
    /* Сравнение формата float64 на "не упорядочено"
         arg0 (int64) - первый аргумент
         arg1 (int64) - второй аргумент
         res0 (int64) - (((bitcast float32/float64)arg0 vs (bitcast float32/float64)arg1)) */
    LCCRT_BB_OPER_FCMP_U_F32,
    LCCRT_BB_OPER_FCMP_U_F64,
    /* Сравнение формата float64 на "упорядочено"
         arg0 (int64) - первый аргумент
         arg1 (int64) - второй аргумент
         res0 (int64) - (((bitcast float32/float64)arg0 vs (bitcast float32/float64)arg1)) */
    LCCRT_BB_OPER_FCMP_O_F32,
    LCCRT_BB_OPER_FCMP_O_F64,
    /* Сравнение формата float64 на "равно или не упорядочено"
         arg0 (int64) - первый аргумент
         arg1 (int64) - второй аргумент
         res0 (int64) - (((bitcast float64)arg0 == (bitcast float64)arg1) || unordered) */
    LCCRT_BB_OPER_FCMPE_U_F64,
    /* Сравнение формата float64 на "равно"
         arg0 (int64) - первый аргумент
         arg1 (int64) - второй аргумент
         res0 (int64) - (((bitcast float32/float64)arg0 == (bitcast float32/float64)arg1)) */
    LCCRT_BB_OPER_FCMPE_O_F32,
    LCCRT_BB_OPER_FCMPE_O_F64,
    /* Сравнение формата float64 на "равно или не упорядочено"
         arg0 (int64) - первый аргумент
         arg1 (int64) - второй аргумент
         res0 (int64) - (((bitcast float64)arg0 < (bitcast float64)arg1) || unordered) */
    LCCRT_BB_OPER_FCMPL_U_F64,
    /* Сравнение формата float64 на "равно"
         arg0 (int64) - первый аргумент
         arg1 (int64) - второй аргумент
         res0 (int64) - (((bitcast float32/float64)arg0 < (bitcast float32/float64)arg1)) */
    LCCRT_BB_OPER_FCMPL_O_F32,
    LCCRT_BB_OPER_FCMPL_O_F64,
    /* Сравнение формата float64 на "равно или не упорядочено"
         arg0 (int64) - первый аргумент
         arg1 (int64) - второй аргумент
         res0 (int64) - (((bitcast float64)arg0 <= (bitcast float64)arg1) || unordered) */
    LCCRT_BB_OPER_FCMPLE_U_F64,
    /* Сравнение формата float64 на "равно"
         arg0 (int64) - первый аргумент
         arg1 (int64) - второй аргумент
         res0 (int64) - (((bitcast float32/float64)arg0 <= (bitcast float32/float64)arg1)) */
    LCCRT_BB_OPER_FCMPLE_O_F32,
    LCCRT_BB_OPER_FCMPLE_O_F64,
    /* Условная перессылка, если предикат true
         arg0 (int64) - итоговое значение, если arg1 == true
         arg1 (int64) - предикат выбора
         res0 (int64) - (if ( arg1 ) res0 = arg1;) */
    LCCRT_BB_OPER_MOVTHEN_U64,
    /* Условная перессылка, если предикат false
         arg0 (int64) - итоговое значение, если arg1 == false
         arg1 (int64) - предикат выбора
         res0 (int64) - (if ( !arg1 ) res0 = arg1;) */
    LCCRT_BB_OPER_MOVELSE_U64,
    LCCRT_BB_OPER_LD_U8,
    LCCRT_BB_OPER_LD_U16,
    LCCRT_BB_OPER_LD_U32,
    LCCRT_BB_OPER_LD_U64,
    LCCRT_BB_OPER_LD_S8,
    LCCRT_BB_OPER_LD_S16,
    LCCRT_BB_OPER_LD_S32,
    LCCRT_BB_OPER_ST_U8,
    LCCRT_BB_OPER_ST_U16,
    LCCRT_BB_OPER_ST_U32,
    LCCRT_BB_OPER_ST_U64,
    /* Безусловный переход по литералу "ленивой" линковки
         arg0 (reloc) - литерал перехода */
    LCCRT_BB_OPER_BRANCH_RELOC,
    /* Условный переход по литералам "ленивой" линковки
         arg0 (reloc) - литерал перехода, если arg2 == true
         arg1 (reloc) - литерал перехода, если arg2 == false
         arg2 (int64) - предикат перехода */
    LCCRT_BB_OPER_BRANCHIF_RELOC,
    LCCRT_BB_OPER_BRANCH_DYNAMIC,
    LCCRT_BB_OPER_CALL_V_U64U64,
    /* Адресуемая метка */
    LCCRT_BB_OPER_LABEL,
    LCCRT_BB_OPER_LAST
} lccrt_bb_oper_name_t;

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */

#endif /* LCCRT_H */
