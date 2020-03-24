<?php


namespace EasySwoole\Compiler;


use EasySwoole\Compiler\Exception\Exception;
use EasySwoole\Utility\File;

class Compiler
{
    static function encrypt(string $file,bool $keepBak = true):int
    {
        $count = 0;
        if(is_dir($file)){
            $list = File::scanDirectory($file)['files'];
            if(array($list)){
                foreach ($list as $file){
                    $data = self::handleFile($file);
                    if($keepBak){
                        $bak = file_get_contents($file);
                        file_put_contents("{$file}.bak",$bak);
                    }
                    file_put_contents($file,$data);
                    $count++;
                }
            }
        }else{
            $data = self::handleFile($file);
            if($keepBak){
                cpoy($file,"{$file}.bak");
            }
            file_put_contents($file,$data);
            $count++;
        }
        return $count;
    }

    static function handleFile(string $file):string
    {
        if(!file_exists($file)){
            throw new Exception("{$file} not exist");
        }
        $data = substr(file_get_contents($file),5);
        $res = easy_compiler_encrypt($data);
        return "<?php return easy_compiler_decrypt('{$res}');";
    }
}