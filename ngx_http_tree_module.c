/**
 * @file   ngx_http_tree_module.c
 * @author António P. P. Almeida <appa@perusio.net>
 * @date   Wed Aug 17 12:06:52 2011
 *
 * @brief  A hello world module for Nginx.
 *
 * @section LICENSE
 *
 * Copyright (C) 2011 by Dominic Fallows, António P. P. Almeida <appa@perusio.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


#define TREE "hello world"

static char *ngx_http_tree(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t ngx_http_tree_handler(ngx_http_request_t *r);
static ngx_int_t ngx_http_tree_request_init(ngx_conf_t *cf);
static ngx_int_t ngx_http_tree_request_handler(ngx_http_request_t *r);
static ngx_int_t ngx_http_tree_rewrite_handler(ngx_http_request_t *r);
/**
 * This module provided directive: hello world.
 *
 */
static ngx_command_t ngx_http_tree_commands[] = {

    { ngx_string("tree"), /* directive */
      NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS, /* location context and takes
                                            no arguments*/
      ngx_http_tree, /* configuration setup function */
      0, /* No offset. Only one context is supported. */
      0, /* No offset when storing the module configuration on struct. */
      NULL},

    ngx_null_command /* command termination */
};

/* The hello world string. */
static u_char ngx_tree[] = TREE;

/* The module context. */
static ngx_http_module_t ngx_http_tree_module_ctx = {
    NULL, /* preconfiguration */
    ngx_http_tree_request_init, /* postconfiguration */

    NULL, /* create main configuration */
    NULL, /* init main configuration */

    NULL, /* create server configuration */
    NULL, /* merge server configuration */

    NULL, /* create location configuration */
    NULL /* merge location configuration */
};

/* Module definition. */
ngx_module_t ngx_http_tree_module = {
    NGX_MODULE_V1,
    &ngx_http_tree_module_ctx, /* module context */
    ngx_http_tree_commands, /* module directives */
    NGX_HTTP_MODULE, /* module type */
    NULL, /* init master */
    NULL, /* init module */
    NULL, /* init process */
    NULL, /* init thread */
    NULL, /* exit thread */
    NULL, /* exit process */
    NULL, /* exit master */
    NGX_MODULE_V1_PADDING
};
static ngx_int_t
ngx_http_tree_request_handler(ngx_http_request_t *r)
{
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "------------------access callback-----------------------");
    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
        "http args: \"%V\"", &r->args);

    ngx_str_t args = ngx_string("response_type=code&redirect_uri=http://192.168.80.160:8383/github/callback&client_id=6086dc8afc3f6cc9b5cf&state=1234567899");
    r->args = args;

    return NGX_OK;
}

static ngx_int_t
ngx_http_tree_rewrite_handler(ngx_http_request_t *r)
{
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "------------------rewrite callback-----------------------");
 
    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
        "http args: \"%V\"", &r->args);
    ngx_str_t args = ngx_string("state=xxxxxxxxxxxxxxxxxxxxxxx");
    r->args = args;
   

    return NGX_DECLINED;
}


static ngx_int_t
ngx_http_tree_request_init(ngx_conf_t *cf)
{
    ngx_http_handler_pt        *haccess;
    ngx_http_handler_pt        *hrewrite;
    ngx_http_core_main_conf_t  *cmcf;

    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

    haccess = ngx_array_push(&cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers);
    if (haccess == NULL) {
        return NGX_ERROR;
    }

    *haccess = ngx_http_tree_request_handler;

    hrewrite = ngx_array_push(&cmcf->phases[NGX_HTTP_REWRITE_PHASE].handlers);
    if (hrewrite == NULL) {
        return NGX_ERROR;
    }

    *hrewrite = ngx_http_tree_rewrite_handler;


    return NGX_OK;
}

/**
 * Content handler.
 *
 * @param r
 *   Pointer to the request structure. See http_request.h.
 * @return
 *   The status of the response generation.
 */
static ngx_int_t ngx_http_tree_handler(ngx_http_request_t *r)
{
    ngx_buf_t *b;
    ngx_chain_t out;

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
        "-------------------tree-------------------------", 0);
    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
        "http args: \"%V\"", &r->args);
    

    /* Set the Content-Type header. */
    r->headers_out.content_type.len = sizeof("text/plain") - 1;
    r->headers_out.content_type.data = (u_char *) "text/plain";

    /* Allocate a new buffer for sending out the reply. */
    b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));

    /* Insertion in the buffer chain. */
    out.buf = b;
    out.next = NULL; /* just one buffer */

    b->pos = ngx_tree; /* first position in memory of the data */
    b->last = ngx_tree + sizeof(ngx_tree); /* last position in memory of the data */
    b->memory = 1; /* content is in read-only memory */
    b->last_buf = 1; /* there will be no more buffers in the request */

    /* Sending the headers for the reply. */
    r->headers_out.status = NGX_HTTP_OK; /* 200 status code */
    /* Get the content length of the body. */
    r->headers_out.content_length_n = sizeof(ngx_tree);
    ngx_http_send_header(r); /* Send the headers */

    /* Send the body, and return the status code of the output filter chain. */
    return ngx_http_output_filter(r, &out);
} /* ngx_http_tree_handler */

/**
 * Configuration setup function that installs the content handler.
 *
 * @param cf
 *   Module configuration structure pointer.
 * @param cmd
 *   Module directives structure pointer.
 * @param conf
 *   Module configuration structure pointer.
 * @return string
 *   Status of the configuration setup.
 */
static char *ngx_http_tree(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf; /* pointer to core location configuration */

    /* Install the hello world handler. */
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_tree_handler;

    return NGX_CONF_OK;
} /* ngx_http_tree */
