#ifndef COMMAND__UTILS__H
#define COMMAND__UTILS__H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool check_size(size_t data_length, size_t expected_length) {
  if (data_length != expected_length) {
    printf("Sizes don't match %zu <> %zu", data_length, expected_length);
    return false;
  }
  return true;
}

#endif // COMMAND__UTILS__H