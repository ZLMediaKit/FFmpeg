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

#include "application.h"
#include "libavformat/network.h"
#include "libavutil/avstring.h"

int av_application_alloc(AVApplicationContext **ph, void *opaque)
{
    AVApplicationContext *h = NULL;

    h = av_mallocz(sizeof(AVApplicationContext));
    if (!h)
        return AVERROR(ENOMEM);

    h->opaque = opaque;

    *ph = h;
    return 0;
}

int av_application_open(AVApplicationContext **ph, void *opaque)
{
    int ret = av_application_alloc(ph, opaque);
    if (ret)
        return ret;

    return 0;
}

void av_application_close(AVApplicationContext *h)
{
    av_free(h);
}

void av_application_closep(AVApplicationContext **ph)
{
    if (!ph || !*ph)
        return;

    av_application_close(*ph);
    *ph = NULL;
}

void av_application_did_tcp_connect_fd(AVApplicationContext *h, int error, int fd)
{
    struct sockaddr_storage so_stg;
    int       ret = 0;
    socklen_t so_len = sizeof(so_stg);
    int       so_family;
    // char      so_ip_name[FFMAX(INET4_ADDRSTRLEN, INET6_ADDRSTRLEN)];
    char      so_ip_name[96] = {0};

    if (!h || !h->func_did_tcp_connect_ip_port || fd <= 0)
        return;

    ret = getpeername(fd, (struct sockaddr *)&so_stg, &so_len);
    if (ret)
        return;

    so_family = ((struct sockaddr*)&so_stg)->sa_family;
    switch (so_family) {
        case AF_INET: {
            struct sockaddr_in* in4 = (struct sockaddr_in*)&so_stg;
            if (inet_ntop(AF_INET, &(in4->sin_addr), so_ip_name, sizeof(so_ip_name)))
                av_application_did_tcp_connect_ip_port(h, error, AF_INET, so_ip_name, in4->sin_port);
            break;
        }
        case AF_INET6: {
            struct sockaddr_in6* in6 = (struct sockaddr_in6*)&so_stg;
            if (inet_ntop(AF_INET6, &(in6->sin6_addr), so_ip_name, sizeof(so_ip_name)))
                av_application_did_tcp_connect_ip_port(h, error, AF_INET6, so_ip_name, in6->sin6_port);
            break;
        }
    }
}

void av_application_did_tcp_connect_ip_port(AVApplicationContext *h, int error, int family, const char *ip, int port)
{
    if (h && h->func_did_tcp_connect_ip_port)
        h->func_did_tcp_connect_ip_port(h, error, family, ip, port);
}

void av_application_on_http_event(AVApplicationContext *h, AVAppHttpEvent *event)
{
    if (h && h->func_on_http_event)
        h->func_on_http_event(h, event);
}

void av_application_will_http_open(AVApplicationContext *h, void *obj, const char *url)
{
    AVAppHttpEvent event = {0};

    if (!h || !obj || !url)
        return;

    event.event_type = AVAPP_EVENT_WILL_HTTP_OPEN;
    event.obj        = obj;
    av_strlcpy(event.url, url, sizeof(event.url));

    av_application_on_http_event(h, &event);
}

void av_application_did_http_open(AVApplicationContext *h, void *obj, const char *url, int error, int http_code)
{
    AVAppHttpEvent event = {0};

    if (!h || !obj || !url)
        return;

    event.event_type = AVAPP_EVENT_DID_HTTP_OPEN;
    event.obj        = obj;
    av_strlcpy(event.url, url, sizeof(event.url));
    event.error     = error;
    event.http_code = http_code;

    av_application_on_http_event(h, &event);
}

void av_application_will_http_seek(AVApplicationContext *h, void *obj, const char *url, int64_t offset)
{
    AVAppHttpEvent event = {0};

    if (!h || !obj || !url)
        return;

    event.event_type = AVAPP_EVENT_WILL_HTTP_SEEK;
    event.obj        = obj;
    event.offset     = offset;
    av_strlcpy(event.url, url, sizeof(event.url));

    av_application_on_http_event(h, &event);
}

void av_application_did_http_seek(AVApplicationContext *h, void *obj, const char *url, int64_t offset, int error, int http_code)
{
    AVAppHttpEvent event = {0};

    if (!h || !obj || !url)
        return;

    event.event_type = AVAPP_EVENT_DID_HTTP_SEEK;
    event.obj        = obj;
    event.offset     = offset;
    av_strlcpy(event.url, url, sizeof(event.url));
    event.error     = error;
    event.http_code = http_code;

    av_application_on_http_event(h, &event);
}

void av_application_on_io_traffic(AVApplicationContext *h, AVAppIOTraffic *event)
{
    if (h && h->func_on_io_traffic)
        h->func_on_io_traffic(h, event);
}

void av_application_did_io_tcp_read(AVApplicationContext *h, void *obj, int bytes)
{
    AVAppIOTraffic event = {0};

    if (!h || !obj || bytes <= 0)
        return;

    event.event_type = AVAPP_EVENT_DID_TCP_READ;
    event.obj        = obj;
    event.bytes      = bytes;

    av_application_on_io_traffic(h, &event);
}
