//
//  ngx_http_google_util.c
//  nginx
//
//  Created by Cube on 14/12/15.
//  Copyright (c) 2014 Cube. All rights reserved.
//

#include "ngx_http_google_util.h"

ngx_str_t
ngx_http_google_trim(char * str, size_t len)
{
  ngx_str_t v;
  v.data = (u_char *)str;
  v.len  = len;
  
  if (!v.data || !v.len) return v;
  while (v.len > 0 && v.data[0]         == ' ') { v.len--; v.data++; }
  while (v.len > 0 && v.data[v.len - 1] == ' ') { v.len--; }
  return v;
}

ngx_array_t * /* ngx_str_t */
ngx_http_google_explode(ngx_http_request_t * r,
                        ngx_str_t          * v, const char * de)
{
  ngx_str_t   * s;
  ngx_array_t * ss = ngx_array_create(r->pool, 4, sizeof(ngx_keyval_t));
  if (!ss) return NULL;
  
  char * dup = ngx_pcalloc(r->pool, v->len + 1);
  if (!dup) return NULL;
  ngx_memcpy(dup, v->data, v->len);
  
  char * pch, * brkt; size_t len;
  pch = strtok_r(dup, de, &brkt);
  for (; pch; pch = strtok_r(NULL, de, &brkt))
  {
    if (!(len = strlen(pch))) continue;
    s = ngx_array_push(ss);
    if (!s) return NULL;
   *s = ngx_http_google_trim(pch, strlen(pch));
  }
  
  return ss;
}

ngx_str_t *
ngx_http_google_implode(ngx_http_request_t * r,
                        ngx_array_t        * a, /* ngx_str_t */
                        const char         * de)
{
  u_char * buf;
  size_t   len = 0, delen = strlen(de);
  
  ngx_uint_t i;
  ngx_str_t * s, * hd = a->elts, * str;
  
  for (i = 0; i < a->nelts; i++) {
    s = hd + i;
    len += s->len + delen;
  }
  
  str = ngx_pcalloc(r->pool, sizeof(ngx_str_t));
  if (!str) return NULL;
  
  buf = str->data = ngx_pcalloc(r->pool, len);
  if (!buf) return NULL;
  
  for (i = 0; i < a->nelts; i++) {
    s = hd + i;
    if (buf > str->data) buf = ngx_copy(buf, de, delen);
    buf = ngx_copy(buf, s->data, s->len);
  }
  
  str->len = buf - str->data;
  return str;
}

ngx_array_t * /* ngx_keyval_t */
ngx_http_google_explode_kv(ngx_http_request_t * r,
                           ngx_str_t          * v, const char * de)
{
  ngx_keyval_t * kv, tkv;
  ngx_array_t  * kvs = ngx_array_create(r->pool, 4, sizeof(ngx_keyval_t));
  if (!kvs) return NULL;
  
  char * dup = ngx_pcalloc(r->pool, v->len + 1);
  if (!dup) return NULL;
  ngx_memcpy(dup, v->data, v->len);
  
  char * pch, * sep, * brkt;
  pch = strtok_r(dup, de, &brkt);
  
  for (; pch; pch = strtok_r(NULL, de, &brkt))
  {
    sep = strchr(pch, '=');
    if (!sep) continue;
    *sep++ = '\0';
    tkv.key   = ngx_http_google_trim(pch, strlen(pch));
    tkv.value = ngx_http_google_trim(sep, strlen(sep));
    if (!tkv.key.len) continue;
    kv = ngx_array_push(kvs);
    if (!kv) return NULL;
    *kv = tkv;
  }
  
  return kvs;
}

ngx_str_t *
ngx_http_google_implode_kv(ngx_http_request_t * r,
                           ngx_array_t        * a, /* ngx_keyval_t */
                           const char         * de,
                           const ngx_int_t    keep_cookie)
{
  u_char * buf;
  size_t   len = 0, delen = strlen(de);
  
  ngx_uint_t i;
  ngx_str_t * str;
  ngx_keyval_t * kv, * hd = a->elts;
  
  for (i = 0; i < a->nelts; i++) {
    kv   = hd + i;
    len += kv->key.len + kv->value.len + delen + 1;
	// ngx_http_google_debug(r->pool, "GGDEBUG: %V(%zu) -> %V(%zu)\n", &kv->key, kv->key.len, &kv->value, kv->value.len);
  }
  str = ngx_pcalloc(r->pool, sizeof(ngx_str_t));
  if (!str) return NULL;
  buf = str->data = ngx_pcalloc(r->pool, len);
  if (!buf)
  {
	  return NULL;
  }
  for (i = 0; i < a->nelts; i++) {
    kv = hd + i;

	// Drop specified cookies
	if (kv->key.len == 2 && keep_cookie == 0 &&
		(
			!ngx_strncasecmp(kv->key.data, (u_char *)"GZ", 2) ||
			!ngx_strncasecmp(kv->key.data, (u_char *)"AC", 2) ||
			!ngx_strncasecmp(kv->key.data, (u_char *)"ED", 2) ||
			!ngx_strncasecmp(kv->key.data, (u_char *)"KI", 2) ||
			!ngx_strncasecmp(kv->key.data, (u_char *)"PW", 2)
			)
		)
		continue;
    if (buf > str->data) buf = ngx_copy(buf, de, delen);
    buf = ngx_copy(buf, kv->key.data,   kv->key.len);
    *buf++ = '=';
    buf = ngx_copy(buf, kv->value.data, kv->value.len);
  }
  str->len = buf - str->data;
  return str;
}

