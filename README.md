
## 准备工作
* 准备一台CentOS7 x64服务器 (推荐腾讯云 阿里云 IDC大宽带)
* CPU/内存：服务器配置最低1核1G
* 带宽：推荐8Mbps以上
* 网络：必须具有固定公网IP（IPV4）

## 安装脚本
如果出现安装失败，请全格重装系统，手动更新yum源后重新执行安装脚本即可。
```shell script
wget --no-check-certificate -O fast.bin https://raw.githubusercontent.com/Shirley-Jones/FAS-Panel/master/fast.bin && chmod +x ./fast.bin && ./fast.bin
```

## 编译说明
* 先安装GCC: yum -y install gcc gcc++ gdb 
* 编译 gcc -o fast.bin newfast.c
* 执行 ./fast.bin


## 常用命令

> 重启流控 vpn restart

> 开端口 vpn port

> 查系统版本 cat /etc/redhat-release

> 查端口开启 netstat -nulp  

> 查服务器时间 date

> 改服务器时间 date -s 09/01/2021

> 禁止ping echo 1 >/proc/sys/net/ipv4/icmp_echo_ignore_all

> 允许ping echo 0 >/proc/sys/net/ipv4/icmp_echo_ignore_all

> 查web端口 netstat -nutlp | grep httpd


## 免责声明
* 脚本写的很辣鸡，还请大佬多多包涵。
* 本脚本仅用于学习交流，禁止商业，下载安装后请在24小时内删除！
* 我们只删除了流控授权部分，其余代码均未修改。
* 流控版权为筑梦网络科技(筑梦工作室)所有！！
* FAS流控官网: https://www.dingd.cn
* 所有文件（部分C语言文件 筑梦冬瓜 没有开源，我也没有！）我个人没有加入任何后门，脚本已全部开源，欢迎检查，不放心的不要用， 谢谢！
## 其他声明
* 流控搭建后部分大厂服务器(如阿里云腾讯云) 会报毒 webshell 漏洞文件（这个文件是筑梦冬瓜留下的，不关本人的事），这个漏洞文件位于 /var/www/html/admin/fas_service.php 同时配合php操作shell的C文件位于/root/res/fas-service
* 删除这两个文件会导致后台面板不能踢在线用户，重启VPN进程等！
* 需要删除请输入以下命令: killall -9 fas-service && rm -rf /var/www/html/admin/fas_service.php /root/res/fas-service 
* 还有一个php文件位于 /var/www/html/system.php 文件中的自定义函数 systemi() ，您可以手动删除这个 systemi() 函数！！！
## 温馨提醒
* 如果需要二次开发FAS流控，请注意不要使用FAS的流量监控 FasAUTH.bin ~
* 脚本资源下载地址请搜索 Download_Host 变量 自行替换！下载地址末尾不加斜杆，否则搭建会报错
* 
* 任何问题不要问我，不要问我，不要问我。
* 任何问题不要问我，不要问我，不要问我。
* 任何问题不要问我，不要问我，不要问我。
* 有问题请反馈至: xshirleyjones02@gmail.com (仅处理脚本搭建报错问题，逻辑错误问题。)


