/*
 * Copyright (C) 2013 The Android Open Source Project
 * Copyright (C) 2015 The CyanogenMod Project
 * Copyright (C) 2018 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <healthd/healthd.h>
#include <fcntl.h>
#include <cutils/klog.h>

#define BACKLIGHT_ON_LEVEL        100
#define BACKLIGHT_OFF_LEVEL       0
#define BACKLIGHT_PATH            "/sys/class/leds/lcd-backlight/brightness"

#define LOGE(x...) do { KLOG_ERROR("charger", x); } while (0)
#define LOGW(x...) do { KLOG_WARNING("charger", x); } while (0)
#define LOGV(x...) do { KLOG_DEBUG("charger", x); } while (0)

void healthd_board_init(struct healthd_config*)
{

}

int healthd_board_battery_update(struct android::BatteryProperties*)
{
    // return 0 to log periodic polled battery status to kernel log
    return 1;
}

void healthd_board_mode_charger_set_backlight(bool en)
{
    int fd;
    char buffer[10];

    memset(buffer, '\0', sizeof(buffer));
    fd = open(BACKLIGHT_PATH, O_RDWR);
    if (fd < 0) {
        LOGE("Could not open backlight node : %s\n", strerror(errno));
        goto cleanup;
    }
    LOGV("set backlight status to %d\n", en);
    if (en)
        snprintf(buffer, sizeof(buffer), "%d\n", BACKLIGHT_ON_LEVEL);
    else
        snprintf(buffer, sizeof(buffer), "%d\n", BACKLIGHT_OFF_LEVEL);

    if (write(fd, buffer,strlen(buffer)) < 0) {
        LOGE("Could not write to backlight node : %s\n", strerror(errno));
        goto cleanup;
    }
cleanup:
    if (fd >= 0)
        close(fd);
}
