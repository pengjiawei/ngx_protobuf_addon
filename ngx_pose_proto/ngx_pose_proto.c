/* Generated by protobuf-nginx 1.1 - DO NOT EDIT */

#include <ngx_protobuf.h>
#include "ngx_pose_proto.h"

/* Empty field descriptors */

/* Pose field descriptors */

ngx_protobuf_field_descriptor_t
ngx_pose_x = {
    ngx_string("x"),
    1,
    NGX_PROTOBUF_WIRETYPE_VARINT,
    NGX_PROTOBUF_LABEL_REQUIRED,
    NGX_PROTOBUF_TYPE_UINT32,
    sizeof(uint32_t),
    0,
    (ngx_protobuf_pack_pt)
    NULL,
    (ngx_protobuf_unpack_pt)
    NULL,
    (ngx_protobuf_size_pt)
    NULL
};

ngx_protobuf_field_descriptor_t
ngx_pose_y = {
    ngx_string("y"),
    2,
    NGX_PROTOBUF_WIRETYPE_VARINT,
    NGX_PROTOBUF_LABEL_REQUIRED,
    NGX_PROTOBUF_TYPE_UINT32,
    sizeof(uint32_t),
    0,
    (ngx_protobuf_pack_pt)
    NULL,
    (ngx_protobuf_unpack_pt)
    NULL,
    (ngx_protobuf_size_pt)
    NULL
};

static ngx_protobuf_module_t
ngx_pose_proto_module_ctx = {
    NULL
};

ngx_module_t ngx_pose_proto_module = {
    NGX_MODULE_V1,
    &ngx_pose_proto_module_ctx,
    NULL,
    NGX_PROTOBUF_MODULE,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NGX_MODULE_V1_PADDING
};

/* Empty message methods */

ngx_int_t
ngx_empty__is_initialized(
    ngx_empty_t *obj)
{
    return 1; /* no required fields */
}

ngx_int_t
ngx_empty__unpack(
    ngx_empty_t *obj,
    ngx_protobuf_context_t *ctx)
{
    u_char      **pos = &ctx->buffer.pos;
    u_char       *end = ctx->buffer.last;
    uint32_t      header;
    uint32_t      field;
    uint32_t      wire;
    ngx_int_t     rc;

    while (*pos < end) {
        if (ngx_protobuf_read_uint32(pos, end, &header) != NGX_OK) {
            return NGX_ABORT;
        }

        if (*pos >= end) {
            return NGX_ABORT;
        }

        field = header >> 3;
        wire = header & 0x07;

        switch (field) {
        default:
            rc = ngx_protobuf_unpack_unknown_field(field, wire, ctx,
                &obj->__unknown);
            if (rc != NGX_OK) {
                return rc;
            }
            break;
        }
    }

    return NGX_OK;
}

size_t
ngx_empty__size(
     ngx_empty_t *obj)
{
    size_t      size = 0;
    ngx_uint_t  i;


    if (obj->__unknown != NULL
        && obj->__unknown->elts != NULL
        && obj->__unknown->nelts > 0)
    {
        ngx_protobuf_unknown_field_t *unk = obj->__unknown->elts;

        for (i = 0; i < obj->__unknown->nelts; ++i) {
            size += ngx_protobuf_size_unknown_field(unk + i);
        }
    }

    return size;
}

ngx_int_t
ngx_empty__pack(
    ngx_empty_t *obj,
    ngx_protobuf_context_t *ctx)
{
    size_t     size = ngx_empty__size(obj);
    ngx_int_t  rc;

    if (size == 0) {
        return NGX_OK;
    }

    if (ctx->buffer.pos + size > ctx->buffer.last) {
        return NGX_ABORT;
    }


    if (obj->__unknown != NULL
        && obj->__unknown->elts != NULL
        && obj->__unknown->nelts > 0)
    {
        ngx_protobuf_unknown_field_t  *unk = obj->__unknown->elts;
        ngx_uint_t                     i;

        for (i = 0; i < obj->__unknown->nelts; ++i) {
            rc = ngx_protobuf_pack_unknown_field(unk + i, ctx);
            if (rc != NGX_OK) {
                return rc;
            }
        }
    }

    return NGX_OK;
}

