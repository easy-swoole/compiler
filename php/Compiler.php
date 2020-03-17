<?php


namespace EasySwoole\Compiler;


use EasySwoole\Compiler\Exception\Exception;

class Compiler
{
    static function encrypt(string $file):string
    {
        if(!file_exists($file)){
            throw new Exception("{$file} not exist");
        }
        $data = substr(file_get_contents($file),5);
        $res = easy_compiler_encrypt($data);
        return "<?php return easy_compiler_decrypt('{$res}');";
    }
}