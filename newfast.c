#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <strings.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#define Start_Check_Scripts "./fast.bin"
#define FAS_Version "3.0"
#define Dingd_Name "筑梦科技 冬瓜"
#define FAS_Name "FAS网络用户管理"  
#define Scripts_Time "2022.12.30 20.25"  
/*
	Time 2022.12.30 20.25
	
	流控版权为筑梦冬瓜所有！！
	
	下载地址说明    请自行搜索变量名替换
	
	Download_Host  资源下载地址  (末尾不加斜杆)
	
	注意 私有源地址请自行配置  (末尾不加斜杆)
	
	所有YUM源使用的阿里云 
	
	任何问题不要问我，不要问我，不要问我。
	
*/
char* cmd_system(char* command);
char buff[1024];
int code = 0;
char* shellcmd(char* cmd, char* buff, int size)
{
  char temp[256];
  FILE* fp = NULL;
  int offset = 0;
  int len;
  
  fp = popen(cmd, "r");
  if(fp == NULL)
  {
    return NULL;
  }
 
  while(fgets(temp, sizeof(temp), fp) != NULL)
  {
    len = strlen(temp);
    if(offset + len < size)
    {
      strcpy(buff+offset, temp);
      offset += len;
    }
    else
    {
      buff[offset] = 0;
      break;
    }
  }
  
  if(fp != NULL)
  {
    pclose(fp);
  }
 
  return buff;
}

int CentOS_Yum_Install(char* pack)
{
	char co_install[100000];
	sprintf(co_install,"yum install %s -y > /dev/null 2>&1;echo -n $?",pack);
	if(strcat(cmd_system(co_install),"0")!="0"){
		return 1;
	}else{
		return 0;
	}
}

int CentOS_Yum_Uninstall(char* pack)
{
	char co_install[100000];
	sprintf(co_install,"yum remove %s -y > /dev/null 2>&1;echo -n $?",pack);
	if(strcat(cmd_system(co_install),"0")!="0"){
		return 1;
	}else{
		return 0;
	}
}

int Debian_Apt_Install(char* pack)
{
	char co_install[100000];
	sprintf(co_install,"apt-get install %s -y > /dev/null 2>&1;echo -n $?",pack);
	if(strcat(cmd_system(co_install),"0")!="0"){
		return 1;
	}else{
		return 0;
	}
}

int Debian_Apt_Uninstall(char* pack)
{
	char co_install[100000];
	sprintf(co_install,"apt-get remove %s -y > /dev/null 2>&1;echo -n $?",pack);
	if(strcat(cmd_system(co_install),"0")!="0"){
		return 1;
	}else{
		return 0;
	}
}

int runshell(int way,char* content)
{
	/*
	指令说明   by kk
	checkcode(runshell(1,""));   CentOS Yum  安装指令
	checkcode(runshell(2,""));   CentOS Yum  卸载指令
	checkcode(runshell(3,""));   Debian apt-get  安装指令
	checkcode(runshell(4,""));   Debian apt-get  卸载指令
	checkcode(runshell(5,""));   直接执行命令
	*/
	
	if(way==1){
		return CentOS_Yum_Install(content);
	}else if(way==2){
		return CentOS_Yum_Uninstall(content);
	}else if(way==3){
		return Debian_Apt_Install(content);
	}else if(way==4){
		return Debian_Apt_Uninstall(content);
	}else if(way==5){
		char com[100000];
		sprintf(com,"%s;echo -n $?",content);
		return atoi(cmd_system(com));
	}else{
		puts("程序逻辑错误！脚本终止...");
		exit(1);
	}
}

void checkcode(int code1)
{
	if(code1!=0){
	code=code+1;
	}
}

void Start_Check()
{
	if(code!=0){
		printf("启动失败！\n");
	}else{
		printf("启动成功！\n");
	}
	code=0;
}


void Readme()
{
	setbuf(stdout,NULL);
	system("clear");
	sleep(1);
	printf("----------------------------------------------------------------\n");
	printf("                  欢迎使用%s系统                   \n",FAS_Name);
	printf("                      版本 V%s                      \n",FAS_Version);
	printf("作者: %s  官网: https://www.dingd.cn/product/fas.html\n",Dingd_Name);
	printf("本脚本仅用于学习交流，禁止商业，下载安装后请在24小时内删除！\n",Dingd_Name);
	printf("此版本与官方版本完全相同，只删除了授权文件，没有任何后门、广告，不放心的请不要用！\n");
	printf("脚本源码全部开源（部分C语言文件 %s 没有开源，我也没有！）\n",Dingd_Name);
	printf("系统支持: CentOS 7 X64位\n");
	printf("Time: %s\n",Scripts_Time);
	printf("----------------------------------------------------------------\n");
	printf("-----------------------同意 请回车继续--------------------------\n");
	char hc11[1];
	gets(hc11);
	GET_IP();
}


void GET_IP()
{
	int Author;
		printf("请选择IP获取方式：\n");
		printf("1.自动获取IP（接口来自 members.3322.org ）\n");
		printf("2.手动输入IP（自动获取IP失败时请使用此选项）\n");
		printf("\n");
		printf("请选择[1-2]: ");
		scanf("%d",&Author);
		char hc11[1];
		gets(hc11);
		
	switch(Author)
	{
		char IP[200];
		
		case 1:
			printf("\n请稍等\n");
			strcpy(IP,cmd_system("echo `curl -s \"http://members.3322.org/dyndns/getip\"` | tr -d '\n'"));
			if (strcmp(IP,"")==0){
				printf("\n无法检测您的服务器IP，可能会影响到您接下来的搭建工作，保险起见，请手动输入IP进行确认！\n");
				sleep(1);
				printf("\n请输入服务器IP: ");
				gets(IP);
				if (strcmp(IP,"")==0){
					printf("\n你脑子有洞吗？\n");
					setbuf(stdout,NULL);
					system("reboot");
					exit(0);
				}else{
					printf("\n您的IP是: %s 如不正确请立即停止安装，回车继续！",IP);
					char read_01[1];
					gets(read_01);
					Install_Option(IP);
				}
			}else{
				printf("\n您的IP是: %s 如不正确请立即停止安装，回车继续！",IP);
				char read_01[1];
				gets(read_01);
				Install_Option(IP);
			}
		break;

		case 2:
			sleep(1);
			printf("\n请输入服务器IP: ");
			gets(IP);
			if (strcmp(IP,"")==0){
				printf("\n你脑子有洞吗？\n");
				setbuf(stdout,NULL);
				system("reboot");
				exit(0);
			}else{
				printf("\n您的IP是: %s 如不正确请立即停止安装，回车继续！",IP);
				char read_01[1];
				gets(read_01);
				Install_Option(IP);
			}
			
			
		break;
		
		default:
		sleep(1);
		printf("\n你脑子有洞吗？\n");
		setbuf(stdout,NULL);
		system("reboot");
		exit(0);
		
	}
}





