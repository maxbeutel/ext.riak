#ifndef RIAK_CURL_H
#define RIAK_CURL_H

#define RIAK_CURL_REQUESTHEADER_CONTENTTYPE_JSON "Content-Type: text/json"
#define RIAK_CURL_REQUESTHEADER_CONTENTTYPE_JSON_LEN strlen(RIAK_CURL_REQUESTHEADER_CONTENTTYPE_JSON)

typedef struct {
    char** str;
    size_t num;
    size_t size;
    size_t block_size;
} riakCurlRequestHeader;

PHPAPI riakCurlRequestHeader* riak_curl_create_request_header();
PHPAPI void riak_curl_delete_request_header(riakCurlRequestHeader* request_header);
PHPAPI int riak_curl_add_request_header_str(riakCurlRequestHeader* request_header, char* str, int str_len);

PHPAPI char** riak_curl_add_request_header_start(riakCurlRequestHeader* request_header);
PHPAPI char** riak_curl_add_request_header_end(riakCurlRequestHeader* request_header);

PHPAPI int riak_curl_fetch_json_response(char *client_id, char *request_url, zval **json_response, riakCurlRequestHeader *response_headers TSRMLS_DC);
PHPAPI int riak_curl_fetch_response(char *client_id, char *request_url, char **response_body, riakCurlRequestHeader *response_headers TSRMLS_DC);

PHPAPI int riak_curl_send_put_json_request(char *client_id, char *request_url, zval *data, riakCurlRequestHeader* request_header TSRMLS_DC);
PHPAPI int riak_curl_send_post_json_request(char *client_id, char *request_url, zval *data, riakCurlRequestHeader* request_header TSRMLS_DC);

PHPAPI int riak_curl_send_put_request(char *client_id, char *request_url, char *data, riakCurlRequestHeader* request_header TSRMLS_DC);
PHPAPI int riak_curl_send_post_request(char *client_id, char *request_url, char *data, riakCurlRequestHeader* request_header TSRMLS_DC);

#endif
