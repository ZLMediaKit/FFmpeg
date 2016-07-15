/*
 * utils.c
 *
 * Copyright (c) 2003 Fabrice Bellard
 * Copyright (c) 2013 Zhang Rui <bbcallen@gmail.com>
 *
 * This file is part of ijkPlayer.
 *
 * ijkPlayer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * ijkPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with ijkPlayer; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdlib.h>
#include "url.h"
#include "ijkavformat.h"

static IjkAVInjectCallback s_av_inject_callback = NULL;

IjkAVInjectCallback ijkav_register_inject_callback(IjkAVInjectCallback callback)
{
    IjkAVInjectCallback prev_callback = s_av_inject_callback;
    s_av_inject_callback = callback;
    return prev_callback;
}

IjkAVInjectCallback ijkav_get_inject_callback(void)
{
    return s_av_inject_callback;
}

static int ijkmds_open(URLContext *h, const char *arg, int flags, AVDictionary **options)
{
    return -1;
}

static const AVClass ijkmediadatasource_context_class = {
    .class_name = "IjkMediaDataSource",
    .item_name  = av_default_item_name,
    .version    = LIBAVUTIL_VERSION_INT,
};

URLProtocol ff_ijkmediadatasource_protocol = {
    .name                = "ijkmediadatasource",
    .url_open2           = ijkmds_open,
    .priv_data_size      = 1,
    .priv_data_class     = &ijkmediadatasource_context_class,
};

int ijkav_register_ijkmediadatasource_protocol(URLProtocol *protocol, int protocol_size)
{
    if (protocol_size != sizeof(URLProtocol)) {
        av_log(NULL, AV_LOG_ERROR, "ijkav_register_ijkmediadatasource_protocol: ABI mismatch.\n");
        return -1;
    }
    memcpy(&ff_ijkmediadatasource_protocol, protocol, protocol_size);
    return 0;
}
