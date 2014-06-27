/*
 * libavformat/ijkavfmsg.h
 *      extends in ijkplayer
 *
 * Copyright (c) 2014 Zhang Rui <bbcallen@gmail.com>
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
#ifndef AVFORMAT_IJKAVFMSG_H
#define AVFORMAT_IJKAVFMSG_H

typedef struct IJKFormatSegmentContext {
    /**
     * Segment position (0-based) in stream
     *
     * set by demux
     */
    int position;

    /**
     * Segment url, use url_free to free
     * 
     * set by application
     */
    char *url;

    /**
     * Callback function to free .url;
     *
     * set by application
     */
    void (*url_free)(void *url);
} IJKFormatSegmentContext;

/**
 * Resolve segment url from concatdec
 *
 * @param data      IJKFormatSegmentContext pointer
 * @param data_size size of data;
 * @return 0 if OK, AVERROR_xxx otherwise
 */
#define IJKAVF_CM_RESOLVE_SEGMENT 0x10001

#endif /* AVFORMAT_IJKAVFMSG_H */