void FAS_On_MySQL(char* IP)
{
	
	
	
	//主机开启远程数据库访问权限
	
	
	printf("\n请输入本机IP: ");
	char FAS_On_MySQL_IP[20];
	gets(FAS_On_MySQL_IP);
	if (strcmp(FAS_On_MySQL_IP,"")==0){
		strcpy(FAS_On_MySQL_IP,IP);
		printf("\n已输入主机IP为: %s\n",FAS_On_MySQL_IP);
	}else{
		printf("\n已输入主机IP为: %s\n",FAS_On_MySQL_IP);
	}
	sleep(1);
	
	printf("\n请输入本机数据库端口: ");
	char FAS_On_MySQL_Port[20];
	gets(FAS_On_MySQL_Port);
	if (strcmp(FAS_On_MySQL_Port,"")==0){
		strcpy(FAS_On_MySQL_Port,"");
		printf("\n已输入本机数据库端口为: %s\n",FAS_On_MySQL_Port);
	}else{
		printf("\n已输入本机数据库端口为: %s\n",FAS_On_MySQL_Port);
	}
	sleep(1);
	
	
	printf("\n请输入本机数据库账户: ");
	char FAS_On_MySQL_User[20];
	gets(FAS_On_MySQL_User);
	if (strcmp(FAS_On_MySQL_User,"")==0){
		strcpy(FAS_On_MySQL_User,"");
		printf("\n已输入本机数据库账户为: %s\n",FAS_On_MySQL_User);
	}else{
		printf("\n已输入本机数据库账户为: %s\n",FAS_On_MySQL_User);
	}
	sleep(1);
	
	printf("\n请输入本机数据库密码: ");
	char FAS_On_MySQL_Pass[20];
	gets(FAS_On_MySQL_Pass);
	if (strcmp(FAS_On_MySQL_Pass,"")==0){
		strcpy(FAS_On_MySQL_Pass,"");
		printf("\n已输入本机数据库密码为: %s\n",FAS_On_MySQL_Pass);
	}else{
		printf("\n已输入本机数据库密码为: %s\n",FAS_On_MySQL_Pass);
	}
	sleep(1);
	
	
	printf("\n操作中... \n");
	sleep(3);
	
	char FAS_On_MySQL1[200];
	sprintf(FAS_On_MySQL1,"mysql -h%s -P%s -u%s -p%s -e \"grant all privileges on *.* to '%s'@'%' identified by '%s' with grant option;flush privileges;\"",FAS_On_MySQL_IP,FAS_On_MySQL_Port,FAS_On_MySQL_User,FAS_On_MySQL_Pass,FAS_On_MySQL_User,FAS_On_MySQL_Pass);
	checkcode(runshell(5,FAS_On_MySQL1));
	
	setbuf(stdout,NULL);
	system("systemctl restart mariadb.service");
	
	
	
	
	printf("\n操作已完成。 \n");
	exit(0);
	
	
	
	
}





void FAS_Note_for_MySQL(char* IP)
{
	
	
	
	//节点机器对接主机数据库.  或者云库
	
	
	printf("\n请输入主机IP/云库域名: ");
	char FAS_Note_for_MySQL_IP[20];
	gets(FAS_Note_for_MySQL_IP);
	if (strcmp(FAS_Note_for_MySQL_IP,"")==0){
		strcpy(FAS_Note_for_MySQL_IP,"");
		printf("\n已输入主机IP为: %s\n",FAS_Note_for_MySQL_IP);
	}else{
		printf("\n已输入主机IP为: %s\n",FAS_Note_for_MySQL_IP);
	}
	sleep(1);
	
	printf("\n请输入主机数据库端口: ");
	char FAS_Note_for_MySQL_Port[20];
	gets(FAS_Note_for_MySQL_Port);
	if (strcmp(FAS_Note_for_MySQL_Port,"")==0){
		strcpy(FAS_Note_for_MySQL_Port,"");
		printf("\n已输入主机数据库端口为: %s\n",FAS_Note_for_MySQL_Port);
	}else{
		printf("\n已输入主机数据库端口为: %s\n",FAS_Note_for_MySQL_Port);
	}
	sleep(1);
	
	
	printf("\n请输入主机数据库账户: ");
	char FAS_Note_for_MySQL_User[20];
	gets(FAS_Note_for_MySQL_User);
	if (strcmp(FAS_Note_for_MySQL_User,"")==0){
		strcpy(FAS_Note_for_MySQL_User,"");
		printf("\n已输入主机数据库账户为: %s\n",FAS_Note_for_MySQL_User);
	}else{
		printf("\n已输入主机数据库账户为: %s\n",FAS_Note_for_MySQL_User);
	}
	sleep(1);
	
	printf("\n请输入主机数据库密码: ");
	char FAS_Note_for_MySQL_Pass[20];
	gets(FAS_Note_for_MySQL_Pass);
	if (strcmp(FAS_Note_for_MySQL_Pass,"")==0){
		strcpy(FAS_Note_for_MySQL_Pass,"");
		printf("\n已输入主机数据库密码为: %s\n",FAS_Note_for_MySQL_Pass);
	}else{
		printf("\n已输入主机数据库密码为: %s\n",FAS_Note_for_MySQL_Pass);
	}
	sleep(1);
	
	
	printf("\n操作中... \n");
	
	
	//删除配置文件
	checkcode(runshell(5,"rm -rf /etc/openvpn/auth_config.conf"));
	checkcode(runshell(5,"rm -rf /var/www/html/config.php"));
	
	//创建新的配置文件auth_config
	char ADD_auth_config[200];
	sprintf(ADD_auth_config,"cat >> /etc/openvpn/auth_config.conf <<EOF\n#!/bin/bash\n#兼容配置文件 此文件格式既可以适应shell也可以适应FasAUTH，但是这里不能使用变量，也不是真的SHELL文件，不要写任何shell在这个文件\n#FasAUTH监控系统配置文件\n#请谨慎修改\n#数据库地址\nmysql_host=%s\n#数据库用户\nmysql_user=%s\n#数据库密码\nmysql_pass=%s\n#数据库端口\nmysql_port=%s\n#数据库端口\nmysql_data=vpndata\n#本机地址\naddress=%s\n#指定异常记录回收时间 单位s 600即为十分钟\nunset_time=600\n#删除僵尸记录地址\ndel=\"/root/res/del\"\n\n#进程1监控地址\nstatus_file_1=\"/var/www/html/openvpn_api/online_1194.txt 7075 1194 tcp-server\"\nstatus_file_2=\"/var/www/html/openvpn_api/online_1195.txt 7076 1195 tcp-server\"\nstatus_file_3=\"/var/www/html/openvpn_api/online_1196.txt 7077 1196 tcp-server\"\nstatus_file_4=\"/var/www/html/openvpn_api/online_1197.txt 7078 1197 tcp-server\"\nstatus_file_5=\"/var/www/html/openvpn_api/user-status-udp.txt 7079 53 udp\"\n#睡眠时间\nsleep=3\nEOF",FAS_Note_for_MySQL_IP,FAS_Note_for_MySQL_User,FAS_Note_for_MySQL_Pass,FAS_Note_for_MySQL_Port,IP);
	setbuf(stdout,NULL);
	system(ADD_auth_config);
	//checkcode(runshell(5,ADD_auth_config));
	checkcode(runshell(5,"chmod -R 0777 /etc/openvpn/auth_config.conf"));
	
	
	//创建新的配置文件config
	char ADD_config[200];
	sprintf(ADD_config,"cat >> /var/www/html/config.php <<EOF\n<?php\ndefine(\"_host_\",\"%s\");\ndefine(\"_user_\",\"%s\");\ndefine(\"_pass_\",\"%s\");\ndefine(\"_port_\",\"%s\");\ndefine(\"_ov_\",\"vpndata\");\ndefine(\"_openvpn_\",\"openvpn\");\ndefine(\"_iuser_\",\"iuser\");\ndefine(\"_ipass_\",\"pass\");\ndefine(\"_isent_\",\"isent\");\ndefine(\"_irecv_\",\"irecv\");\ndefine(\"_starttime_\",\"starttime\");\ndefine(\"_endtime_\",\"endtime\");\ndefine(\"_maxll_\",\"maxll\");\ndefine(\_other_\",\"dlid,tian\");\ndefine(\"_i_\",\"i\");\nEOF",FAS_Note_for_MySQL_IP,FAS_Note_for_MySQL_User,FAS_Note_for_MySQL_Pass,FAS_Note_for_MySQL_Port);
	setbuf(stdout,NULL);
	system(ADD_config);
	//checkcode(runshell(5,ADD_config));
	checkcode(runshell(5,"chmod -R 0777 /var/www/html/config.php"));
	
	setbuf(stdout,NULL);
	system("vpn restart");
	printf("\n操作已完成。 \n");
	exit(0);
	
	
	
	
	
	
	
}

