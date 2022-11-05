#ifndef HAL__UNIQUE_ID_H
#define HAL__UNIQUE_ID_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct board_id_t board_id_t;

typedef board_id_t *board_id_handle_t;

board_id_handle_t get_unique_board_id();

#ifdef __cplusplus
}
#endif
#endif // HAL__UNIQUE_ID_H