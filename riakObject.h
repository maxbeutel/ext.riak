
#ifndef RIAK_OBJECT_H
#define RIAK_OBJECT_H

#define RIAK_OBJECT_CLIENT "client"
#define RIAK_OBJECT_CLIENT_LEN strlen(RIAK_OBJECT_CLIENT)

#define RIAK_OBJECT_BUCKET "bucket"
#define RIAK_OBJECT_BUCKET_LEN strlen(RIAK_OBJECT_BUCKET)

#define RIAK_OBJECT_KEY "key"
#define RIAK_OBJECT_KEY_LEN strlen(RIAK_OBJECT_KEY)

#define RIAK_OBJECT_JSONIZE "jsonize"
#define RIAK_OBJECT_JSONIZE_LEN strlen(RIAK_OBJECT_JSONIZE)

#define RIAK_OBJECT_HEADERS "headers"
#define RIAK_OBJECT_HEADERS_LEN strlen(RIAK_OBJECT_HEADERS)

#define RIAK_OBJECT_LINKS "links"
#define RIAK_OBJECT_LINKS_LEN strlen(RIAK_OBJECT_LINKS)

#define RIAK_OBJECT_SIBLINGS "siblings"
#define RIAK_OBJECT_SIBLINGS_LEN strlen(RIAK_OBJECT_SIBLINGS)

#define RIAK_OBJECT_EXISTS "exists"
#define RIAK_OBJECT_EXISTS_LEN strlen(RIAK_OBJECT_EXISTS)

#define RIAK_OBJECT_DATA "data"
#define RIAK_OBJECT_DATA_LEN strlen(RIAK_OBJECT_DATA)

zend_class_entry *riak_ce_riakObject;

PHP_METHOD(riakObject, __construct);

PHP_METHOD(riakObject, getBucket);
PHP_METHOD(riakObject, getKey);

PHP_METHOD(riakObject, getData);
PHP_METHOD(riakObject, setData);

PHP_METHOD(riakObject, status);
PHP_METHOD(riakObject, exists);

PHP_METHOD(riakObject, getContentType);
PHP_METHOD(riakObject, setContentType);

PHP_METHOD(riakObject, addLink);
PHP_METHOD(riakObject, removeLink);
PHP_METHOD(riakObject, getLinks);

PHP_METHOD(riakObject, store);
PHP_METHOD(riakObject, reload);
PHP_METHOD(riakObject, delete);

PHP_METHOD(riakObject, clear);
PHP_METHOD(riakObject, vclock);

PHP_METHOD(riakObject, populate);
PHP_METHOD(riakObject, populateLinks);

PHP_METHOD(riakObject, hasSiblings);
PHP_METHOD(riakObject, getSiblingCount);
PHP_METHOD(riakObject, getSibling);
PHP_METHOD(riakObject, getSiblings);

PHP_METHOD(riakObject, add);
PHP_METHOD(riakObject, link);

PHP_METHOD(riakObject, reduce);
PHP_METHOD(riakObject, map);

#endif