void FAS_Load(char* IP)
{
	
	//系统负载
	setbuf(stdout,NULL);
	system("clear");
	sleep(1);
	int Author1;
		printf("\n请选择安装类型：\n");
		printf("1.主机开启远程数据库访问权限(只需要开启一次)\n");
		printf("2.节点机器对接主机数据库(也可以直接对接云库)\n");
		printf("3.退出脚本\n");
		printf("\n");
		printf("请选择[1-3]: ");
		scanf("%d",&Author1);
		char hc111[1];
		gets(hc111);
			
	switch(Author1)
	{
		case 1:
			FAS_On_MySQL(IP);
		break;

		case 2:
			FAS_Note_for_MySQL(IP);
		break;
		
		case 3:
			printf("\n脚本结束。\n");
			exit(0);
		break;
			
			
			
		default:
		printf("\n你脑子有洞吗？\n");
		setbuf(stdout,NULL);
		system("reboot");
		exit(0);
		
	}
	
	
}

void CentOS_Update()
{
	
	//系统更新  
	setbuf(stdout,NULL);
	system("clear");
	sleep(1);
	char CentOS_Version[10];
	strcpy(CentOS_Version,cmd_system("echo `cat /etc/redhat-release | awk '{print$4}' | awk -F \".\" '{print$1}'` | tr -d '\n'"));
	if (strcmp(CentOS_Version,"7")==0){
		printf("\n确认将系统更新至CentOS7.10吗\n");
		char opcxdfg[1];
		gets(opcxdfg);
		printf("\n开始更新(使用System Shell更新)很耗时耐心等待....\n");
		sleep(3);
		checkcode(runshell(5,"rm -rf /etc/yum.repos.d/CentOS-Base.repo"));
		checkcode(runshell(5,"wget -O /etc/yum.repos.d/CentOS-Base.repo https://mirrors.aliyun.com/repo/Centos-7.repo >/dev/null 2>&1"));
		setbuf(stdout,NULL);
		system("yum clean all");
		setbuf(stdout,NULL);
		system("yum makecache");
		setbuf(stdout,NULL);
		system("yum update -y");
		setbuf(stdout,NULL);
		system("yum upgrade -y");
		
		printf("\n升级完成！！！回车重启服务器应用升级文件！稍后重启请CTRL+C结束脚本！\n");
		char opcxda[1];
		gets(opcxda);
		setbuf(stdout,NULL);
		system("reboot");
		setbuf(stdout,NULL);
		system("shutdown -r now");
		exit(0);
	}else{
		printf("\n错误，系统非CentOS或其他CentOS版本!!!\n");
		exit(1);
	}
}


void Install_Option(char* IP)
{
	
	
	setbuf(stdout,NULL);
	system("clear");
	sleep(1);
	int Author1;
		printf("\n请选择安装类型：\n");
		printf("1.全新安装[FAS3.0]\n");
		printf("2.卸载流控[FAS3.0]\n");
		printf("3.系统负载[集群]\n");
		printf("4.系统更新[将CentOS更新至CentOS7最新版本!]\n");
		printf("5.退出脚本\n");
		printf("\n");
		printf("请选择[1-5]: ");
		scanf("%d",&Author1);
		char hc111[1];
		gets(hc111);
			
	switch(Author1)
	{
		case 1:
			Install_FAS(IP);
		break;

		case 2:
			Uninstall_FAS();
		break;

		case 3:
			FAS_Load(IP);
		break;
		
		case 4:
			CentOS_Update();
		break;
		
		case 5:
			printf("\n脚本结束。\n");
			exit(0);
		break;
		
		
		
		default:
		printf("\n你脑子有洞吗？\n");
		setbuf(stdout,NULL);
		system("reboot");
		exit(0);
		
	}
}


