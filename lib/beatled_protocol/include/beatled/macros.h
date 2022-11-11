#ifndef BEATLED_PROTOCOL__MACROS_H
#define BEATLED_PROTOCOL__MACROS_H

#define deserialize_msg(fn_name, msg_t)                                        \
  int fn_name(beatled_message_t *msg_data, size_t data_length,                 \
              process_message_fn_t *process_fn_map) {                          \
    if (data_length != sizeof(msg_t)) {                                        \
      printf("Error sizes don't match %zu, %zu", buf_len, sizeof(msg_t));      \
      return 1;                                                                \
    }                                                                          \
    process_fn_map(buffer_payload);                                            \
  }

#if defined(__ICCARM__)

#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) x
#define PACK_STRUCT_USE_INCLUDES

#elif defined(__CC_ARM)

#define PACK_STRUCT_BEGIN __packed
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) x

#elif defined(__GNUC__)

#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT __attribute__((__packed__))
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) x

#elif defined(__TASKING__)

#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) x

#endif

#ifndef PACK_STRUCT_FLD_8
#define PACK_STRUCT_FLD_8(x) PACK_STRUCT_FIELD(x)
#endif /* PACK_STRUCT_FLD_8 */

#endif // BEATLED_PROTOCOL__MACROS_H
