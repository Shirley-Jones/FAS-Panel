
## 准备工作
* 准备一台CentOS7 x64服务器 (推荐腾讯云 阿里云 IDC大宽带)
* CPU/内存：服务器配置最低1核1G
* 带宽：推荐5M以上
* 网络：必须具有固定公网IP

## 安装脚本
如果出现安装失败，请全格重装系统，手动更新yum源后重新执行安装脚本即可。
```shell script
wget --no-check-certificate -O fast.bin https://raw.githubusercontent.com/shirley852/FAS-Panel/master/fast.bin && chmod +x ./fast.bin && ./fast.bin
```

## 编译说明
* 先安装GCC yum -y install gcc gcc++ gdb 
* 编译 gcc -o fast.bin newfast.c
* 执行 ./fast.bin


## 常用命令

> 重启流控 vpn restart

>开端口 port

>查系统版本 cat /etc/redhat-release

>查端口开启 netstat -nulp  

>查服务器时间 date

>改服务器时间 date -s 09/01/2021

>禁止ping echo 1 >/proc/sys/net/ipv4/icmp_echo_ignore_all

>允许ping echo 0 >/proc/sys/net/ipv4/icmp_echo_ignore_all

>查web端口 netstat -nutlp | grep httpd


## 免责声明
* 此脚本仅用适用于测试学习，不可用于非法或商业用途，严禁用于任何违法违规用途
* 流控版权为筑梦冬瓜所有！！
* 所有文件（部分C语言文件 筑梦冬瓜 没有开源，我也没有！）没有任何后门，脚本已开源，欢迎检查，不放心的不要用，不要用！不要用！不要用！！ 谢谢！
* 此版本与官方版本完全相同，只删除了授权文件，没有任何后门、广告，不放心的不要用，不要用！不要用！不要用！！ 谢谢！
* 任何问题不要问我，不要问我，不要问我。
* 任何问题不要问我，不要问我，不要问我。
* 任何问题不要问我，不要问我，不要问我。


