/* GStreamer
 * Copyright (C) 2009 Wim Taymans <wim.taymans@gmail.be>
 *
 * gstmeta.h: Header for Metadata structures
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#ifndef __GST_META_H__
#define __GST_META_H__

G_BEGIN_DECLS

typedef struct _GstMeta GstMeta;
typedef struct _GstMetaInfo GstMetaInfo;

#define GST_META_CAST(meta)   ((GstMeta *)(meta))

/**
 * GstMetaFlags:
 * @GST_META_FLAG_NONE: no flags
 * @GST_META_FLAG_READONLY: metadata should not be modified
 * @GST_META_FLAG_POOLED: metadata is managed by a bufferpool and should not
 *    be removed
 * @GST_META_FLAG_LAST: additional flags can be added starting from this flag.
 *
 * Extra metadata flags.
 */
typedef enum {
  GST_META_FLAG_NONE        = 0,
  GST_META_FLAG_READONLY    = (1 << 0),
  GST_META_FLAG_POOLED      = (1 << 1),

  GST_META_FLAG_LAST        = (1 << 16)
} GstMetaFlags;

/**
 * GST_META_FLAGS:
 * @meta: a #GstMeta.
 *
 * A flags word containing #GstMetaFlag flags set on @meta
 */
#define GST_META_FLAGS(meta)  (GST_META_CAST (meta)->flags)
/**
 * GST_META_FLAG_IS_SET:
 * @meta: a #GstMeta.
 * @flag: the #GstMetaFlag to check.
 *
 * Gives the status of a specific flag on a metadata.
 */
#define GST_META_FLAG_IS_SET(meta,flag)        !!(GST_META_FLAGS (meta) & (flag))
/**
 * GST_META_FLAG_SET:
 * @meta: a #GstMeta.
 * @flag: the #GstMetaFlag to set.
 *
 * Sets a metadata flag on a metadata.
 */
#define GST_META_FLAG_SET(meta,flag)           (GST_META_FLAGS (meta) |= (flag))
/**
 * GST_META_FLAG_UNSET:
 * @meta: a #GstMeta.
 * @flag: the #GstMetaFlag to clear.
 *
 * Clears a metadata flag.
 */
#define GST_META_FLAG_UNSET(meta,flag)         (GST_META_FLAGS (meta) &= ~(flag))

/**
 * GstMeta:
 * @flags: extra flags for the metadata
 * @info: pointer to the #GstMetaInfo
 *
 * Base structure for metadata. Custom metadata will put this structure
 * as the first member of their structure.
 */
struct _GstMeta {
  GstMetaFlags       flags;
  const GstMetaInfo *info;
};

/**
 * GstMetaInitFunction:
 * @meta: a #GstMeta
 * @params: parameters passed to the init function
 * @buffer: a #GstBuffer
 *
 * Function called when @meta is initialized in @buffer.
 */
typedef gboolean (*GstMetaInitFunction) (GstMeta *meta, gpointer params, GstBuffer *buffer);

/**
 * GstMetaFreeFunction:
 * @meta: a #GstMeta
 * @buffer: a #GstBuffer
 *
 * Function called when @meta is freed in @buffer.
 */
typedef void (*GstMetaFreeFunction)     (GstMeta *meta, GstBuffer *buffer);

/**
 * GstMetaCopyFunction:
 * @dest: a destination #GstBuffer
 * @meta: a #GstMeta
 * @buffer: a #GstBuffer
 * @offset: an offset
 * @size: a size
 *
 * Function called when the region at @offset and @size in @buffer is copied
 * into @dest. The function should update the metadata on @dest using @meta.
 */
typedef void (*GstMetaCopyFunction)     (GstBuffer *dest, GstMeta *meta,
                                         GstBuffer *buffer, gsize offset, gsize size);
/**
 * GstMetaTransformFunction:
 * @transbuf: a #GstBuffer
 * @meta: a #GstMeta
 * @buffer: a #GstBuffer
 * @data: transform specific data.
 *
 * Function called for each @meta in @buffer as a result of performing a
 * transformation on @transbuf. Additional type specific transform data
 * is passed to the function.
 *
 * Implementations should check the type of the transform @data and parse
 * additional type specific field that should be used to perform the transform.
 */
typedef void (*GstMetaTransformFunction) (GstBuffer *transbuf, GstMeta *meta,
                                          GstBuffer *buffer, gpointer data);

/**
 * GstMetaInfo:
 * @api: tag indentifying the metadata structure and api
 * @type: type indentifying the implementor of the api
 * @size: size of the metadata
 * @init_func: function for initializing the metadata
 * @free_func: function for freeing the metadata
 * @copy_func: function for copying the metadata
 * @transform_func: function for transforming the metadata
 *
 * The #GstMetaInfo provides information about a specific metadata
 * structure.
 */
struct _GstMetaInfo {
  GQuark                     api;
  GType                      type;
  gsize                      size;

  GstMetaInitFunction        init_func;
  GstMetaFreeFunction        free_func;
  GstMetaCopyFunction        copy_func;
  GstMetaTransformFunction   transform_func;

  /*< private >*/
  gpointer _gst_reserved[GST_PADDING];
};

const GstMetaInfo *  gst_meta_register        (const gchar *api, const gchar *impl,
                                               gsize size,
                                               GstMetaInitFunction        init_func,
                                               GstMetaFreeFunction        free_func,
                                               GstMetaCopyFunction        copy_func,
                                               GstMetaTransformFunction   transform_func);
const GstMetaInfo *  gst_meta_get_info        (const gchar * impl);

G_END_DECLS

#endif /* __GST_META_H__ */