void Uninstall_FAS()
{
	/*
	FAS流控 卸载程序说明：
	本脚本仅保证可以完全卸载聚力工作室的FAS流控，尽可能兼容于官方筑梦FAS流控卸载(官方流控可能存在文件残留)。
	所有脚本  没有任何后门，脚本已开源，欢迎检查，不放心的不要用，不要用！不要用！不要用！！ 谢谢！
	更新时间 2022.12.30
	*/
	
	setbuf(stdout,NULL);
	system("clear");
	sleep(1);
	printf("\nFAS流控 卸载程序说明：\n本脚本仅保证可以完全卸载FAS流控，尽可能兼容于官方筑梦FAS流控卸载(官方流控可能存在文件残留)。\n所有脚本  没有任何后门，脚本已开源，欢迎检查，不放心的不要用，不要用，不要用！！\n更新时间 2022.12.30\n");
	sleep(1);
	printf("\n确认卸载FAS3.0流控吗，此操作无法撤销，回车卸载！CTRL+C结束脚本。\n");
	char opcxd[1];
	gets(opcxd);
	printf("\n开始卸载(使用system shell卸载)....\n");
	sleep(3);
	setbuf(stdout,NULL);
	system("systemctl start firewalld.servic");
	setbuf(stdout,NULL);
	system("systemctl enable firewalld.service");
	setbuf(stdout,NULL);
	system("#firewall-cmd --add-port=22/tcp");
	setbuf(stdout,NULL);
	system("firewall-cmd --zone=public --add-port=22/tcp --permanent");
	setbuf(stdout,NULL);
	system("firewall-cmd --reload");
	setbuf(stdout,NULL);
	system("vpn stop");
	setbuf(stdout,NULL);
	system("systemctl stop fas.service");
	setbuf(stdout,NULL);
	system("systemctl disable fas.service");
	setbuf(stdout,NULL);
	system("systemctl stop mariadb.service");
	setbuf(stdout,NULL);
	system("killall -9 FasAUTH.bin");
	setbuf(stdout,NULL);
	system("clekillall -9 jk.shar");
	setbuf(stdout,NULL);
	system("killall -9 openvpn.bin");
	setbuf(stdout,NULL);
	system("killall -9 rate.bin");
	setbuf(stdout,NULL);
	system("rm -rf /etc/sysctl.conf");
	setbuf(stdout,NULL);
	system("rm -rf /bin/FasAUTH.bin");
	setbuf(stdout,NULL);
	system("rm -rf /bin/jk.sh");
	setbuf(stdout,NULL);
	system("rm -rf /bin/openvpn.bin");
	setbuf(stdout,NULL);
	system("rm -rf /bin/rate.bin");
	setbuf(stdout,NULL);
	system("rm -rf /bin/vpn");
	setbuf(stdout,NULL);
	system("rm -rf /root/res");
	setbuf(stdout,NULL);
	system("rm -rf /var/www/*");
	setbuf(stdout,NULL);
	system("rm -rf /etc/openvpn");
	setbuf(stdout,NULL);
	system("rm -rf /etc/dnsmasq.conf");
	setbuf(stdout,NULL);
	system("rm -rf /lib/systemd/system/fas.service");
	setbuf(stdout,NULL);
	system("rm -rf /etc/yum.repos.d/JuLi_PHP.repo");
	setbuf(stdout,NULL);
	system("rm -rf /etc/pki/rpm-gpg/RPM-GPG-KEY-EPEL-7");
	setbuf(stdout,NULL);
	system("rm -rf /etc/yum.repos.d/epel.repo");
	setbuf(stdout,NULL);
	system("rm -rf /etc/yum.repos.d/epel-testing.repo");
	setbuf(stdout,NULL);
	system("rm -rf /etc/yum.repos.d/dingd_PHP.repo");
	setbuf(stdout,NULL);
	system("rm -rf /etc/yum.repos.d/epel-modular.repo");
	setbuf(stdout,NULL);
	system("rm -rf /etc/yum.repos.d/epel-testing-modular.repo");
	setbuf(stdout,NULL);
	system("rm -rf /etc/yum.repos.d/Dingd_PHP.repo");
	setbuf(stdout,NULL);
	system("rm -rf /etc/yum.repos.d/Dingd-PHP.repo");
	setbuf(stdout,NULL);
	system("rm -rf /etc/yum.repos.d/dingd-PHP.repo");
	setbuf(stdout,NULL);
	system("rm -rf /etc/fas_host");
	setbuf(stdout,NULL);
	system("rm -rf /etc/rate.d");
	setbuf(stdout,NULL);
	system("sed -i \"s/vpn restart/#vpn restart/g\" /etc/rc.d/rc.local");
	setbuf(stdout,NULL);
	system("yum -y remove mariadb mariadb-server mariadb*");
	setbuf(stdout,NULL);
	system("yum -y remove mariadb-libs");
	setbuf(stdout,NULL);
	system("rm -rf /etc/my.cnf");
	setbuf(stdout,NULL);
	system("rm -rf /var/lib/mysql");
	setbuf(stdout,NULL);
	system("rm -rf /var/lib64/mysql");
	setbuf(stdout,NULL);
	system("yum -y remove epel-release");
	setbuf(stdout,NULL);
	system("yum -y remove telnet avahi openssl openssl-libs openssl-devel lzo lzo-devel pam pam-devel automake pkgconfig gawk tar zip unzip net-tools psmisc gcc pkcs11-helper libxml2 libxml2-devel bzip2 bzip2-devel libcurl libcurl-devel libjpeg libjpeg-devel libpng libpng-devel freetype freetype-devel gmp gmp-devel libmcrypt libmcrypt-devel readline readline-devel libxslt libxslt-devel");
	setbuf(stdout,NULL);
	system("yum -y remove docker iptables iptables-services httpd dnsmasq java ");
	setbuf(stdout,NULL);
	system("rpm -e –nodeps liblz4-1.8.1.2-alt1.x86_64");
	setbuf(stdout,NULL);
	system("rpm -e –nodeps pkcs11-helper-1.11-3.el7.x86_64");
	setbuf(stdout,NULL);
	system("rpm -e –nodeps openvpn-2.4.6-1.el7.x86_64");
	setbuf(stdout,NULL);
	system("rpm -e –nodeps openvpn-2.4.3-1.el7.x86_64");
	setbuf(stdout,NULL);
	system("rpm -e –nodeps openvpn-2.4.12-1.el7.x86_64");
	setbuf(stdout,NULL);
	system("yum -y remove php70w php70w-bcmath php70w-cli php70w-common php70w-dba php70w-devel php70w-embedded php70w-enchant php70w-gd php70w-imap php70w-ldap php70w-mbstring php70w-mcrypt php70w-mysqlnd php70w-odbc php70w-opcache php70w-pdo php70w-pdo_dblib php70w-pear.noarch php70w-pecl-apcu php70w-pecl-apcu-devel php70w-pecl-imagick php70w-pecl-imagick-devel php70w-pecl-mongodb php70w-pecl-redis php70w-pecl-xdebug php70w-pgsql php70w-xml php70w-xmlrpc php70w-intl php70w-mcrypt --nogpgcheck php-fedora-autoloader php-php-gettext php-tcpdf php-tcpdf-dejavu-sans-fonts php70w-tidy");
	setbuf(stdout,NULL);
	system("yum -y remove php* php70* php70w* php56* php56w* php71* php71w* php72* php72w*");
	
	
	printf("\n卸载完成！！！回车重启服务器保证数据库卸载干净！稍后重启请CTRL+C结束脚本！\n");
	char opcxda[1];
	gets(opcxda);
	setbuf(stdout,NULL);
	system("reboot");
	setbuf(stdout,NULL);
	system("shutdown -r now");
	exit(0);
	
	
}



