#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

extern "C" {
#include "ngx_map_proto.h"
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include <stdio.h>
#include <stdlib.h>


  static char *
  ngx_http_mymap(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

  static ngx_int_t ngx_http_mymap_handler(ngx_http_request_t *r);

}
using namespace std;
//static char *
//ngx_http_mytest(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

//static ngx_int_t ngx_http_mytest_handler(ngx_http_request_t *r);

static ngx_command_t  ngx_http_mymap_commands[] =
{

  {
    ngx_string("mymap"),
    NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LMT_CONF | NGX_CONF_NOARGS,
    ngx_http_mymap,
    NGX_HTTP_LOC_CONF_OFFSET,
    0,
    NULL
  },

  ngx_null_command
};

static ngx_http_module_t  ngx_http_mymap_module_ctx =
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

ngx_module_t  ngx_http_mymap_module =
{
  NGX_MODULE_V1,
  &ngx_http_mymap_module_ctx,           /* module context */
  ngx_http_mymap_commands,              /* module directives */
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
ngx_http_mymap(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
  ngx_http_core_loc_conf_t  *clcf;

  //首先找到mytest配置项所属的配置块，clcf貌似是location块内的数据
//结构，其实不然，它可以是main、srv或者loc级别配置项，也就是说在每个
//http{}和server{}内也都有一个ngx_http_core_loc_conf_t结构体
  clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);

  //http框架在处理用户请求进行到NGX_HTTP_CONTENT_PHASE阶段时，如果
//请求的主机域名、URI与mytest配置项所在的配置块相匹配，就将调用我们
//实现的ngx_http_mytest_handler方法处理这个请求
  clcf->handler = ngx_http_mymap_handler;

  return NGX_CONF_OK;
}

ngx_map_t* unpack_map(ngx_str_t* val, ngx_pool_t* pool) {
  ngx_protobuf_context_t ctx;
  ngx_map_t* map;
  map = ngx_map__alloc(pool);
  if (map == NULL) {
    return NULL;
  }
  ctx.pool = pool;
  ctx.buffer.start = val->data;
  ctx.buffer.pos = ctx.buffer.start;
  ctx.buffer.last = val->data + val->len;
  ctx.reuse_strings = 1;

  if ( ngx_map__unpack(map, &ctx) != NGX_OK ) {
    return NULL;
  }
  return map;
}

ngx_int_t pack_map(ngx_map_t* map, ngx_str_t* output, ngx_pool_t* pool, FILE* file) {
  FILE* fout;
  fout = fopen("/tmp/nginx_pack_log", "a+tc");
  fprintf(fout, "in pack map!\n");
  fflush(fout);
  ngx_protobuf_context_t ctx;
  size_t size;
  ngx_int_t rc;


  size = ngx_map__size(map);
  if (size == 0)
  {
    return NGX_DECLINED;
  }
  fprintf(fout, "map size = %d\n", size);
  fflush(fout);
  output->data = ngx_palloc(pool, size);
  if (output->data == NULL)
  {
    fprintf(fout, "output->data is null\n");
    fflush(fout);
    return NGX_ERROR;
  }

  ctx.pool = pool;
  ctx.buffer.start = output->data;
  ctx.buffer.pos = ctx.buffer.start;
  ctx.buffer.last = ctx.buffer.start + size;

  rc = ngx_map__pack(map, &ctx);

  output->len = ctx.buffer.pos - ctx.buffer.start;
  u_char cc = *(output->data + output->len);
  fprintf(fout, "pack rc = %d,output->len = %d output->data = %c\n", rc, output->len, cc);
  fflush(fout);
  return rc;
}

void readPgm(std::string pgm_file_path, unsigned int& width , unsigned int& height, std::vector<unsigned char>& value_vec) {
  int row = 0, col = 0;
  ifstream infile(pgm_file_path.c_str());
  stringstream ss;
  string inputLine = "";

  // First line : version
  getline(infile, inputLine);
  if (inputLine.compare("P5") != 0) cerr << "Version error" << endl;
  else cout << "Version : " << inputLine << endl;

  // Second line : comment
  getline(infile, inputLine);

  while (inputLine.find("#") != string::npos) {
    cout << "Comment : " << inputLine << endl;
    getline(infile, inputLine);
  }
  // Continue with a stringstream
  ss << infile.rdbuf();
  // Third line : size
  stringstream wwhhss(inputLine);
  wwhhss >> width >> height;
  cout << width << " columns and " << height << " rows" << endl;

  int array[height][width];

  int max_value;
  ss >> max_value;
  cout << "max value = " << max_value << endl;
  unsigned char pixel;
  // Following lines : data
  for (row = 0; row < height; ++row) {
    for (col = 0; col < width; ++col) {
      ss >> pixel;
      array[row][col] = pixel;
      value_vec.push_back(pixel);
    }
  }
  // Now print the array to see the result
  // FILE* file = fopen("/home/pengjiawei/pixels.log","a");
  // for(row = 0; row < height; ++row) {
  //     for(col = 0; col < width; ++col) {
  //         fprintf(file,"%d\n",array[row][col]);
  //     }
  // }
  // fclose(file);

  infile.close();
}

//pgm value
const unsigned char FREE = 255;
const unsigned char OBSTACLE = 0;
static ngx_int_t ngx_http_mymap_handler(ngx_http_request_t *r)
{
  //必须是GET或者HEAD方法，否则返回405 Not Allowed
  if (!(r->method & (NGX_HTTP_GET | NGX_HTTP_HEAD)))
  {
    return NGX_HTTP_NOT_ALLOWED;
  }
  //丢弃请求中的包体
  ngx_int_t rc = ngx_http_discard_request_body(r);
  if (rc != NGX_OK)
  {
    return rc;
  }

  FILE* fout;
  fout = fopen("/tmp/nginx_map_log", "a+tc");
  fprintf(fout, "m->start process!\n");
  fflush(fout);

  std::string path = "/home/pengjiawei/willow-full.pgm";
  unsigned int width;
  unsigned int height;
  std::vector<unsigned char> vec_value;
  readPgm(path, width, height, vec_value);
  fprintf(fout, "width = %d,height = %d,vec_value size = %d\n", width, height, vec_value.size());
  fflush(fout);
  int mapcell_num = width * height;
  // ngx_array_t* cell_array = ngx_array_create(r->pool, mapcell_num, sizeof(ngx_map_cell_t) );

  fprintf(fout, "create ngx array \n");
  fprintf(fout, "construct_map\n");
  ngx_map_t* map = ngx_map__alloc(r->pool);

  fflush(fout);
  for (int i = 0; i < height; ++i)
  {
    for (int j = 0; j < width; ++j)
    {
      // ngx_map_cell_t* mapcell = ngx_array_push(cell_array);
      int index = j + i * width;

      if (vec_value[index] == FREE) {
      ngx_map_cell_t* mapcell = ngx_map__add__mapCell(map, r->pool);
        ngx_map_cell__set_x(mapcell, j);
        ngx_map_cell__set_y(mapcell, i);
        ngx_map_cell__set_value(mapcell, NGX_MAPCELL_FREE);
        
      } else if (vec_value[index] == OBSTACLE)
      {
      ngx_map_cell_t* mapcell = ngx_map__add__mapCell(map, r->pool);

        ngx_map_cell__set_x(mapcell, j);
        ngx_map_cell__set_y(mapcell, i);
        ngx_map_cell__set_value(mapcell, NGX_MAPCELL_OBSTACLE);
      }

    }
  }

  fprintf(fout, "ngx map size = %d\n", ngx_map__size(map) );




  //remember to use set
  ngx_map__set_width(map, width);
  ngx_map__set_height(map, height);
  //I tried to use this method but i failed , look above in for loop,you will find the correct solution
  // ngx_map__set_mapCell(map,cell_array);

  fprintf(fout, "map has width = %d,has height =%d  has mapcell = %d \n", map->__has_width, map->__has_height, map->__has_mapCell);
  ngx_str_t str = ngx_null_string;
  ngx_str_t* output = &str;
  fprintf(fout, "pack map\n");
  fflush(fout);
  pack_map(map, output, r->pool, fout);


  ngx_str_t test_t = ngx_string("hello, this is nginx");
  // output = &test_t;
  fprintf(fout, "output len = %d data = %c\n", output->len, *(output->data + output->len) );

  //设置返回的Content-Type。注意，ngx_str_t有一个很方便的初始化宏
//ngx_string，它可以把ngx_str_t的data和len成员都设置好
  ngx_str_t type = ngx_string ("text/plain");
  //设置Content-Type
  r->headers_out.content_type = type;
  //set length of msg
  r->headers_out.content_length_n = output->len;
  //设置返回状态码
  r->headers_out.status = NGX_HTTP_OK;

  rc = ngx_http_send_header(r);

  if (rc == NGX_ERROR || rc > NGX_OK || r->header_only)  {
    fprintf(fout, "send header error\n");
    return rc;
  }

  ngx_buf_t* b;
  b = ngx_create_temp_buf(r->pool, output->len);
  if (b == NULL)
  {
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
  fprintf (fout, "m->end process!\n");
  fflush (fout);
  fclose (fout);

  return ngx_http_output_filter (r, &out);

}