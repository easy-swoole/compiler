<?php


namespace EasySwoole\Compiler;


use EasySwoole\Compiler\Exception\Exception;

class Compiler
{
    static function encrypt(string $file,License $license = null):?string
    {
        if(!file_exists($file)){
            throw new Exception("{$file} not exist");
        }
        $data = substr(file_get_contents($file),5);
        $pack = new Package();
        $pack->setLicense($license);
        $pack->setCode($data);
        $data = serialize($pack);
        return easy_compiler_encrypt($data);
    }
}