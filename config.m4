dnl Tell PHP about the argument to enable the compiler extension

PHP_ARG_ENABLE(easy_compiler, enable the easy_compiler extension,NULL)


PHP_NEW_EXTENSION(easy_compiler, src/compiler.c src/pkcs7.c src/aes.c, $ext_shared)
