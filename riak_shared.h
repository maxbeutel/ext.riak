#ifndef RIAK_SHARED_H
#define RIAK_SHARED_H

/* arginfo helper constant */
#if (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION > 2) || PHP_MAJOR_VERSION > 5
#   define RIAK_ARG_INFO
#else
#   define RIAK_ARG_INFO static
#endif


/* common helper macros */
#define RIAK_MALLOC_WARNING() \
    zend_error(E_ERROR, "Memory allocation failed");

#define RIAK_RETURN_SELF() \
    zval *_current_instance = getThis(); \
    RETURN_ZVAL(_current_instance, 1, 0); 

    
/* macros for returning object properties */
#define RIAK_CALL_SIMPLE_GETTER(classname, property_name, property_name_len) \
    zval *data; \
    data = zend_read_property(classname, getThis(), property_name, property_name_len, 0 TSRMLS_CC); \
    *return_value = *data; \
    zval_copy_ctor(return_value); 


/* macros for calling object methods */
/* @TODO prefix with RIAK_ */
#define PUSH_PARAM(arg) zend_vm_stack_push(arg TSRMLS_CC)
#define POP_PARAM() (void)zend_vm_stack_pop(TSRMLS_C)
#define PUSH_EO_PARAM()
#define POP_EO_PARAM()
 
#define CALL_METHOD_BASE(classname, name) zim_##classname##_##name
 
#define CALL_METHOD_HELPER(classname, name, retval, thisptr, num, param) \
    PUSH_PARAM(param); PUSH_PARAM((void*)num);                            \
    PUSH_EO_PARAM();                                                      \
    CALL_METHOD_BASE(classname, name)(num, retval, NULL, thisptr, 0 TSRMLS_CC); \
    POP_EO_PARAM();                       \
    POP_PARAM(); POP_PARAM();
 
#define CALL_METHOD(classname, name, retval, thisptr)                  \
    CALL_METHOD_BASE(classname, name)(0, retval, NULL, thisptr, 0 TSRMLS_CC);
 
#define CALL_METHOD1(classname, name, retval, thisptr, param1)         \
    CALL_METHOD_HELPER(classname, name, retval, thisptr, 1, param1);
 
#define CALL_METHOD2(classname, name, retval, thisptr, param1, param2) \
    PUSH_PARAM(param1);                                                   \
    CALL_METHOD_HELPER(classname, name, retval, thisptr, 2, param2);     \
    POP_PARAM();
 
#define CALL_METHOD3(classname, name, retval, thisptr, param1, param2, param3) \
    PUSH_PARAM(param1); PUSH_PARAM(param2);                               \
    CALL_METHOD_HELPER(classname, name, retval, thisptr, 3, param3);     \
    POP_PARAM(); POP_PARAM();
  
  
#endif  
