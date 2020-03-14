# 这是一个很笨的加密器
## 加密原理
我们可以经常在某些经过加密文件的php文件代码格式大体如下：
```php
xxx_loader_lable
<?php
if(!function_exists("xxx_loader")){
   die('xxx_loader not install');
}
//encrypt part
xxxxxxxxxxx
```

这个文件。正常情况下,php是无法解析的。但是呢，zend_vm的一些接口，允许我们载入某些文件的时候，对文件进行预处理。因此我的拓展需要做的事情就是，如果遇到这样格式的文件，那么我把他解析为以下两部分：
- 部分1
```
<?php
if(!function_exists("xxx_loader")){
   die('xxx_loader not install');
}
```
- 部分2
```
//encrypt part
xxxxxxxxxxx
```

因此，code就是我经过加密后的目标字符串，显然，我们需要完成的一个步骤就是、字符串到代码的转变。而这个时候，如果有敏感的同学，就会想到一个东西，那就是
```eval()```。因此以上代码等价于：
```
<?php
if(!function_exists("xxx_loader")){
   die('xxx_loader not install');
}
eval(encrypt part);
```

但是实际上，并没有这么简单，如果我需要实现对机器授权的限制，那么应该是这样的。
```
$info = xxx_loader->decode(encrypPart);
$license = $info->licenseCheck();
if($license){
    eval($info->realyCode);
}
```

因此，如何保护我这个xxx_loader的实现逻辑，或者是加密秘钥，成为了代码加解密的关键。但是用php的话，容易出现，被逆向比如目前场景的php混淆，很容易破解。
因此就有人提出想法，如果我把这个加密的函数协程php拓展编译成so动态库文件，然后so在做加壳混淆，不就完美的解决了吗。毕竟、so加壳混淆的方案，可是非常成熟的。好了，我的加密拓展，就是这样的思路。

## 实现步骤
- 我们会实现一个php版本的，用于原理直观讲解
- c版本。c版本我们也会实现并开源，且教大家如何对自己编译出来的so文件进行加壳混淆。