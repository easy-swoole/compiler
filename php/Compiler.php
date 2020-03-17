<?php


namespace EasySwoole\Compiler;


use EasySwoole\Compiler\Exception\Exception;

class Compiler
{
    public static $loaderName = 'easy_compiler_decrypt';
    static function encrypt(string $file,License $license = null):string
    {
        if(!file_exists($file)){
            throw new Exception("{$file} not exist");
        }
        $data = substr(file_get_contents($file),5);
        $pack = new Package();
        $pack->setLicense($license);
        $pack->setCode($data);
        $data = serialize($pack);
        $res = easy_compiler_encrypt($data);
        return "<?php ".self::$loaderName."('{$res}');";


    }
}