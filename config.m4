dnl Tell PHP about the argument to enable the complier extension

PHP_ARG_ENABLE(easy_complier, enable the easy_complier extension,NULL)

PHP_NEW_EXTENSION(easy_complier, src/complier.c, $ext_shared)
