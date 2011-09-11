#include <php.h>

#include "php_riak.h"
#include "riakClient.h"
#include "riakBucket.h"
#include "riakObject.h"

zend_function_entry riak_functions[] = {
    { NULL, NULL, NULL }
};

zend_module_entry riak_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_RIAK_EXTNAME,
    riak_functions,
    PHP_MINIT(riak),
    NULL,
    NULL,
    NULL,
    NULL,
    PHP_RIAK_VERSION,
    STANDARD_MODULE_PROPERTIES
};

PHP_MINIT_FUNCTION(riak) {
    riak_init_riakClient(TSRMLS_C);
    riak_init_riakBucket(TSRMLS_C);  
    riak_init_riakObject(TSRMLS_C);  
}

ZEND_GET_MODULE(riak)

