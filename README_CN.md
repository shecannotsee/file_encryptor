# FileEncryptor

## 简介

该应用是一个文件加解密应用，主要用途是使用密码对文件进行加密和解密，并且在终端添加了用户友好的界面。



## 如何使用

首先将项目编译成可执行程序，然后做如下准备工作

```bash
$ ls
FileEncryptor
$ echo "hello FileEncryptor"> a.txt
$ ls
a.txt  FileEncryptor
$ cat a.txt 
hello FileEncryptor

$ ./FileEncryptor # 接下来运行应用对文件进行加密
```

ui如下

![ui_display](../FileEncryptor/resource/ui_display.png)

加密按照下列方法用密码123456进行加密

![en](../FileEncryptor/resource/en.png)

会发现在当前目录下多出一个`a.txt.enc`文件，该文件就是使用密码加密后的文件。

接下来对`a.txt.enc`文件进行解密，仍然使用密码123456

![de](../FileEncryptor/resource/de.png)

在最后对文件进行检查

```bash
$ ls
a.txt  a.txt.enc  a.txt.enc.dec  FileEncryptor
$ cat a.txt.enc.dec 
hello FileEncryptor
$ 
```

