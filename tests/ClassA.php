<?php


namespace EasySwoole\Compiler\Test;


class ClassA
{
    function test()
    {
        return (new ClassB())->run();
    }
}