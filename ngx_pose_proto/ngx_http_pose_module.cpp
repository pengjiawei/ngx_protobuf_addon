
extern "C" {

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include "ngx_pose_proto.h"
#include <stdio.h>
#include <stdlib.h>
static char *
ngx_http_pose(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_int_t ngx_http_pose_handler(ngx_http_request_t *r);

}
using namespace std;

static ngx_command_t ngx_http_pose_commands[] =
        {

                {
                        ngx_string("pose"),
                        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LMT_CONF |
                        NGX_CONF_NOARGS,
                        ngx_http_pose,
                        NGX_HTTP_LOC_CONF_OFFSET,
                        0,
                        NULL
                },

                ngx_null_command
        };

static ngx_http_module_t ngx_http_pose_module_ctx =
        {
                NULL,                              /* preconfiguration */
                NULL,                       /* postconfiguration */

                NULL,                              /* create main configuration */
                NULL,                              /* init main configuration */

                NULL,                              /* create server configuration */
                NULL,                              /* merge server configuration */

                NULL,                   /* create location configuration */
                NULL                    /* merge location configuration */
        };

ngx_module_t ngx_http_pose_module =
        {
                NGX_MODULE_V1,
                &ngx_http_pose_module_ctx,           /* module context */
                ngx_http_pose_commands,              /* module directives */
                NGX_HTTP_MODULE,                       /* module type */
                NULL,                                  /* init master */
                NULL,                                  /* init module */
                NULL,                                  /* init process */
                NULL,                                  /* init thread */
                NULL,                                  /* exit thread */
                NULL,                                  /* exit process */
                NULL,                                  /* exit master */
                NGX_MODULE_V1_PADDING
        };


static char *
ngx_http_pose(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_http_core_loc_conf_t *clcf;

    //首先找到mytest配置项所属的配置块，clcf貌似是location块内的数据
//结构，其实不然，它可以是main、srv或者loc级别配置项，也就是说在每个
//http{}和server{}内也都有一个ngx_http_core_loc_conf_t结构体
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);

    //http框架在处理用户请求进行到NGX_HTTP_CONTENT_PHASE阶段时，如果
//请求的主机域名、URI与mytest配置项所在的配置块相匹配，就将调用我们
//实现的ngx_http_mytest_handler方法处理这个请求
    clcf->handler = ngx_http_pose_handler;

    return NGX_CONF_OK;
}

ngx_pose_t *unpack_pose(ngx_str_t *val, ngx_pool_t *pool) {
    ngx_protobuf_context_t ctx;
    ngx_pose_t *pose;
    pose = ngx_pose__alloc(pool);
    if (pose == NULL) {
        return NULL;
    }
    ctx.pool = pool;
    ctx.buffer.start = val->data;
    ctx.buffer.pos = ctx.buffer.start;
    ctx.buffer.last = val->data + val->len;
    ctx.reuse_strings = 1;

    if (ngx_pose__unpack(pose, &ctx) != NGX_OK) {
        return NULL;
    }
    return pose;
}

ngx_int_t pack_pose(ngx_pose_t *pose, ngx_str_t *output, ngx_pool_t *pool, FILE *file) {
    FILE *fout;
    fout = fopen("/tmp/nginx_pack_log", "a+tc");
    fprintf(fout, "in pack pose!\n");
    fflush(fout);
    ngx_protobuf_context_t ctx;
    size_t size;
    ngx_int_t rc;

    fprintf(fout, "pose x = %d\n", pose->x);
    fflush(fout);
    size = ngx_pose__size(pose);
    if (size == 0) {
        return NGX_DECLINED;
    }
    fprintf(fout, "pose size = %d\n", size);
    fflush(fout);
    output->data = ngx_palloc(pool, size);
    if (output->data == NULL) {
        fprintf(fout, "output->data is null\n");
        fflush(fout);
        return NGX_ERROR;
    }

    ctx.pool = pool;
    ctx.buffer.start = output->data;
    ctx.buffer.pos = ctx.buffer.start;
    ctx.buffer.last = ctx.buffer.start + size;

    rc = ngx_pose__pack(pose, &ctx);

    output->len = ctx.buffer.pos - ctx.buffer.start;
    u_char cc = *(output->data + output->len);
    fprintf(fout, "pack rc = %d,output->len = %d output->data = %c\n", rc, output->len, cc);
    fflush(fout);
    return rc;
}


static ngx_int_t ngx_http_pose_handler(ngx_http_request_t *r) {
    //必须是GET或者HEAD方法，否则返回405 Not Allowed
    if (!(r->method & (NGX_HTTP_GET | NGX_HTTP_HEAD))) {
        return NGX_HTTP_NOT_ALLOWED;
    }

    //丢弃请求中的包体
    ngx_int_t rc = ngx_http_discard_request_body(r);
    if (rc != NGX_OK) {
        return rc;
    }

    FILE *fout;
    fout = fopen("/tmp/nginx_pose_log", "a+tc");
    fprintf(fout, "m->start process!\n");
    fflush(fout);

    ngx_pose_t *pose = ngx_pose__alloc(r->pool);
    fprintf(fout, "construct_pose\n");
    ngx_pose__set_x(pose, 1);
    ngx_pose__set_y(pose, 2);

    fprintf(fout, "pose has x = %d,has y =%d,size = %d\n", pose->__has_x, pose->__has_y, ngx_pose__size(pose));
    ngx_str_t str = ngx_null_string;
    ngx_str_t *output = &str;
    fprintf(fout, "pack pose\n");
    fflush(fout);
    pack_pose(pose, output, r->pool, fout);


    ngx_str_t test_t = ngx_string("hello, this is nginx");
    // output = &test_t;
    fprintf(fout, "output len = %d data = %c\n", output->len, *(output->data + output->len));

    //设置返回的Content-Type。注意，ngx_str_t有一个很方便的初始化宏
//ngx_string，它可以把ngx_str_t的data和len成员都设置好
    ngx_str_t type = ngx_string("text/plain");
    //设置Content-Type
    r->headers_out.content_type = type;
    //set length of msg
    r->headers_out.content_length_n = output->len;
    //设置返回状态码
    r->headers_out.status = NGX_HTTP_OK;

    rc = ngx_http_send_header(r);

    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
        fprintf(fout, "send header error\n");
        return rc;
    }

    ngx_buf_t *b;
    b = ngx_create_temp_buf(r->pool, output->len);
    if (b == NULL) {
        fprintf(fout, "ngx_buf_t is null\n");
        fflush(fout);
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    ngx_memcpy(b->pos, output->data, output->len);
    b->last = b->pos + output->len;
    b->last_buf = 1;

    //构造发送时的ngx_chain_t结构体
    ngx_chain_t out;
    //赋值ngx_buf_t
    out.buf = b;
    //设置next为NULL
    out.next = NULL;

    //最后一步发送包体，http框架会调用ngx_http_finalize_request方法
//结束请求
    fprintf(fout, "m->end process!\n");
    fflush(fout);
    fclose(fout);

    return ngx_http_output_filter(r, &out);

}
