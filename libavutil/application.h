/*
 * copyright (c) 2016 Zhang Rui
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVUTIL_APPLICATION_H
#define AVUTIL_APPLICATION_H

#include "libavutil/log.h"

typedef struct AVApplicationContext AVApplicationContext;
struct AVApplicationContext {
    const AVClass *av_class;    /**< information for av_log(). Set by av_application_open(). */
    void *opaque;               /**< user data. */

    void (*func_did_tcp_connect_ip_port)(AVApplicationContext *h, int family, const char *ip, int port);
};

int  av_application_alloc(AVApplicationContext **ph, void *opaque);
int  av_application_open(AVApplicationContext **ph, void *opaque);
void av_application_close(AVApplicationContext *h);
void av_application_closep(AVApplicationContext **ph);

void av_application_did_tcp_connect_fd(AVApplicationContext *h, int fd);
void av_application_did_tcp_connect_ip_port(AVApplicationContext *h, int family, const char *ip, int port);

#endif /* AVUTIL_APPLICATION_H */
