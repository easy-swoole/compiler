<?php
require_once __DIR__.'/../vendor/autoload.php';

if(!file_exists(__DIR__.'/ClassA.php.bak')){
    EasySwoole\Compiler\Compiler::encrypt(__DIR__.'/ClassA.php',true);
}

if(pcntl_fork()){

    $test = new \EasySwoole\Compiler\Test\ClassA();
    var_dump($test->test());
    sleep(2);
    exit();
}else{

    sleep(2);
    $test = new \EasySwoole\Compiler\Test\ClassA();
    var_dump($test->test());
    exit();
}