void Install_FAS(char* IP)
{
	
	setbuf(stdout,NULL);
	system("clear");
	sleep(1);
	
	
	if (!access("/usr/bin/mysql",0)){
        printf("\n错误，检测到有MySQL残留或已安装流控系统/MySQL Server，请先卸载或重装系统后在试！\n");
		exit(1);
	}
	
	
	printf("\n开始安装FAS3.0\n");
	sleep(1);
	
	
	//随机数据库密码
	char Random_MySQL_Pass[100];
	strcpy(Random_MySQL_Pass,cmd_system("date +%s%N | md5sum | head -c 20"));
	
	printf("\n请设置数据库密码: ");
	char JuLi_MySQL_Pass[20];
	gets(JuLi_MySQL_Pass);
	if (strcmp(JuLi_MySQL_Pass,"")==0){
		strcpy(JuLi_MySQL_Pass,Random_MySQL_Pass);
		printf("已设置数据库密码为: %s\n",JuLi_MySQL_Pass);
	}else{
		printf("已设置数据库密码为: %s\n",JuLi_MySQL_Pass);
	}
	
	printf("\n请设置APP名称: ");
	char JuLi_APP_Name[20];
	gets(JuLi_APP_Name);
	if (strcmp(JuLi_APP_Name,"")==0){
		strcpy(JuLi_APP_Name,"流量卫士");
		printf("已设置APP名称为: %s\n",JuLi_APP_Name);
	}else{
		printf("已设置APP名称为: %s\n",JuLi_APP_Name);
	}
	
	printf("\n请输入SSH端口: ");
	char JuLi_SSH_Port[20];
	gets(JuLi_SSH_Port);
	if (strcmp(JuLi_SSH_Port,"")==0){
		strcpy(JuLi_SSH_Port,"22");
		printf("已输入SSH端口为: %s\n",JuLi_SSH_Port);
	}else{
		printf("已输入SSH端口为: %s\n",JuLi_SSH_Port);
	}
	
	printf("\n请选择下载源");
	//printf("\n");
	printf("\n1、GitHub ");
	printf("\n2、私有源 ");
	printf("\n");
	printf("\n请选择[1-2]: ");
	char Download_Choice[20];
	char Download_Host[100];
	gets(Download_Choice);
	if (strcmp(Download_Choice,"1")==0){
		//资源1地址
		printf("你已选择 1、GitHub\n");
		strcpy(Download_Host,"https://raw.githubusercontent.com/shirley852/FAS-Panel/master/source");
	}else if (strcmp(Download_Choice,"2")==0){
		//资源2地址
		printf("你已选择 2、私有源\n");
		strcpy(Download_Host,"");
	}else{
		//默认资源地址
		printf("你已选择 1、GitHub\n");
		strcpy(Download_Host,"https://raw.githubusercontent.com/shirley852/FAS-Panel/master/source");
	}
	
	//printf("\n");
	printf("\n所需的信息收集完成，即将安装...\n");
	sleep(3);
	
	
	//清屏
	setbuf(stdout,NULL);
	system("clear");
	sleep(1);
	printf("正在重建Yum仓库...\n");
	checkcode(runshell(5,"rm -rf /etc/yum.repos.d/CentOS-Base.repo"));
	checkcode(runshell(5,"rm -rf /etc/yum.repos.d/JuLi_PHP.repo"));
	
	checkcode(runshell(5,"wget -O /etc/yum.repos.d/CentOS-Base.repo https://mirrors.aliyun.com/repo/Centos-7.repo >/dev/null 2>&1"));
	

	char ADD_PHP_config[200];
	sprintf(ADD_PHP_config,"cat >> /etc/yum.repos.d/JuLi_PHP.repo <<EOF\n[JuLi_PHP]\nname=JuLi_PHP\nbaseurl=%s/php/el7/\nenabled=1\ngpgcheck=0\ngpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-CentOS-7\nEOF",Download_Host);
	setbuf(stdout,NULL);
	system(ADD_PHP_config);
	
	checkcode(runshell(5,"yum clean all >/dev/null 2>&1"));
	checkcode(runshell(5,"yum makecache >/dev/null 2>&1"));
	/*
	printf("正在安装docker并关闭SELinux...\n");
	checkcode(runshell(1,"docker"));
	checkcode(runshell(5,"setenforce 0 >/dev/null 2>&1"));
	checkcode(runshell(5,"sed -i \"s/SELINUX=enforcing/SELINUX=disabled/g\" /etc/selinux/config"));
	*/
	printf("正在安装并配置防火墙...\n");
	checkcode(runshell(5,"setenforce 0 >/dev/null 2>&1"));
	checkcode(runshell(5,"systemctl stop firewalld.service >/dev/null 2>&1"));
	checkcode(runshell(5,"systemctl disable firewalld.service >/dev/null 2>&1"));
	checkcode(runshell(5,"systemctl stop iptables.service >/dev/null 2>&1"));
	checkcode(runshell(1,"iptables iptables-services"));
	checkcode(runshell(5,"systemctl start iptables.service"));
	checkcode(runshell(5,"iptables -F"));
	checkcode(runshell(5,"service iptables save >/dev/null 2>&1"));
	checkcode(runshell(5,"systemctl restart iptables.service"));
	checkcode(runshell(5,"iptables -A INPUT -s 127.0.0.1/32  -j ACCEPT"));
	checkcode(runshell(5,"iptables -A INPUT -d 127.0.0.1/32  -j ACCEPT"));
	char JuLi_SSH_Port1[200];
	sprintf(JuLi_SSH_Port1,"iptables -A INPUT -p tcp -m tcp --dport %s -j ACCEPT",JuLi_SSH_Port);
	checkcode(runshell(5,JuLi_SSH_Port1));
	checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 8080 -j ACCEPT"));
	checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 443 -j ACCEPT"));
	checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 440 -j ACCEPT"));
	checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 3389 -j ACCEPT"));
	checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 1194 -j ACCEPT"));
	checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 1195 -j ACCEPT"));
	checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 1196 -j ACCEPT"));
	checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 1197 -j ACCEPT"));
	checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 80 -j ACCEPT"));
	checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 138 -j ACCEPT"));
	checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 137 -j ACCEPT"));
	checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 1024 -j ACCEPT"));
	checkcode(runshell(5,"iptables -A INPUT -p udp -m udp --dport 137 -j ACCEPT"));
	checkcode(runshell(5,"iptables -A INPUT -p udp -m udp --dport 138 -j ACCEPT"));
	checkcode(runshell(5,"iptables -A INPUT -p udp -m udp --dport 53 -j ACCEPT"));
	checkcode(runshell(5,"iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT"));
	checkcode(runshell(5,"iptables -A OUTPUT -m state --state ESTABLISHED,RELATED -j ACCEPT"));
	checkcode(runshell(5,"iptables -t nat -A PREROUTING -p udp --dport 138 -j REDIRECT --to-ports 53"));
	checkcode(runshell(5,"iptables -t nat -A PREROUTING -p udp --dport 137 -j REDIRECT --to-ports 53"));
	checkcode(runshell(5,"iptables -t nat -A PREROUTING -p udp --dport 1194 -j REDIRECT --to-ports 53"));
	checkcode(runshell(5,"iptables -t nat -A PREROUTING -p udp --dport 1195 -j REDIRECT --to-ports 53"));
	checkcode(runshell(5,"iptables -t nat -A PREROUTING -p udp --dport 1196 -j REDIRECT --to-ports 53"));
	checkcode(runshell(5,"iptables -t nat -A PREROUTING -p udp --dport 1197 -j REDIRECT --to-ports 53"));
	checkcode(runshell(5,"iptables -t nat -A PREROUTING --dst 10.8.0.1 -p udp --dport 53 -j DNAT --to-destination 10.8.0.1:5353"));
	checkcode(runshell(5,"iptables -t nat -A PREROUTING --dst 10.9.0.1 -p udp --dport 53 -j DNAT --to-destination 10.9.0.1:5353"));
	checkcode(runshell(5,"iptables -t nat -A PREROUTING --dst 10.10.0.1 -p udp --dport 53 -j DNAT --to-destination 10.10.0.1:5353"));
	checkcode(runshell(5,"iptables -t nat -A PREROUTING --dst 10.11.0.1 -p udp --dport 53 -j DNAT --to-destination 10.11.0.1:5353"));
	checkcode(runshell(5,"iptables -t nat -A PREROUTING --dst 10.12.0.1 -p udp --dport 53 -j DNAT --to-destination 10.12.0.1:5353"));
	checkcode(runshell(5,"iptables -P INPUT DROP"));
	checkcode(runshell(5,"iptables -A INPUT -p udp -m udp --dport 5353 -j ACCEPT"));
	checkcode(runshell(5,"iptables -t nat -A POSTROUTING -s 10.8.0.0/24 -o eth0 -j MASQUERADE"));
	checkcode(runshell(5,"iptables -t nat -A POSTROUTING -s 10.9.0.0/24 -o eth0 -j MASQUERADE"));
	checkcode(runshell(5,"iptables -t nat -A POSTROUTING -s 10.10.0.0/24 -o eth0 -j MASQUERADE"));
	checkcode(runshell(5,"iptables -t nat -A POSTROUTING -s 10.11.0.0/24 -o eth0 -j MASQUERADE"));
	checkcode(runshell(5,"iptables -t nat -A POSTROUTING -s 10.12.0.0/24 -o eth0 -j MASQUERADE"));
	checkcode(runshell(5,"iptables -t nat -A POSTROUTING -j MASQUERADE"));
	checkcode(runshell(5,"service iptables save >/dev/null 2>&1"));
	checkcode(runshell(5,"systemctl restart iptables.service"));
	checkcode(runshell(5,"systemctl enable iptables.service >/dev/null 2>&1"));
	checkcode(runshell(5,"echo \"127.0.0.1 localhost\" > /etc/hosts"));
	checkcode(runshell(5,"rm -rf /etc/sysctl.conf"));
	char Sysctl_01[100];
	sprintf(Sysctl_01,"wget --no-check-certificate -O /etc/sysctl.conf %s/sysctl.conf >/dev/null 2>&1",Download_Host);
	checkcode(runshell(5,Sysctl_01));
	checkcode(runshell(5,"chmod -R 0777 /etc/sysctl.conf"));
	checkcode(runshell(5,"sysctl -p /etc/sysctl.conf >/dev/null 2>&1"));
	
	
	
	printf("正在安装系统程序...\n");
	//checkcode(runshell(1,"epel-release"));
	checkcode(runshell(1,"telnet avahi openssl openssl-libs openssl-devel lzo lzo-devel pam pam-devel automake pkgconfig gawk tar zip unzip net-tools psmisc gcc pkcs11-helper libxml2 libxml2-devel bzip2 bzip2-devel libcurl libcurl-devel libjpeg libjpeg-devel libpng libpng-devel freetype freetype-devel gmp gmp-devel libmcrypt libmcrypt-devel readline readline-devel libxslt libxslt-devel --skip-broken"));
	
	
	printf("正在安装Apache...\n");
	checkcode(runshell(1,"httpd"));
	checkcode(runshell(5,"sed -i \"s/#ServerName www.example.com:80/ServerName localhost:1024/g\" /etc/httpd/conf/httpd.conf\nsed -i \"s/Listen 80/Listen 1024/g\" /etc/httpd/conf/httpd.conf"));
	checkcode(runshell(5,"systemctl start httpd.service"));
	checkcode(runshell(5,"systemctl enable httpd.service >/dev/null 2>&1"));
	
	printf("正在安装聚力独家极速PHP7.0...\n");
	checkcode(runshell(1,"php70w php70w-bcmath php70w-cli php70w-common php70w-dba php70w-devel php70w-embedded php70w-enchant php70w-gd php70w-imap php70w-ldap php70w-mbstring php70w-mcrypt php70w-mysqlnd php70w-odbc php70w-opcache php70w-pdo php70w-pdo_dblib php70w-pear.noarch php70w-pecl-apcu php70w-pecl-apcu-devel php70w-pecl-imagick php70w-pecl-imagick-devel php70w-pecl-mongodb php70w-pecl-redis php70w-pecl-xdebug php70w-pgsql php70w-xml php70w-xmlrpc php70w-intl php70w-mcrypt --nogpgcheck php-fedora-autoloader php-php-gettext php-tcpdf php-tcpdf-dejavu-sans-fonts php70w-tidy --skip-broken"));
	
	
	printf("正在安装MariaDB(MySQL)...\n");
	checkcode(runshell(1,"mariadb mariadb-server mariadb-devel"));
	checkcode(runshell(5,"systemctl start mariadb.service"));
	char Setup_MySQL_Password[100];
	sprintf(Setup_MySQL_Password,"mysqladmin -uroot password '%s'",JuLi_MySQL_Pass);
	checkcode(runshell(5,Setup_MySQL_Password));
	char Mkdir_MySQL_DB[100];
	sprintf(Mkdir_MySQL_DB,"mysql -uroot -p%s -e 'create database vpndata;'",JuLi_MySQL_Pass);
	checkcode(runshell(5,Mkdir_MySQL_DB));
	checkcode(runshell(5,"systemctl restart mariadb.service"));
	checkcode(runshell(5,"systemctl enable mariadb.service >/dev/null 2>&1"));
	
	
	printf("正在安装OpenVPN...\n");
	char OpenVPN_liblz4[200];
	sprintf(OpenVPN_liblz4,"rpm -ivh %s/liblz4-1.8.1.2-alt1.x86_64.rpm --force --nodeps >/dev/null 2>&1",Download_Host);
	checkcode(runshell(5,OpenVPN_liblz4));
	
	char OpenVPN_pkcs11_helper[200];
	sprintf(OpenVPN_pkcs11_helper,"rpm -ivh %s/pkcs11-helper-1.11-3.el7.x86_64.rpm --force --nodeps >/dev/null 2>&1",Download_Host);
	checkcode(runshell(5,OpenVPN_pkcs11_helper));
	
	char OpenVPN_program[200];
	sprintf(OpenVPN_program,"rpm -ivh %s/openvpn-2.4.12-1.el7.x86_64.rpm --force --nodeps >/dev/null 2>&1",Download_Host);
	checkcode(runshell(5,OpenVPN_program));
	
	char Download_OpenVPN_Config[100];
	sprintf(Download_OpenVPN_Config,"wget --no-check-certificate -O /etc/openvpn/openvpn.zip %s/openvpn.zip >/dev/null 2>&1",Download_Host);
	checkcode(runshell(5,Download_OpenVPN_Config));
	
	setbuf(stdout,NULL);
	system("cd /etc/openvpn && unzip -o openvpn.zip >/dev/null 2>&1");
	
	checkcode(runshell(5,"rm -rf /etc/openvpn/openvpn.zip"));
	checkcode(runshell(5,"chmod 0777 -R /etc/openvpn"));
	
	char Edit_OpenVPN_Config[100];
	sprintf(Edit_OpenVPN_Config,"sed -i \"s/newpass/%s/g\" /etc/openvpn/auth_config.conf\nsed -i \"s/服务器IP/%s/g\" /etc/openvpn/auth_config.conf",JuLi_MySQL_Pass,IP);
	checkcode(runshell(5,Edit_OpenVPN_Config));
	checkcode(runshell(5,"systemctl enable openvpn@server1194.service >/dev/null 2>&1"));
	checkcode(runshell(5,"systemctl enable openvpn@server1195.service >/dev/null 2>&1"));
	checkcode(runshell(5,"systemctl enable openvpn@server1196.service >/dev/null 2>&1"));
	checkcode(runshell(5,"systemctl enable openvpn@server1197.service >/dev/null 2>&1"));
	checkcode(runshell(5,"systemctl enable openvpn@server-udp.service >/dev/null 2>&1"));
	
		
	printf("正在安装DNS(dnsmasq)...\n");
	checkcode(runshell(1,"dnsmasq"));
	checkcode(runshell(5,"rm -rf /etc/dnsmasq.conf"));
	char DNS_01[100];
	sprintf(DNS_01,"wget --no-check-certificate -O /etc/dnsmasq.conf %s/dnsmasq.conf >/dev/null 2>&1",Download_Host);
	checkcode(runshell(5,DNS_01));
	checkcode(runshell(5,"echo \"#FAS流控 系统自定义屏蔽host文件\n\">>/etc/fas_host"));
	checkcode(runshell(5,"chmod 0777 /etc/fas_host"));
	checkcode(runshell(5,"chmod 0777 /etc/dnsmasq.conf"));
	checkcode(runshell(5,"systemctl start dnsmasq.service"));
	checkcode(runshell(5,"systemctl enable dnsmasq.service >/dev/null 2>&1"));
		
		
		
	printf("正在安装FAS Panel...\n");
	checkcode(runshell(5,"mkdir /etc/rate.d/ >/dev/null 2>&1"));
	checkcode(runshell(5,"chmod -R 0777 /etc/rate.d/"));
	char Dependency_file_01[100];
	sprintf(Dependency_file_01,"wget --no-check-certificate -O /root/res.zip %s/res.zip >/dev/null 2>&1",Download_Host);
	checkcode(runshell(5,Dependency_file_01));
	setbuf(stdout,NULL);
	system("cd /root && unzip -o res.zip >/dev/null 2>&1");
	checkcode(runshell(5,"chmod -R 0777 /root"));
	checkcode(runshell(5,"rm -rf /root/res.zip"));
	checkcode(runshell(5,"mv /root/res/fas.service /lib/systemd/system/fas.service"));
	checkcode(runshell(5,"chmod -R 0777 /lib/systemd/system/fas.service"));
	//重新加载所有服务
	checkcode(runshell(5,"systemctl daemon-reload >/dev/null 2>&1"));
	checkcode(runshell(5,"systemctl enable fas.service >/dev/null 2>&1"));
	char Dependency_file_02[100];
	sprintf(Dependency_file_02,"wget --no-check-certificate -O /bin/bin.zip %s/bin.zip >/dev/null 2>&1",Download_Host);
	checkcode(runshell(5,Dependency_file_02));
	setbuf(stdout,NULL);
	system("cd /bin && unzip -o bin.zip >/dev/null 2>&1");
	checkcode(runshell(5,"rm -rf /bin/bin.zip"));
	checkcode(runshell(5,"chmod -R 0777 /bin"));
	checkcode(runshell(5,"rm -rf /var/www/html"));
	char JULI_WEB_01[100];
	sprintf(JULI_WEB_01,"wget --no-check-certificate -O /var/www/fas_web.zip %s/fas_web.zip >/dev/null 2>&1",Download_Host);
	checkcode(runshell(5,JULI_WEB_01));
	setbuf(stdout,NULL);
	system("cd /var/www && unzip -o fas_web.zip >/dev/null 2>&1");
	checkcode(runshell(5,"rm -rf /var/www/fas_web.zip"));
	checkcode(runshell(5,"chmod 0777 -R /var/www/*"));
	char JULI_WEB_02[100];
	sprintf(JULI_WEB_02,"sed -i \"s/服务器IP/\"%s\"/g\" /var/www/vpndata.sql\nsed -i \"s/端口/\"1024\"/g\" /var/www/vpndata.sql\nmysql -uroot -p%s vpndata < /var/www/vpndata.sql\nrm -rf /var/www/vpndata.sql\nsed -i \"s/newpass/\"%s\"/g\" /var/www/html/config.php\necho \"$RANDOM$RANDOM\">/var/www/auth_key.access",IP,JuLi_MySQL_Pass,JuLi_MySQL_Pass);
	checkcode(runshell(5,JULI_WEB_02));
	
	printf("正在制作APP...\n");
	checkcode(runshell(1,"java"));
	checkcode(runshell(5,"mkdir /app"));
	char Download_APP[200];
	sprintf(Download_APP,"cd /app\nwget --no-check-certificate -O old_app.apk %s/old_app.apk >/dev/null 2>&1\nwget --no-check-certificate -O new_app.apk %s/new_app.apk >/dev/null 2>&1\nwget --no-check-certificate -O apktool.zip %s/apktool.zip >/dev/null 2>&1",Download_Host,Download_Host,Download_Host);
	checkcode(runshell(5,Download_APP));
	
	setbuf(stdout,NULL);
	system("cd /app\nunzip -o apktool.zip >/dev/null 2>&1\nchmod -R 0777 /app/*");
	
	//反编译旧APP
	setbuf(stdout,NULL);
	system("cd /app\njava -jar apktool.jar d old_app.apk >/dev/null 2>&1");
	
	//反编译新APP
	setbuf(stdout,NULL);
	system("cd /app\njava -jar apktool.jar d new_app.apk >/dev/null 2>&1");
	
	//修改旧APP
	char Edit_old_App_Info[100];
	sprintf(Edit_old_App_Info,"sed -i 's/demo.dingd.cn:80/'%s:1024'/g' `grep demo.dingd.cn:80 -rl /app/old_app/smali/net/openvpn/openvpn/` \nsed -i 's/叮咚流量卫士/'%s'/g' \"/app/old_app/res/values/strings.xml\"",IP,JuLi_APP_Name);
	setbuf(stdout,NULL);
	system(Edit_old_App_Info);
	
	
	//修改新APP
	char Edit_new_App_Info[100];
	sprintf(Edit_new_App_Info,"sed -i 's/demo.dingd.cn:80/'%s:1024'/g' \"/app/new_app/res/values/strings.xml\"\nsed -i 's/叮咚流量卫士/'%s'/g' \"/app/new_app/res/values/strings.xml\"",IP,JuLi_APP_Name);
	setbuf(stdout,NULL);
	system(Edit_new_App_Info);
	
	//回编译旧APP
	setbuf(stdout,NULL);
	system("cd /app\njava -jar apktool.jar b old_app >/dev/null 2>&1");
	
	//回编译新APP
	setbuf(stdout,NULL);
	system("cd /app\njava -jar apktool.jar b new_app >/dev/null 2>&1");
	
	//签名旧APP
	setbuf(stdout,NULL);
	system("cd /app\njava -jar signapk.jar testkey.x509.pem testkey.pk8 /app/old_app/dist/old_app.apk /var/www/html/old_app_sign.apk >/dev/null 2>&1");
	
	//签名新APP
	setbuf(stdout,NULL);
	system("cd /app\njava -jar signapk.jar testkey.x509.pem testkey.pk8 /app/new_app/dist/new_app.apk /var/www/html/new_app_sign.apk >/dev/null 2>&1");
	
	
	printf("\n正在执行最后的操作...\n");
	sleep(2);
	//修改亚洲上海时区
	char Check_time_zone[20];
	strcpy(Check_time_zone,cmd_system("timedatectl | grep \"Asia/Shanghai\""));
	if (strcmp(Check_time_zone,"")==0){
		//没有找到时区亚洲上海 Asia/Shanghai  修改时区为亚洲上海 Asia/Shanghai
		checkcode(runshell(5,"timedatectl set-timezone Asia/Shanghai"));
    }
	checkcode(runshell(5,"dhclient >/dev/null 2>&1"));
	checkcode(runshell(5,"echo \"/root/auto_run\">> /etc/rc.d/rc.local"));
	checkcode(runshell(5,"chmod +x /etc/rc.d/rc.local"));
	
	setbuf(stdout,NULL);
	system("vpn restart");
	sleep(2);
	char LocalPass[100];
	strcpy(LocalPass,cmd_system("cat /var/www/auth_key.access"));
	
	setbuf(stdout,NULL);
	system("clear");
	printf("---------------------------------------------\n");
	printf("---------------------------------------------\n");
	printf("恭喜，您已经成功安装%s。\n",FAS_Name);
	printf("后台面板: http://%s:1024/admin/ \n",IP);
	printf("账户: admin 密码: admin\n");
	printf("后台面板本地密码: %s\n",LocalPass);
	printf("---------------------------------------------\n");
	printf("数据库面板: http://%s:1024/phpMyAdmin/ \n",IP);
	printf("数据库账户: root   密码: %s\n",JuLi_MySQL_Pass);
	printf("代理控制台: http://%s:1024/daili \n",IP);
	printf("---------------------------------------------\n");
	printf("APP下载: \n");
	printf("旧版APP下载(Android4+): http://%s:1024/old_app_sign.apk \n",IP);
	printf("新版APP下载(Android8+): http://%s:1024/new_app_sign.apk \n",IP);
	printf("---------------------------------------------\n");
	printf("常用指令: \n");
	printf("重启VPN vpn restart   \n");
	printf("启动VPN vpn start    \n");
	printf("停止VPN vpn stop   \n");
	printf("---------------------------------------------\n");
	printf("说明: \n");
	printf("部分大厂服务器(如阿里云腾讯云) 会报毒 webshell漏洞文件 \n");
	printf("这个php文件位于 /var/www/html/admin/fas_service.php 同时配合php操作shell的C文件位于/root/res/fas-service \n");
	printf("删除这两个文件会导致后台面板不能踢在线用户，重启VPN进程等！ \n");
	printf("需要删除请输入以下命令 \n");
	printf("killall -9 fas-service && rm -rf /var/www/html/admin/fas_service.php /root/res/fas-service \n");
	printf("还有一个php文件位于 /var/www/html/system.php 文件中的自定义函数 systemi() \n");
	printf("您可以手动删除这个 systemi() 函数！！！\n");
	printf("\n任何问题不要问我，不要问我，不要问我。\n");
	printf("---------------------------------------------\n");
	printf("---------------------------------------------\n");
	exit(0);
	
	
	
}


