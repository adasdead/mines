/* 
 * Copyright (c) 2023 adasdead
 * This software is licensed under the MIT License.
 */

#include "discord.h"

#include <stdio.h>

#include <discord_game_sdk.h>

#include "util/logger.h"

#include "definitions.h"

#define BUF_SIZE                        128

#if _MSC_VER
#pragma warning(disable : 4996)
#endif

struct IDiscordCore *core = NULL;

struct IDiscordActivityManager *manager = NULL;

static void discord_activity_callback(void *data,
                                      enum EDiscordResult result)
{
    if (result == DiscordResult_Ok) {
        logger_info("Activity updated");
    }
    else {
        logger_warn("Activity update failed");
    }
}

void discord_initialize(void)
{
    enum EDiscordResult result;
    struct DiscordCreateParams params;

    params.client_id = DISCORD_CLIENT_ID;
    params.flags = DiscordCreateFlags_Default;

    result = DiscordCreate(DISCORD_VERSION, &params, &core);

    if (result == DiscordResult_Ok) {
        manager = core->get_activity_manager(core);
        logger_info("Discord SDK initialized!");
    }
}

void discord_update_activity(const char *details, const char *state,
                             time_t start_time)
{
    struct DiscordActivity activity = {0};

    snprintf(activity.name, BUF_SIZE, "%s", DISCORD_ACTIVITY_NAME);
    snprintf(activity.assets.large_image, BUF_SIZE, "%s", "icon");
    snprintf(activity.assets.large_text, BUF_SIZE, "%s", "icon");

    activity.application_id = DISCORD_CLIENT_ID;
    activity.type = DiscordActivityType_Playing;
    activity.timestamps.start = (DiscordTimestamp) start_time;

    if (state != NULL)
        snprintf(activity.state, BUF_SIZE, "%s", state);

    if (details != NULL)
        snprintf(activity.details, BUF_SIZE, "%s", details);

    if (core != NULL) {
        manager->update_activity(manager, &activity, NULL,
                                 discord_activity_callback);
    }
}

void discord_run_callbacks(void)
{
    if (core != NULL)
        core->run_callbacks(core);
}

void discord_free(void)
{
    if (core != NULL)
        core->destroy(core);
}