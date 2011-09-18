
#ifndef RIAK_CLIENT_H
#define RIAK_CLIENT_H

zend_class_entry *riak_ce_riakClient;

#define RIAK_CLIENT_HOST "host"
#define RIAK_CLIENT_HOST_LEN strlen(RIAK_CLIENT_HOST)

#define RIAK_CLIENT_PORT "port"
#define RIAK_CLIENT_PORT_LEN strlen(RIAK_CLIENT_PORT)

#define RIAK_CLIENT_PREFIX "prefix"
#define RIAK_CLIENT_PREFIX_LEN strlen(RIAK_CLIENT_PREFIX)

#define RIAK_CLIENT_MAPRED_PREFIX "mapred_prefix"
#define RIAK_CLIENT_MAPRED_PREFIX_LEN strlen(RIAK_CLIENT_MAPRED_PREFIX)

#define RIAK_CLIENT_CLIENT_ID "clientid"
#define RIAK_CLIENT_CLIENT_ID_LEN strlen(RIAK_CLIENT_CLIENT_ID)

#define RIAK_CLIENT_R "r"
#define RIAK_CLIENT_R_LEN strlen(RIAK_CLIENT_R)

#define RIAK_CLIENT_W "w"
#define RIAK_CLIENT_W_LEN strlen(RIAK_CLIENT_W)

#define RIAK_CLIENT_DW "dw"
#define RIAK_CLIENT_DW_LEN strlen(RIAK_CLIENT_DW)

PHP_METHOD(riakClient, __construct);

PHP_METHOD(riakClient, getR);
PHP_METHOD(riakClient, setR);

PHP_METHOD(riakClient, getW);
PHP_METHOD(riakClient, setW);

PHP_METHOD(riakClient, getDW);
PHP_METHOD(riakClient, setDW);

PHP_METHOD(riakClient, getBaseAddress);

PHP_METHOD(riakClient, getClientId);

PHP_METHOD(riakClient, isAlive);

PHP_METHOD(riakClient, bucket);
PHP_METHOD(riakClient, buckets);

PHP_METHOD(riakClient, add);

PHP_METHOD(riakClient, search);

PHP_METHOD(riakClient, link);

PHP_METHOD(riakClient, map);
PHP_METHOD(riakClient, reduce);

#endif




