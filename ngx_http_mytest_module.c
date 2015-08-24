/*
 * ngx-myhttp.c
 *
 *  Created on: Aug 22, 2015
 *      Author: yangzz
 */
#include <nginx.h>
#include <ngx_event.h>
#include <ngx_event_connect.h>
#include <ngx_http.h>

static ngx_int_t
ngx_http_mytest_handler(ngx_http_request_t *r);

static char*
ngx_http_mytest(ngx_conf_t* cf,ngx_command_t *cmd,void * conf){

	ngx_http_core_loc_conf_t* clcf;
	clcf = ngx_http_conf_get_module_loc_conf(cf,ngx_http_core_module);
	clcf->handler = ngx_http_mytest_handler;
	return NGX_CONF_OK;
}


static ngx_command_t
ngx_http_mytest_commands[] = {
		{
				ngx_string("my_test"),
				NGX_HTTP_MAIN_CONF |
				NGX_HTTP_SRV_CONF |
				NGX_HTTP_LOC_CONF |
				NGX_HTTP_LMT_CONF |
				NGX_CONF_NOARGS,
				ngx_http_mytest,
				NGX_HTTP_LOC_CONF_OFFSET,
				0,
				NULL
		},
		ngx_null_command
};



static ngx_http_module_t ngx_http_mytest_module_ctx = {
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
};

ngx_module_t ngx_http_mytest_module = {
		NGX_MODULE_V1,
		&ngx_http_mytest_module_ctx,
		ngx_http_mytest_commands,
		NGX_HTTP_MODULE,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NGX_MODULE_V1_PADDING
};


//process request

//ngx_http_handler_pt
static ngx_int_t
ngx_http_mytest_handler(ngx_http_request_t *r){
	//	method name
	//	r->method_name.data;

	// recieve body aysc callback()yi
//	ngx_int_t rc = ngx_http_read_client_request_body(r,ngx_http_mytest_body_handler);
//
//	if(rc  >=  NGX_HTTP_SPECIAL_RESPONSE){
//		return rc;
//	}

	if(!(r->method & (NGX_HTTP_GET | NGX_HTTP_HEAD))){
		return NGX_HTTP_NOT_ALLOWED;
	}

	ngx_int_t rc = ngx_http_discard_request_body(r);
	if(rc != NGX_OK){
		return rc;
	}

	ngx_str_t type = ngx_string("text/plain");
	ngx_str_t response = ngx_string("Hello World!");
	r->headers_out.status = NGX_HTTP_OK;
	r->headers_out.content_length_n = response.len;
	r->headers_out.content_type = type;

	// send reponse header fadsfd
	//1. set header
//	ngx_table_elt_t *h = ngx_list_push(&r->headers_out.headers);
//	if(h == NULL){
//		return NGX_ERROR;
//	}
//	h->hash=1;
//	h->key.len=sizeof("TestHead") -1;
//	h->key.data = (u_char *) "TestHead";
//	h->value.len = sizeof("TestValue") -1;
//	h->value.data = (u_char *)"TestValue" ;
//
//	rc = ngx_http_send_header(r);
//	if(rc == NGX_ERROR||rc == NGX_OK||r->header_only){
//		return rc;
//	}


	//2. send body
	ngx_buf_t *b;
	b = ngx_create_temp_buf(r->pool,response.len);
	if(b == NULL){
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	ngx_memcpy(b->pos,response.data,response.len);

	b->last = b->pos + response.len;
	b->last_buf = 1;

	ngx_chain_t out;

	out.buf = b;
	out.next = NULL;
	return ngx_http_output_filter(r,&out);

//	return NGX_DONE;
//	return ngx_http_output_filter(r,&out);
//	return NGX_HTTP_OK;
//	ngx_table_elt_t ;


}

//  after recive http_body callback
//void
//ngx_http_mytest_body_handler(ngx_http_request_t *r){
//
//}