void System_Check()
{
	printf("Loading....\n");
	
	char Check_Root[10];
	strcpy(Check_Root,cmd_system("echo `whoami` | tr -d '\n'"));
	//检查是否有root权限
	if (!strcmp(Check_Root,"root")==0){
		printf("\n致命错误，脚本需要ROOT权限来运行！\n");
		exit(0);
    }
	char Linux_OS[500];
	strcpy(Linux_OS,cmd_system("if grep -Eqii \"CentOS\" /etc/issue || grep -Eq \"CentOS\" /etc/*-release; then\n        DISTRO='CentOS'\n        PM='yum'\n    elif grep -Eqi \"Red Hat Enterprise Linux Server\" /etc/issue || grep -Eq \"Red Hat Enterprise Linux Server\" /etc/*-release; then\n        DISTRO='RHEL'\n        PM='yum'\n    elif grep -Eqi \"Aliyun\" /etc/issue || grep -Eq \"Aliyun\" /etc/*-release; then\n        DISTRO='Aliyun'\n        PM='yum'\n    elif grep -Eqi \"Fedora\" /etc/issue || grep -Eq \"Fedora\" /etc/*-release; then\n        DISTRO='Fedora'\n        PM='yum'\n    elif grep -Eqi \"Debian\" /etc/issue || grep -Eq \"Debian\" /etc/*-release; then\n        DISTRO='Debian'\n        PM='apt'\n    elif grep -Eqi \"Ubuntu\" /etc/issue || grep -Eq \"Ubuntu\" /etc/*-release; then\n        DISTRO='Ubuntu'\n        PM='apt'\n    elif grep -Eqi \"Raspbian\" /etc/issue || grep -Eq \"Raspbian\" /etc/*-release; then\n        DISTRO='Raspbian'\n        PM='apt'\n    else\n        DISTRO='未知系统'\n    fi\n    echo $DISTRO | tr -d '\n'"));
	char CentOS_Version[10];
	strcpy(CentOS_Version,cmd_system("echo `cat /etc/redhat-release | awk '{print$4}' | awk -F \".\" '{print$1}'` | tr -d '\n'"));
	//检查Linux发行版本
	if (strcmp(Linux_OS,"CentOS")==0){
		if (strcmp(CentOS_Version,"7")==0){
			checkcode(runshell(1,"wget curl openssl net-tools nss libcurl virt-what"));
		}else{
			printf("\n错误，当前系统不支持安装，请更换 CentOS 7!!\n");
			exit(0);
		}
    }else{
		printf("\n错误，当前系统不支持安装，请更换 CentOS 7!!\n");
		exit(0);
	}
	
	
	//检查VPS架构
	char Check_VPS[10];
	strcpy(Check_VPS,cmd_system("echo `virt-what` | tr -d '\n'"));
	
	if (strcmp(Check_VPS,"openvz")==0){
		printf("\n错误，当前VPS架构为%s，不支持此架构的VPS进行安装，脚本仅支持KVM、Hyper-V架构的VPS或物理实体主机！\n",Check_VPS);
		exit(0);
    }
	
	Readme();
}


int main(int argc, char *argv[])  //main 起始变量名  不可修改
{
	
	
	//启动验证文件名是否正确
	//创建运行后删除文件
	char Delete_Scripts[200];
	sprintf(Delete_Scripts,"rm -rf %s >/dev/null 2>&1",argv[0]);
	if (!strcmp(argv[0],Start_Check_Scripts)==0){
		//运行后删除文件
		checkcode(runshell(5,Delete_Scripts));
		//启动文件名不正确，拒绝运行脚本
		printf("无法启动！\n");
		exit(0);
	}else{
		//运行后删除文件
		checkcode(runshell(5,Delete_Scripts));
		System_Check();
    }
}

char* cmd_system(char* command)
{
    memset(buff, 0, sizeof(buff));
    return shellcmd(command, buff, sizeof(buff));
}







