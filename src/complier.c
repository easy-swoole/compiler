#include <php.h>
#include "complier.h"

zend_function_entry easy_complier_functions[] = {
        PHP_FE(easy_complier_encrypt, NULL)
        PHP_FE(easy_complier_decrypt, NULL)
        PHP_FE_END
};

zend_module_entry easy_complier_module_entry = {
        STANDARD_MODULE_HEADER,
        PHP_COMPLIER_EXTNAME,
        easy_complier_functions,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        PHP_COMPLIER_VERSION,
        STANDARD_MODULE_PROPERTIES,
};

ZEND_GET_MODULE(easy_complier);

PHP_FUNCTION(easy_complier_encrypt) {
    php_printf("easy_complier_encrypt");
};


PHP_FUNCTION(easy_complier_decrypt) {
    char *encrypt_string;
    size_t encrypt_string_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &encrypt_string, &encrypt_string_len) == FAILURE) {
        RETURN_NULL();
    }

    php_printf("Hello %s ", encrypt_string);
    RETURN_TRUE;
};
