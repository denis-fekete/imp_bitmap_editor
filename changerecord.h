#ifndef CHANGERECORD_H
#define CHANGERECORD_H

#include <cstdint>

typedef struct {
    uint8_t row;
    uint8_t col;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} ChangeRecord;

#endif // CHANGERECORD_H