ngx_int_t
ngx_http_google_debug(ngx_pool_t * pool, const char * fmt, ...)
{
  u_char   * buf;
  ngx_uint_t len = 4096;
  
  buf = ngx_pcalloc(pool, len + 1);
  if (!buf) {
    return fprintf(stderr, "ngx_pcalloc(%lu) failed\n", (unsigned long)len);
  }
  
  va_list args;
  va_start(args, fmt);
  ngx_vsnprintf(buf, len, fmt, args);
  va_end(args);
  
  return fprintf(stdout, "%s", buf);
}

ngx_int_t
ngx_http_google_validate_user(ngx_http_request_t * r, const ngx_int_t keyid, const ngx_uint_t endtime, const ngx_str_t * auth_code, ngx_int_t * rkeyid)
{
	/*
	 * 0: pass
	 * 1: timeout
	 * 2: hash not pass
	 * 3: other
	 */
	ngx_uint_t now = (ngx_uint_t)time(NULL);
	if (endtime < now)
	{
		// ngx_http_google_debug(r->pool, "Time exceeded! %d < %d\n", endtime, now);
		return 1;
	}
	ngx_http_google_loc_conf_t * glcf;
	glcf = ngx_http_get_module_loc_conf(r, ngx_http_google_filter_module);
	if (glcf->auth_password == NGX_CONF_UNSET_PTR)
	{
		// ngx_http_google_debug(r->pool, "Error: auth_password not set.\n");
		return 3;
	}
	ngx_str_t * hd = glcf->auth_password->elts;

	if (auth_code->len != 32 || (ngx_uint_t)keyid >= glcf->auth_password->nelts)
	{
		return 2;
	}
	if (keyid < 0)
	{
		ngx_int_t nkeyid;
		for (nkeyid = 0; (ngx_uint_t)nkeyid < glcf->auth_password->nelts; nkeyid++)
			if (!ngx_http_google_validate_user(r, nkeyid, endtime, auth_code, rkeyid))
				break;
		if ((ngx_uint_t)nkeyid < glcf->auth_password->nelts)
		{
			// keyid to be set = nkeyid
			*rkeyid = nkeyid;
			return 0;
		}
		return 2;
	}
	ngx_str_t * key = hd + (ngx_uint_t)keyid;
	u_char * buf;
	ngx_uint_t buf_len = 17 + key->len + glcf->auth_salt.len;
	buf = ngx_pcalloc(r->pool, buf_len);
	if (!buf) return 3;
	ngx_snprintf(buf, 16, "%015d", endtime);
	ngx_snprintf(buf + 15, key->len + 1, "%s", key->data);
	ngx_snprintf(buf + 15 + key->len, glcf->auth_salt.len + 1, "%s", glcf->auth_salt.data);
	buf[15 + key->len + glcf->auth_salt.len] = '\0';

	u_char auth_code_buf[16];
	MD5(buf, strlen((char *)buf), auth_code_buf);
	ngx_uint_t loop;
	static const char hexchars[] = "0123456789abcdef";
	for(loop = 0; loop < 16; loop++)
	{
		unsigned char b = auth_code_buf[loop];
		if (auth_code->data[loop * 2] != hexchars[b >> 4] ||
			auth_code->data[loop * 2 + 1] != hexchars[b & 0xF])
			return 2;
	}
	return 0;
}

ngx_int_t
ngx_http_google_get_validate_token(ngx_http_request_t * r, const ngx_str_t * key, const ngx_uint_t endtime, ngx_uint_t * rkeyid, ngx_str_t * auth_code)
{
	/*
	 * 0: OK
	 * 1: key_error
	 * 2: other
	 */
	ngx_uint_t keyid;
	ngx_http_google_loc_conf_t * glcf;
	glcf = ngx_http_get_module_loc_conf(r, ngx_http_google_filter_module);
	if (glcf->auth_password == NGX_CONF_UNSET_PTR)
	{
		ngx_http_google_debug(r->pool, "Error: auth_password not set.\n");
		return 2;
	}
	ngx_str_t * hd = glcf->auth_password->elts, * key_item;
	for(keyid = 0; keyid < glcf->auth_password->nelts; keyid++)
	{
		key_item = hd + keyid;
		if (key->len != key_item->len)
			continue;
		if (!ngx_strncmp(key->data, key_item->data, key->len))
			break;
	}
	if (keyid >= glcf->auth_password->nelts)
		return 1;
	u_char * buf;
	ngx_uint_t buf_len = 17 + key->len + glcf->auth_salt.len;
	buf = ngx_pcalloc(r->pool, buf_len);
	if (!buf) return 2;
	ngx_snprintf(buf, 16, "%015d", endtime);
	ngx_snprintf(buf + 15, key->len + 1, "%s", key->data);
	ngx_snprintf(buf + 15 + key->len, glcf->auth_salt.len + 1, "%s", glcf->auth_salt.data);
	buf[15 + key->len + glcf->auth_salt.len] = '\0';

	// ngx_http_google_debug(r->pool, "Before md5: %s\n", buf);

	u_char auth_code_buf[16];
	MD5(buf, 15 + key->len + glcf->auth_salt.len, auth_code_buf);
	static const char hexchars[] = "0123456789abcdef";
	auth_code->len = 32;
	auth_code->data = ngx_pcalloc(r->pool, auth_code->len + 1);
	ngx_uint_t i;
	for(i = 0; i < 16; i++)
	{
		unsigned char b = auth_code_buf[i];
		auth_code->data[2 * i] = hexchars[b >> 4];
		auth_code->data[2 * i + 1] = hexchars[b & 0xF];
	}
	*rkeyid = keyid;
	return 0;
}
