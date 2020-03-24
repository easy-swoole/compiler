<?php
require_once __DIR__.'/../vendor/autoload.php';

if(!file_exists(__DIR__.'/ClassA.php.bak')){
    EasySwoole\Compiler\Compiler::encrypt(__DIR__.'/ClassA.php',true);
}


$http = new swoole_http_server("127.0.0.1", 9501);

$http->on("request", function ($request, $response) {
    $class = 'EasySwoole\Compiler\Test\ClassA';

    $response->header("Content-Type", "text/plain");
    $test = new $class();
    $str = $test->test();
    $response->end($str);
});

$http->start();
