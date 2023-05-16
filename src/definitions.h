/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License. (see the LICENSE file)
 */

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "version.h"

/****************************************************************************
 * Project definitions
 ***************************************************************************/

#define WINDOW_NAME                     "Minesweeper "PROJECT_VERSION_STRING
#define WINDOW_SCALE                    33.75f

#define ASSETS_DIR                      "assets"

#define FIELD_LX                        1
#define FIELD_LY                        4
#define FIELD_RX                        FIELD_LX
#define FIELD_RY                        FIELD_LX

#define SMILE_LY                        1
#define SMILE_OFFSET                    0.15f
#define SMILE_SCALE                     1.655f

#define COUNTER_LY                      1
#define COUNTER_NUMBERS                 3
#define COUNTER_OFFSET_LRX              0.5f
#define COUNTER_OFFSET_LY               0.25f
#define COUNTER_WIDTH                   0.82f
#define COUNTER_HEIGHT                  1.465f

#define BORDER_SEP_LY                   3

#endif /* DEFINITIONS_H */