/* Pose message methods */

ngx_int_t
ngx_pose__is_initialized(
    ngx_pose_t *obj)
{
    return (ngx_pose__has_x(obj)
        && ngx_pose__has_y(obj));
}

ngx_int_t
ngx_pose__unpack(
    ngx_pose_t *obj,
    ngx_protobuf_context_t *ctx)
{
    u_char      **pos = &ctx->buffer.pos;
    u_char       *end = ctx->buffer.last;
    uint32_t      header;
    uint32_t      field;
    uint32_t      wire;
    ngx_int_t     rc;

    while (*pos < end) {
        if (ngx_protobuf_read_uint32(pos, end, &header) != NGX_OK) {
            return NGX_ABORT;
        }

        if (*pos >= end) {
            return NGX_ABORT;
        }

        field = header >> 3;
        wire = header & 0x07;

        switch (field) {
        case 1: /* Pose.x */
            if (wire == NGX_PROTOBUF_WIRETYPE_VARINT) {
                if (ngx_protobuf_read_uint32(pos, end,
                    &obj->x) != NGX_OK)
                {
                    return NGX_ABORT;
                }
                obj->__has_x = 1;
            } else {
                if (ngx_protobuf_skip(pos, end, wire) != NGX_OK) {
                    return NGX_ABORT;
                }
            }
            break;
        case 2: /* Pose.y */
            if (wire == NGX_PROTOBUF_WIRETYPE_VARINT) {
                if (ngx_protobuf_read_uint32(pos, end,
                    &obj->y) != NGX_OK)
                {
                    return NGX_ABORT;
                }
                obj->__has_y = 1;
            } else {
                if (ngx_protobuf_skip(pos, end, wire) != NGX_OK) {
                    return NGX_ABORT;
                }
            }
            break;
        default:
            rc = ngx_protobuf_unpack_unknown_field(field, wire, ctx,
                &obj->__unknown);
            if (rc != NGX_OK) {
                return rc;
            }
            break;
        }
    }

    return NGX_OK;
}

size_t
ngx_pose__size(
     ngx_pose_t *obj)
{
    size_t      size = 0;
    ngx_uint_t  i;

    if (obj->__has_x) {
        size += ngx_protobuf_size_uint32_field(obj->x, 1);
    }
    if (obj->__has_y) {
        size += ngx_protobuf_size_uint32_field(obj->y, 2);
    }

    if (obj->__unknown != NULL
        && obj->__unknown->elts != NULL
        && obj->__unknown->nelts > 0)
    {
        ngx_protobuf_unknown_field_t *unk = obj->__unknown->elts;

        for (i = 0; i < obj->__unknown->nelts; ++i) {
            size += ngx_protobuf_size_unknown_field(unk + i);
        }
    }

    return size;
}

ngx_int_t
ngx_pose__pack(
    ngx_pose_t *obj,
    ngx_protobuf_context_t *ctx)
{
    size_t     size = ngx_pose__size(obj);
    ngx_int_t  rc;

    if (size == 0) {
        return NGX_OK;
    }

    if (ctx->buffer.pos + size > ctx->buffer.last) {
        return NGX_ABORT;
    }

    if (obj->__has_x) {
        ctx->buffer.pos = ngx_protobuf_write_uint32_field(
            ctx->buffer.pos,
            obj->x,
            1);
    }
    if (obj->__has_y) {
        ctx->buffer.pos = ngx_protobuf_write_uint32_field(
            ctx->buffer.pos,
            obj->y,
            2);
    }

    if (obj->__unknown != NULL
        && obj->__unknown->elts != NULL
        && obj->__unknown->nelts > 0)
    {
        ngx_protobuf_unknown_field_t  *unk = obj->__unknown->elts;
        ngx_uint_t                     i;

        for (i = 0; i < obj->__unknown->nelts; ++i) {
            rc = ngx_protobuf_pack_unknown_field(unk + i, ctx);
            if (rc != NGX_OK) {
                return rc;
            }
        }
    }

    return NGX_OK;
}

