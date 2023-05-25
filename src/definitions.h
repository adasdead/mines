/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License.
 */

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "version.h"

/****************************************************************************
 * Project definitions
 ***************************************************************************/

#define WINDOW_NAME                     "Mines v"PROJECT_VERSION
#define WINDOW_SCALE                    33.75f

#define ASSETS_DIR                      "assets"

#define FIELD_LEFT_X                    1
#define FIELD_LEFT_Y                    4
#define FIELD_RIGHT_X                   FIELD_LEFT_X
#define FIELD_RIGHT_Y                   FIELD_LEFT_X

#define SMILE_LEFT_Y                    1
#define SMILE_OFFSET                    0.15f
#define SMILE_SCALE                     1.655f

#define COUNTER_LEFT_Y                  1
#define COUNTER_DIGITS                  3
#define COUNTER_OFFSET_LRX              0.5f
#define COUNTER_OFFSET_LY               0.25f
#define COUNTER_WIDTH                   0.82f
#define COUNTER_HEIGHT                  1.465f

#define BORDER_SEP_LEFT_Y               3

/****************************************************************************
 * Discord
 ***************************************************************************/

#define DISCORD_CLIENT_ID               1108520471139463300
#define DISCORD_ACTIVITY_NAME           "mines"

#endif /* DEFINITIONS_H */