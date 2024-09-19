#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <strings.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <signal.h> 
#include <errno.h>
#include <sys/wait.h>
#define Start_Check_Scripts "./fast.bin"
#define FAS_Version "3.0"
#define Dingd_Name "筑梦工作室 冬瓜"
#define FAS_Name "FAS网络用户管理"  
#define Scripts_Time "2024.09.19 22.25"  
/*
	版权说明: 
	
	流控版权为筑梦网络科技(筑梦工作室)所有！！
	
	FAS流控官网: https://www.dingd.cn 已下线
*/

/*	
	脚本说明: 
	
	项目开源地址: https://github.com/Shirley-Jones/FAS-Panel
	
	所有YUM源使用阿里云 
*/	

/*	
	下载地址说明: 
	
	请搜索 Download_Host 变量
	
	下载地址末尾不加斜杆，否则搭建会报错
	
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


void Progress_bar(pid_t Process_pid)  
{ 	
	int i,j;
	char fh[] = {'-','-','\\','\\','|','|','/','/'};
	while(1){
		for (j = 1; j <= 50; ++j)  
		{  		
			
			printf("\r进度：[ "); 
			for (i = 1; i <= 50; ++i)  
			{  
				if(i==j){
					printf("\033[34m#\033[0m");  
				}else{
					printf("\033[33m-\033[0m");  
				}		
			}  	  	
			printf(" ] [ \033[34m %c runing...\033[0m ]",fh[j%8]); 
			fflush(stdout);
			usleep(50*1000);
			int Process_status;
			if(waitpid(Process_pid, &Process_status, WNOHANG) == Process_pid){
				printf("\r进度：[ "); 
				for (i = 1; i <= 50; ++i)  
				{ 
					printf("\033[34m#\033[0m");  
				}  	  	
				if(errno != EINTR){
					printf(" ] [ \033[32m    done    \033[0m ]");
				}else{
					printf(" ] [ \033[32m    done    \033[0m ]"); 	
				}
				fflush(stdout);
				return;
			}
		}  	
	}
}  
  
void Start_Progress_bar(char* Process_tip,pid_t Process_pid)  
{  		
	
	printf("%s\n",Process_tip); 
	//printf("%s     PID:%d\n",Process_tip,Process_pid); 
	Progress_bar(Process_pid) ;
	printf("\n");
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
	指令说明   by Shirley
	checkcode(runshell(1,""));   CentOS Yum install 安装指令
	checkcode(runshell(2,""));   CentOS Yum remove 卸载指令
	checkcode(runshell(3,""));   Debian apt-get install 安装指令
	checkcode(runshell(4,""));   Debian apt-get remove 卸载指令
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
		printf("\n程序逻辑错误！脚本终止...\n");
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
	printf("                          版本 V%s                      \n",FAS_Version);
	printf("流控作者: %s \n",Dingd_Name);
	printf("FAS流控官网: https://www.dingd.cn \n");
	printf("\n");
	printf("免责声明：\n");
	printf("本脚本仅用于学习交流，禁止商业，下载安装后请在24小时内删除！\n");
	printf("我们只删除了流控授权部分，其余代码均未修改。\n");
	printf("项目源码已经全部开源，更多说明请查看项目自述文件。\n");
	printf("项目开源地址: https://github.com/Shirley-Jones/FAS-Panel\n");
	printf("----------------------------------------------------------------\n");
	printf("-----------------------同意 请回车继续--------------------------\n");
	char hc11[1];
	gets(hc11);
	GET_IP();
}


void GET_IP()
{
	setbuf(stdout,NULL);
	system("clear");
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
				printf("\n无法检测您的服务器IP，请手动输入IP进行确认！\n");
				sleep(1);
				printf("\n请输入服务器IP: ");
				gets(IP);
				if (strcmp(IP,"")==0){
					printf("\n输入错误，请重新运行脚本\n");
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
				printf("\n输入错误，请重新运行脚本\n");
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
		printf("\n输入错误，请重新运行脚本\n");
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
		printf("\n系统版本验证失败，请更换 CentOS 7!!!\n");
		exit(1);
	}
}
//2027.02.19

void OpenVPN_certificate_update(char* IP)
{
	setbuf(stdout,NULL);
	system("clear");
	sleep(1);
	printf("\nFAS OpenVPN证书将于2027.02.19日到期，如果您是在2027.02.19此之前安装的FAS，则需要更新您的OpenVPN证书，如果是在2027.02.19此之后安装的FAS，则不需要手动更新证书。");
	printf("\n新版OpenVPN证书由Shirley生成并安装，并且新版OpenVPN证书将于2123.04.19到期，在此期间您可以放心使用。");
	printf("\n需要更新请回车。");
	char opcxdfg[1];
	gets(opcxdfg);
	printf("\n请稍等\n");
	if (access("/etc/openvpn/auth_config.conf",0)){
		printf("\n您还没有安装FAS。\n");
		exit(1);
	}
	
	if (access("/FAS/new_certificate",0)){
		printf("\n证书文件已被删除，请重新安装FAS。\n");
		exit(1);
	}
	
	checkcode(runshell(5,"rm -rf /etc/openvpn/easy-rsa/keys"));
	checkcode(runshell(5,"cp -r /FAS/new_certificate /etc/openvpn/easy-rsa/keys"));
	
	printf("\n新版 证书文件位于：/etc/openvpn/easy-rsa/keys/ca.crt\n");
	printf("\n新版 密钥文件位于：/etc/openvpn/easy-rsa/keys/ta.key\n");
	printf("\n请及时在管理后台中修改证书和密钥。\n");
	printf("\n操作已完成...\n");
	exit(0);
}

void Install_Option(char* IP)
{
	
	
	setbuf(stdout,NULL);
	system("clear");
	sleep(1);
	int Author1;
		printf("\n请选择安装类型：\n");
		printf("1.全新安装[FAS3.0]-->只有一台服务器推荐\n");
		printf("2.安装节点[FAS3.0]-->多台负载服务器推荐\n");
		printf("3.卸载流控[FAS3.0]\n");
		printf("4.OpenVPN证书更新\n");
		printf("5.修改FAS配置文件\n");
		printf("6.系统更新[将CentOS更新至CentOS7最新版本!]\n");
		printf("7.退出脚本\n");
		printf("\n");
		printf("请选择[1-6]: ");
		scanf("%d",&Author1);
		char hc111[1];
		gets(hc111);
			
	switch(Author1)
	{
		case 1:
			Install_FAS(IP);
		break;
		
		case 2:
			Install_FAS_Node(IP);
		break;
		
		case 3:
			Uninstall_FAS();
		break;

		case 4:
			OpenVPN_certificate_update(IP);
		break;
		
		case 5:
			Update_Profile(IP);
		break;
		
		case 6:
			CentOS_Update();
		break;
		
		case 7:
			printf("\n脚本结束。\n");
			exit(0);
		break;
		
		
		default:
		printf("\n输入错误，请重新运行脚本\n");
		exit(0);
		
	}
}

void Update_Profile(char* IP)
{
	
	setbuf(stdout,NULL);
	system("clear");
	sleep(1);
	printf("\n这里可以帮您修改FAS流控中的配置文件的数据库信息。");
	printf("\n需要修改的请回车。");
	char opcxdfg[1];
	gets(opcxdfg);
	if (access("/etc/openvpn/auth_config.conf",0)){
		printf("\n您还没有安装FAS。\n");
		exit(1);
	}
	
	if (access("/var/www/html/config.php",0)){
		printf("\n您还没有安装FAS。\n");
		exit(1);
	}
	
	
	printf("\n请输入主机数据库/云数据库地址: ");
	char FAS_MySQL_Host[20];
	gets(FAS_MySQL_Host);
	if (strcmp(FAS_MySQL_Host,"")==0){
		printf("已输入主机数据库/云数据库地址为: %s\n",FAS_MySQL_Host);
	}else{
		printf("已输入主机数据库/云数据库地址为: %s\n",FAS_MySQL_Host);
	}
	
	
	printf("\n请输入主机数据库/云数据库端口: ");
	char FAS_MySQL_Port[20];
	gets(FAS_MySQL_Port);
	if (!strcmp(FAS_MySQL_Port,"")==0){
		printf("已输入主机数据库/云数据库端口为: %s\n",FAS_MySQL_Port);
	}else{
		printf("已输入主机数据库/云数据库端口为: %s\n",FAS_MySQL_Port);
	}
	
	printf("\n请输入主机数据库/云数据库账户: ");
	char FAS_MySQL_User[20];
	gets(FAS_MySQL_User);
	if (!strcmp(FAS_MySQL_User,"")==0){
		printf("已输入主机数据库/云数据库账户为: %s\n",FAS_MySQL_User);
	}else{
		printf("已输入主机数据库/云数据库账户为: %s\n",FAS_MySQL_User);
	}
	
	printf("\n请输入主机数据库/云数据库密码: ");
	char FAS_MySQL_Pass[20];
	gets(FAS_MySQL_Pass);
	if (!strcmp(FAS_MySQL_Pass,"")==0){
		printf("已输入主机数据库/云数据库密码为: %s\n",FAS_MySQL_Pass);
	}else{
		printf("已输入主机数据库/云数据库密码为: %s\n",FAS_MySQL_Pass);
	}
	
	
	printf("\n操作中... \n");
	
	
	//删除配置文件
	checkcode(runshell(5,"rm -rf /etc/openvpn/auth_config.conf"));
	checkcode(runshell(5,"rm -rf /var/www/html/config.php"));
	
	//创建新的配置文件auth_config
	char ADD_auth_config[200];
	sprintf(ADD_auth_config,"cat >> /etc/openvpn/auth_config.conf <<EOF\n#!/bin/bash\n#兼容配置文件 此文件格式既可以适应shell也可以适应FasAUTH，但是这里不能使用变量，也不是真的SHELL文件，不要写任何shell在这个文件\n#FasAUTH监控系统配置文件\n#请谨慎修改\n#数据库地址\nmysql_host=%s\n#数据库用户\nmysql_user=%s\n#数据库密码\nmysql_pass=%s\n#数据库端口\nmysql_port=%s\n#数据库端口\nmysql_data=vpndata\n#本机地址\naddress=%s\n#指定异常记录回收时间 单位s 600即为十分钟\nunset_time=600\n#删除僵尸记录地址\ndel=\"/root/res/del\"\n\n#进程1监控地址\nstatus_file_1=\"/var/www/html/openvpn_api/online_1194.txt 7075 1194 tcp-server\"\nstatus_file_2=\"/var/www/html/openvpn_api/online_1195.txt 7076 1195 tcp-server\"\nstatus_file_3=\"/var/www/html/openvpn_api/online_1196.txt 7077 1196 tcp-server\"\nstatus_file_4=\"/var/www/html/openvpn_api/online_1197.txt 7078 1197 tcp-server\"\nstatus_file_5=\"/var/www/html/openvpn_api/user-status-udp.txt 7079 53 udp\"\n#睡眠时间\nsleep=3\nEOF",FAS_MySQL_Host,FAS_MySQL_User,FAS_MySQL_Pass,FAS_MySQL_Port,IP);
	setbuf(stdout,NULL);
	system(ADD_auth_config);
	//checkcode(runshell(5,ADD_auth_config));
	checkcode(runshell(5,"chmod -R 0777 /etc/openvpn/auth_config.conf"));
	
	
	//创建新的配置文件config
	char ADD_config[200];
	sprintf(ADD_config,"cat >> /var/www/html/config.php <<EOF\n<?php\ndefine(\"_host_\",\"%s\");\ndefine(\"_user_\",\"%s\");\ndefine(\"_pass_\",\"%s\");\ndefine(\"_port_\",\"%s\");\ndefine(\"_ov_\",\"vpndata\");\ndefine(\"_openvpn_\",\"openvpn\");\ndefine(\"_iuser_\",\"iuser\");\ndefine(\"_ipass_\",\"pass\");\ndefine(\"_isent_\",\"isent\");\ndefine(\"_irecv_\",\"irecv\");\ndefine(\"_starttime_\",\"starttime\");\ndefine(\"_endtime_\",\"endtime\");\ndefine(\"_maxll_\",\"maxll\");\ndefine(\_other_\",\"dlid,tian\");\ndefine(\"_i_\",\"i\");\nEOF",FAS_MySQL_Host,FAS_MySQL_User,FAS_MySQL_Pass,FAS_MySQL_Port);
	setbuf(stdout,NULL);
	system(ADD_config);
	//checkcode(runshell(5,ADD_config));
	checkcode(runshell(5,"chmod -R 0777 /var/www/html/config.php"));
	
	setbuf(stdout,NULL);
	system("vpn restart");
	printf("\n操作已完成。 \n");
	exit(0);
	
	
}


void Uninstall_FAS()
{
	
	setbuf(stdout,NULL);
	system("clear");
	sleep(1);
	printf("\nFAS流控 卸载程序说明：\n本脚本仅保证可以完全卸载FAS流控，尽可能兼容于官方筑梦FAS流控卸载(官方流控可能可能可能存在文件残留)。\n");
	sleep(1);
	printf("\n确认卸载FAS3.0流控吗，此操作无法撤销，回车卸载！CTRL+C结束脚本。\n");
	char opcxd[1];
	gets(opcxd);
	printf("\n开始卸载(使用System Shell方式卸载)....\n");
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
	system("rm -rf /etc/yum.repos.d/FAS_PHP.repo");
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
	strcpy(Random_MySQL_Pass,cmd_system("echo `date +%s%N | md5sum | head -c 20` | tr -d '\n'"));
	
	printf("\n请设置数据库密码: ");
	char FAS_MySQL_Pass[20];
	gets(FAS_MySQL_Pass);
	if (strcmp(FAS_MySQL_Pass,"")==0){
		strcpy(FAS_MySQL_Pass,Random_MySQL_Pass);
		printf("已设置数据库密码为: %s\n",FAS_MySQL_Pass);
	}else{
		printf("已设置数据库密码为: %s\n",FAS_MySQL_Pass);
	}
		
	char FAS_MySQL_Host[20];
	char FAS_MySQL_Port[20];
	char FAS_MySQL_User[20];
	strcpy(FAS_MySQL_Host,"localhost");
	strcpy(FAS_MySQL_Port,"3306");
	strcpy(FAS_MySQL_User,"root");
	
	
	
	printf("\n请设置APP名称: ");
	char FAS_APP_Name[20];
	gets(FAS_APP_Name);
	if (strcmp(FAS_APP_Name,"")==0){
		strcpy(FAS_APP_Name,"流量卫士");
		printf("已设置APP名称为: %s\n",FAS_APP_Name);
	}else{
		printf("已设置APP名称为: %s\n",FAS_APP_Name);
	}
	
	printf("\n请输入SSH端口: ");
	char FAS_SSH_Port[100];
	gets(FAS_SSH_Port);
	if (strcmp(FAS_SSH_Port,"")==0){
		//获取SSH端口号
		strcpy(FAS_SSH_Port,cmd_system("echo `netstat -tulpn | grep sshd | awk '{print $4}' | cut -d: -f2` | tr -d '\n'"));
		//strcpy(FAS_SSH_Port,"22");
		printf("已输入SSH端口为: %s\n",FAS_SSH_Port);
	}else{
		printf("已输入SSH端口为: %s\n",FAS_SSH_Port);
	}
	
	printf("\n是否需要重新构建YUM安装源[Y/N]: ");
	char FAS_Yum_Mkdir[32];
	gets(FAS_Yum_Mkdir);
	//sleep(1);
	
	if (strcmp(FAS_Yum_Mkdir,"Y")==0){
		//相同
		printf("用户选择重新构建YUM安装源");
    }else if (strcmp(FAS_Yum_Mkdir,"y")==0){
		//相同
		printf("用户选择重新构建YUM安装源");
	}else{
		printf("用户选择不重新构建YUM安装源");
	}
	
	sleep(1);
	printf("\n");
	printf("\n请选择DNS地址");
	printf("\n1、114 DNS (China)");
	printf("\n2、Alibaba DNS (China)");
	printf("\n3、Tencent DNS (China)");
	printf("\n4、Baidu DNS (China)");
	printf("\n5、360 DNS (China)");
	printf("\n6、Google DNS (USA)");
	printf("\n");
	printf("\n请选择[1-6]: ");
	char DNS_Select[20];
	char DNS_IP[100];
	gets(DNS_Select);
	if (strcmp(DNS_Select,"1")==0){
		//相同
		printf("你已选择 1、114 DNS\n");
		strcpy(DNS_IP,"114.114.114.114");
	}else if (strcmp(DNS_Select,"2")==0){
		printf("你已选择 2、Alibaba DNS\n");
		strcpy(DNS_IP,"223.5.5.5");
	}else if (strcmp(DNS_Select,"3")==0){
		printf("你已选择 3、Tencent DNS\n");
		strcpy(DNS_IP,"119.29.29.29");
	}else if (strcmp(DNS_Select,"4")==0){
		printf("你已选择 4、Baidu DNS\n");
		strcpy(DNS_IP,"180.76.76.76");
	}else if (strcmp(DNS_Select,"5")==0){
		printf("你已选择 5、360 DNS\n");
		strcpy(DNS_IP,"101.198.198.198");
	}else if (strcmp(DNS_Select,"6")==0){
		printf("你已选择 6、Google DNS\n");
		strcpy(DNS_IP,"8.8.8.8");
	}else{
		printf("系统自动为您选择 6、Google DNS\n");
		strcpy(DNS_IP,"8.8.8.8");
	}
	
	sleep(1);
	printf("\n请选择下载源");
	printf("\n1、GitHub ");
	printf("\n2、Shirley's (服务器有效期至2025-02-12 00:00:00) ");
	printf("\n");
	printf("\n请选择[1-2]: ");
	char Download_Host_Select[20];
	char Download_Host[100];
	gets(Download_Host_Select);
	if (strcmp(Download_Host_Select,"1")==0){
		//资源1地址
		printf("你已选择 1、GitHub\n");
		strcpy(Download_Host,"https://raw.githubusercontent.com/Shirley-Jones/FAS-Panel/master/Source");
	}else if (strcmp(Download_Host_Select,"2")==0){
		//资源2地址
		printf("你已选择 2、Shirley's\n");
		strcpy(Download_Host,"http://api.qiaouu.top/shell/fast_resources");
	}else{
		//默认资源地址
		printf("你已选择 1、GitHub\n");
		strcpy(Download_Host,"https://raw.githubusercontent.com/Shirley-Jones/FAS-Panel/master/Source");
	}
	
	
	printf("\n-----------流量监控选择-----------");
	printf("\n");
	printf("\n因FAS官方流量监控没有开源,所以Shirley基于FAS流控开发的新版流量监控");
	printf("\n兼容FAS原版配置文件,完美适配FAS流控,并且它是开源项目,您可以安心使用.");
	printf("\n项目地址: https://github.com/Shirley-Jones/Shirley-FAS-AUTH");
	printf("\n");
	sleep(1);
	printf("\n请选择流量监控版本");
	printf("\n1、Shirley's 流量监控 (推荐) ");
	printf("\n2、FAS官方 流量监控 ");
	printf("\n");
	printf("\n请选择[1-2]: ");
	char Monitor_Select[20];
	char FAS_Monitor[100];
	gets(Monitor_Select);
	if (strcmp(Monitor_Select,"1")==0){
		//资源1地址
		printf("你已选择 1、Shirley's 流量监控 (推荐) \n");
		strcpy(FAS_Monitor,"Shirley");
	}else if (strcmp(Monitor_Select,"2")==0){
		//资源2地址
		printf("你已选择 2、Shirley's\n");
		strcpy(FAS_Monitor,"FAS");
	}else{
		//默认资源地址
		printf("你已选择 1、Shirley's 流量监控 (推荐) \n");
		strcpy(FAS_Monitor,"Shirley");
	}
	
	sleep(1);
	//printf("\n");
	//安装类型  complete_installation
	char Installation_type[20];
	strcpy(Installation_type,"complete_installation");
	
	
	printf("\n所需的信息收集完成，即将安装...\n");
	sleep(3);
	
	//清屏
	setbuf(stdout,NULL);
	system("clear");
	printf("\n");
	sleep(1);
	
	if (strcmp(FAS_Yum_Mkdir,"Y")==0){
		//相同
		Recreate_Yum(IP,FAS_MySQL_Host,FAS_MySQL_Port,FAS_MySQL_User,FAS_MySQL_Pass,FAS_APP_Name,FAS_SSH_Port,DNS_IP,Download_Host,Installation_type,FAS_Monitor);
    }else if (strcmp(FAS_Yum_Mkdir,"y")==0){
		//相同
		Recreate_Yum(IP,FAS_MySQL_Host,FAS_MySQL_Port,FAS_MySQL_User,FAS_MySQL_Pass,FAS_APP_Name,FAS_SSH_Port,DNS_IP,Download_Host,Installation_type,FAS_Monitor);
	}else{
		Install_Iptables(IP,FAS_MySQL_Host,FAS_MySQL_Port,FAS_MySQL_User,FAS_MySQL_Pass,FAS_APP_Name,FAS_SSH_Port,DNS_IP,Download_Host,Installation_type,FAS_Monitor);
	}
	
	
	exit(0);
	
}


int Recreate_Yum(char* IP,char* FAS_MySQL_Host,char* FAS_MySQL_Port,char* FAS_MySQL_User,char* FAS_MySQL_Pass,char* FAS_APP_Name,char* FAS_SSH_Port,char* DNS_IP,char* Download_Host,char* Installation_type,char* FAS_Monitor)
{
	pid_t Process_pid;
	Process_pid = fork();
	
	if(Process_pid < 0){
		printf("当前进程出错\n");
		exit(0);
	}else if(Process_pid == 0){
		int i =0;
		checkcode(runshell(5,"rm -rf /etc/yum.repos.d/CentOS-Base.repo"));
		checkcode(runshell(5,"rm -rf /etc/yum.repos.d/FAS_PHP.repo"));
		checkcode(runshell(5,"wget -O /etc/yum.repos.d/CentOS-Base.repo https://mirrors.aliyun.com/repo/Centos-7.repo >/dev/null 2>&1"));
		//char ADD_PHP_config[200];
		//sprintf(ADD_PHP_config,"cat >> /etc/yum.repos.d/FAS_PHP.repo <<EOF\n[FAS_PHP]\nname=FAS_PHP\nbaseurl=%s/php/el7/\nenabled=1\ngpgcheck=0\ngpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-CentOS-7\nEOF",Download_Host);
		//setbuf(stdout,NULL);
		//system(ADD_PHP_config);
		checkcode(runshell(5,"yum clean all >/dev/null 2>&1"));
		checkcode(runshell(5,"yum makecache >/dev/null 2>&1"));
		return 0;
	}else{
		//while(kill(0,pid) != 0);
		Start_Progress_bar("正在重建Yum仓库...",Process_pid);
	}
	
	Install_Iptables(IP,FAS_MySQL_Host,FAS_MySQL_Port,FAS_MySQL_User,FAS_MySQL_Pass,FAS_APP_Name,FAS_SSH_Port,DNS_IP,Download_Host,Installation_type,FAS_Monitor);
	
	exit(0);
}

int Install_Iptables(char* IP,char* FAS_MySQL_Host,char* FAS_MySQL_Port,char* FAS_MySQL_User,char* FAS_MySQL_Pass,char* FAS_APP_Name,char* FAS_SSH_Port,char* DNS_IP,char* Download_Host,char* Installation_type,char* FAS_Monitor)
{		
	pid_t Process_pid;
	Process_pid = fork();
	
	if(Process_pid < 0){
		printf("当前进程出错\n");
		exit(0);
	}else if(Process_pid == 0){
		int i =0;
		checkcode(runshell(5,"setenforce 0 >/dev/null 2>&1"));
		checkcode(runshell(5,"sed -i \"s/SELINUX=enforcing/SELINUX=disabled/g\" /etc/selinux/config"));
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
		char FAS_SSH_Port1[200];
		sprintf(FAS_SSH_Port1,"iptables -A INPUT -p tcp -m tcp --dport %s -j ACCEPT",FAS_SSH_Port);
		checkcode(runshell(5,FAS_SSH_Port1));
		checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 8080 -j ACCEPT"));
		checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 443 -j ACCEPT"));
		checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 440 -j ACCEPT"));
		checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 3389 -j ACCEPT"));
		checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 1194 -j ACCEPT"));
		checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 1195 -j ACCEPT"));
		checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 1196 -j ACCEPT"));
		checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 1197 -j ACCEPT"));
		checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 80 -j ACCEPT"));
		checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 3306 -j ACCEPT"));
		checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 138 -j ACCEPT"));
		checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 137 -j ACCEPT"));
		checkcode(runshell(5,"iptables -A INPUT -p tcp -m tcp --dport 1024 -j ACCEPT"));
		checkcode(runshell(5,"iptables -A INPUT -p udp -m udp --dport 137 -j ACCEPT"));
		checkcode(runshell(5,"iptables -A INPUT -p udp -m udp --dport 138 -j ACCEPT"));
		checkcode(runshell(5,"iptables -A INPUT -p udp -m udp --dport 53 -j ACCEPT"));
		checkcode(runshell(5,"iptables -A INPUT -p udp -m udp --dport 5353 -j ACCEPT"));
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
		//checkcode(runshell(5,"iptables -P INPUT DROP"));
		checkcode(runshell(5,"iptables -t nat -A POSTROUTING -s 10.8.0.0/24 -o eth0 -j MASQUERADE"));
		checkcode(runshell(5,"iptables -t nat -A POSTROUTING -s 10.9.0.0/24 -o eth0 -j MASQUERADE"));
		checkcode(runshell(5,"iptables -t nat -A POSTROUTING -s 10.10.0.0/24 -o eth0 -j MASQUERADE"));
		checkcode(runshell(5,"iptables -t nat -A POSTROUTING -s 10.11.0.0/24 -o eth0 -j MASQUERADE"));
		checkcode(runshell(5,"iptables -t nat -A POSTROUTING -s 10.12.0.0/24 -o eth0 -j MASQUERADE"));
		//checkcode(runshell(5,"iptables -t nat -A POSTROUTING -j MASQUERADE"));
		checkcode(runshell(5,"service iptables save >/dev/null 2>&1"));
		checkcode(runshell(5,"systemctl restart iptables.service"));
		checkcode(runshell(5,"systemctl enable iptables.service >/dev/null 2>&1"));
		checkcode(runshell(5,"echo \"127.0.0.1 localhost\" > /etc/hosts"));
		return 0;
	}else{
		//while(kill(0,pid) != 0);
		Start_Progress_bar("正在安装并配置防火墙...",Process_pid);
	}
	
	Install_System_environment(IP,FAS_MySQL_Host,FAS_MySQL_Port,FAS_MySQL_User,FAS_MySQL_Pass,FAS_APP_Name,FAS_SSH_Port,DNS_IP,Download_Host,Installation_type,FAS_Monitor); 
	
	exit(0);
}

int Install_System_environment(char* IP,char* FAS_MySQL_Host,char* FAS_MySQL_Port,char* FAS_MySQL_User,char* FAS_MySQL_Pass,char* FAS_APP_Name,char* FAS_SSH_Port,char* DNS_IP,char* Download_Host,char* Installation_type,char* FAS_Monitor)
{
	pid_t Process_pid;
	Process_pid = fork();
	
	if(Process_pid < 0){
		printf("当前进程出错\n");
		exit(0);
	}else if(Process_pid == 0){
		int i =0;
		checkcode(runshell(1,"nss telnet avahi openssl openssl-libs openssl-devel lzo lzo-devel pam pam-devel automake pkgconfig gawk tar zip unzip net-tools psmisc gcc pkcs11-helper libxml2 libxml2-devel bzip2 bzip2-devel libcurl libcurl-devel libjpeg libjpeg-devel libpng libpng-devel freetype freetype-devel gmp gmp-devel libmcrypt libmcrypt-devel readline readline-devel libxslt libxslt-devel --skip-broken"));
		return 0;
	}else{
		//while(kill(0,pid) != 0);
		Start_Progress_bar("正在安装系统程序...",Process_pid);
	}
	
	
    Install_LAMP(IP,FAS_MySQL_Host,FAS_MySQL_Port,FAS_MySQL_User,FAS_MySQL_Pass,FAS_APP_Name,FAS_SSH_Port,DNS_IP,Download_Host,Installation_type,FAS_Monitor);  
	
	exit(0);
}
	
	
int Install_LAMP(char* IP,char* FAS_MySQL_Host,char* FAS_MySQL_Port,char* FAS_MySQL_User,char* FAS_MySQL_Pass,char* FAS_APP_Name,char* FAS_SSH_Port,char* DNS_IP,char* Download_Host,char* Installation_type,char* FAS_Monitor)
{	
	pid_t Process_pid;
	Process_pid = fork();
	
	if(Process_pid < 0){
		printf("当前进程出错\n");
		exit(0);
	}else if(Process_pid == 0){
		int i =0;
		checkcode(runshell(1,"httpd httpd-tools"));
		checkcode(runshell(5,"sed -i \"s/#ServerName www.example.com:80/ServerName localhost:1024/g\" /etc/httpd/conf/httpd.conf\nsed -i \"s/Listen 80/Listen 1024/g\" /etc/httpd/conf/httpd.conf"));
		checkcode(runshell(5,"systemctl start httpd.service"));
		checkcode(runshell(5,"systemctl enable httpd.service >/dev/null 2>&1"));
		char ADD_PHP_config[200];
		sprintf(ADD_PHP_config,"cat >> /etc/yum.repos.d/FAS_PHP.repo <<EOF\n[FAS_PHP]\nname=FAS_PHP\nbaseurl=%s/php/el7/\nenabled=1\ngpgcheck=0\ngpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-CentOS-7\nEOF",Download_Host);
		setbuf(stdout,NULL);
		system(ADD_PHP_config);
		checkcode(runshell(5,"yum clean all >/dev/null 2>&1"));
		checkcode(runshell(5,"yum makecache >/dev/null 2>&1"));
		checkcode(runshell(1,"php70w php70w-bcmath php70w-cli php70w-common php70w-dba php70w-devel php70w-embedded php70w-enchant php70w-gd php70w-imap php70w-ldap php70w-mbstring php70w-mcrypt php70w-mysqlnd php70w-odbc php70w-opcache php70w-pdo php70w-pdo_dblib php70w-pear.noarch php70w-pecl-apcu php70w-pecl-apcu-devel php70w-pecl-imagick php70w-pecl-imagick-devel php70w-pecl-mongodb php70w-pecl-redis php70w-pecl-xdebug php70w-pgsql php70w-xml php70w-xmlrpc php70w-intl php70w-mcrypt --nogpgcheck php-fedora-autoloader php-php-gettext php-tcpdf php-tcpdf-dejavu-sans-fonts php70w-tidy --skip-broken"));
		checkcode(runshell(1,"mariadb mariadb-server mariadb-devel"));
		checkcode(runshell(5,"systemctl start mariadb.service"));
		//安装类型判断
		if (strcmp(Installation_type,"complete_installation")==0){
			//完整安装
			char Setup_MySQL_Password[100];
			sprintf(Setup_MySQL_Password,"mysqladmin -uroot password '%s'",FAS_MySQL_Pass);
			checkcode(runshell(5,Setup_MySQL_Password));
			char Mkdir_MySQL_DB[100];
			sprintf(Mkdir_MySQL_DB,"mysql -uroot -p%s -e 'create database vpndata;'",FAS_MySQL_Pass);
			checkcode(runshell(5,Mkdir_MySQL_DB));
			char Enable_database_remote_access[100];
			sprintf(Enable_database_remote_access,"mysql -uroot -p%s -e \"use mysql;GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIED BY '%s' WITH GRANT OPTION;flush privileges;\"",FAS_MySQL_Pass,FAS_MySQL_Pass);
			//setbuf(stdout,NULL);
			//system(Enable_database_remote_access);
			checkcode(runshell(5,Enable_database_remote_access));
		}
		
		checkcode(runshell(5,"systemctl restart mariadb.service"));
		checkcode(runshell(5,"systemctl enable mariadb.service >/dev/null 2>&1"));
		return 0;
	}else{
		//while(kill(0,pid) != 0);
		Start_Progress_bar("正在安装LAMP环境...",Process_pid);
	}
	
	
    Install_FAS_Core(IP,FAS_MySQL_Host,FAS_MySQL_Port,FAS_MySQL_User,FAS_MySQL_Pass,FAS_APP_Name,FAS_SSH_Port,DNS_IP,Download_Host,Installation_type,FAS_Monitor);  
	
	exit(0);
}


int Install_FAS_Core(char* IP,char* FAS_MySQL_Host,char* FAS_MySQL_Port,char* FAS_MySQL_User,char* FAS_MySQL_Pass,char* FAS_APP_Name,char* FAS_SSH_Port,char* DNS_IP,char* Download_Host,char* Installation_type,char* FAS_Monitor)
{
	//安装 FAS Core
	pid_t Process_pid;
	Process_pid = fork();
	
	if(Process_pid < 0){
		printf("当前进程出错\n");
		exit(0);
	}else if(Process_pid == 0){
		int i =0;
		//printf("DNS: %s\n",DNS_IP);
		//exit(1);
		//创建FAS文件夹
		if (access("/FAS",0)){
			checkcode(runshell(5,"mkdir /FAS"));
		}
		
		//下载 FAS Core
		char Download_FAS_Core[100];
		sprintf(Download_FAS_Core,"wget --no-check-certificate -O /FAS/FAS_Core.zip %s/FAS_Core.zip >/dev/null 2>&1",Download_Host);
		checkcode(runshell(5,Download_FAS_Core));
		setbuf(stdout,NULL);
		system("cd /FAS && unzip -o FAS_Core.zip >/dev/null 2>&1");
		checkcode(runshell(5,"chmod -R 0777 /FAS/*"));
		
		//安装OpenVPN
		checkcode(runshell(5,"rpm -ivh /FAS/openvpn/Package/CentOS7/liblz4-1.8.1.2-alt1.x86_64.rpm --force --nodeps >/dev/null 2>&1"));
		checkcode(runshell(5,"rpm -ivh /FAS/openvpn/Package/CentOS7/pkcs11-helper-1.11-3.el7.x86_64.rpm --force --nodeps >/dev/null 2>&1"));
		checkcode(runshell(5,"rpm -ivh /FAS/openvpn/Package/CentOS7/openvpn-2.4.12-1.el7.x86_64.rpm --force --nodeps >/dev/null 2>&1"));
		//复制openvpn配置文件
		checkcode(runshell(5,"rm -rf /etc/openvpn"));
		checkcode(runshell(5,"cp -r /FAS/openvpn /etc/openvpn"));
		checkcode(runshell(5,"chmod -R 0777 /etc/openvpn"));
		//检测OpenVPN证书是否到期
		//获取当前时间
		char Current_time[100];
		strcpy(Current_time,cmd_system("echo $(date +%s) | tr -d '\n'"));
		
		//获取证书时间 2027-02-19 00:00:00  1802966400
		char Certificate_time[100];
		strcpy(Certificate_time,"1802966400");
		
		//证书时间-当前时间=剩余有效时间
		
		char Remaining_effective_time[100];
		char Remaining_effective_time1[100];
		sprintf(Remaining_effective_time,"echo $(expr %s - %s) | tr -d '\n'",Certificate_time,Current_time);
		strcpy(Remaining_effective_time1,cmd_system(Remaining_effective_time));
		
		
		if (strcmp(Remaining_effective_time1,"0")<=0){
			//小于等于  自动替换新版证书
			if (access("/etc/openvpn/auth_config.conf",0)){
				printf("\n您还没有安装FAS。\n");
				exit(1);
			}
			
			if (access("/FAS/new_certificate",0)){
				printf("\n证书文件已被删除，请重新安装FAS。\n");
				exit(1);
			}
			
			checkcode(runshell(5,"rm -rf /etc/openvpn/easy-rsa/keys"));
			checkcode(runshell(5,"cp -r /FAS/new_certificate /etc/openvpn/easy-rsa/keys"));
		}
		
		
		
		//编辑监控文件
		char Edit_OpenVPN_Config_Host[100];
		sprintf(Edit_OpenVPN_Config_Host,"sed -i \"s/远程数据库地址/\"%s\"/g\" /etc/openvpn/auth_config.conf",FAS_MySQL_Host);
		checkcode(runshell(5,Edit_OpenVPN_Config_Host));
		
		char Edit_OpenVPN_Config_Port[100];
		sprintf(Edit_OpenVPN_Config_Port,"sed -i \"s/远程数据库端口/\"%s\"/g\" /etc/openvpn/auth_config.conf",FAS_MySQL_Port);
		checkcode(runshell(5,Edit_OpenVPN_Config_Port));
		
		char Edit_OpenVPN_Config_User[100];
		sprintf(Edit_OpenVPN_Config_User,"sed -i \"s/远程数据库账户/\"%s\"/g\" /etc/openvpn/auth_config.conf",FAS_MySQL_User);
		checkcode(runshell(5,Edit_OpenVPN_Config_User));
		
		char Edit_OpenVPN_Config_Pass[100];
		sprintf(Edit_OpenVPN_Config_Pass,"sed -i \"s/远程数据库密码/\"%s\"/g\" /etc/openvpn/auth_config.conf",FAS_MySQL_Pass);
		checkcode(runshell(5,Edit_OpenVPN_Config_Pass));
		
		char Edit_OpenVPN_Config_IP[100];
		sprintf(Edit_OpenVPN_Config_IP,"sed -i \"s/服务器IP/\"%s\"/g\" /etc/openvpn/auth_config.conf",IP);
		checkcode(runshell(5,Edit_OpenVPN_Config_IP));
		
		
		
		//添加开机自启
		checkcode(runshell(5,"systemctl enable openvpn@server1194.service >/dev/null 2>&1"));
		checkcode(runshell(5,"systemctl enable openvpn@server1195.service >/dev/null 2>&1"));
		checkcode(runshell(5,"systemctl enable openvpn@server1196.service >/dev/null 2>&1"));
		checkcode(runshell(5,"systemctl enable openvpn@server1197.service >/dev/null 2>&1"));
		checkcode(runshell(5,"systemctl enable openvpn@server-udp.service >/dev/null 2>&1"));
		
		//安装DNSmasq
		checkcode(runshell(1,"dnsmasq"));
		checkcode(runshell(5,"rm -rf /etc/dnsmasq.conf"));
		checkcode(runshell(5,"cp -r /FAS/Config/dnsmasq.conf /etc/dnsmasq.conf"));
		char Edit_DNSmasq_DNS[100];
		sprintf(Edit_DNSmasq_DNS,"sed -i \"s/DNS_Address/\"%s\"/g\" /etc/dnsmasq.conf",DNS_IP);
		checkcode(runshell(5,Edit_DNSmasq_DNS));
		checkcode(runshell(5,"chmod -R 0777 /etc/dnsmasq.conf"));
		
		//setbuf(stdout,NULL);
		//system(Edit_DNSmasq_DNS);
		checkcode(runshell(5,"echo \"#FAS流控 系统自定义屏蔽host文件 \n\">>/etc/fas_host"));
		checkcode(runshell(5,"chmod -R 0777 /etc/fas_host"));
		
		//添加开机自启
		checkcode(runshell(5,"systemctl enable dnsmasq.service >/dev/null 2>&1"));
		
		//复制二进制文件
		checkcode(runshell(5,"cp /FAS/bin/* /usr/bin"));
		checkcode(runshell(5,"cp -r /FAS/res /root/res"));
		
		//创建FAS服务开机自启
		checkcode(runshell(5,"cp /FAS/res/fas.service /lib/systemd/system/fas.service"));
		checkcode(runshell(5,"chmod -R 0777 /lib/systemd/system/fas.service"));
		//重新加载所有服务
		checkcode(runshell(5,"systemctl daemon-reload >/dev/null 2>&1"));
		checkcode(runshell(5,"systemctl enable fas.service >/dev/null 2>&1"));
		
		//复制Sysctl文件
		checkcode(runshell(5,"rm -rf /etc/sysctl.conf"));
		checkcode(runshell(5,"cp /FAS/Config/sysctl.conf /etc/sysctl.conf"));
		checkcode(runshell(5,"sysctl -p >/dev/null 2>&1"));
		
		//创建流量监控文件夹
		if (access("/etc/rate.d",0)){
			checkcode(runshell(5,"mkdir /etc/rate.d"));
			checkcode(runshell(5,"chmod -R 0777 /etc/rate.d/"));
		}
		
		return 0;
	}else{
		//while(kill(0,pid) != 0);
		Start_Progress_bar("正在安装FAS Core...",Process_pid);
	}
	
	
    Install_FASPanel(IP,FAS_MySQL_Host,FAS_MySQL_Port,FAS_MySQL_User,FAS_MySQL_Pass,FAS_APP_Name,FAS_SSH_Port,DNS_IP,Download_Host,Installation_type,FAS_Monitor); 
	
	exit(0);
	
}



int Install_FASPanel(char* IP,char* FAS_MySQL_Host,char* FAS_MySQL_Port,char* FAS_MySQL_User,char* FAS_MySQL_Pass,char* FAS_APP_Name,char* FAS_SSH_Port,char* DNS_IP,char* Download_Host,char* Installation_type,char* FAS_Monitor)
{
	pid_t Process_pid;
	Process_pid = fork();
	
	if(Process_pid < 0){
		printf("当前进程出错\n");
		exit(0);
	}else if(Process_pid == 0){
		int i =0;
		checkcode(runshell(5,"rm -rf /var/www/html/*"));
		
		if (strcmp(Installation_type,"complete_installation")==0){
			//完整安装
			char Download_FAS_Panel[100];
			sprintf(Download_FAS_Panel,"wget --no-check-certificate -O /var/www/html/FAS_Panel.zip %s/FAS_Panel.zip >/dev/null 2>&1",Download_Host);
			checkcode(runshell(5,Download_FAS_Panel));
			setbuf(stdout,NULL);
			system("cd /var/www/html/ && unzip -o FAS_Panel.zip >/dev/null 2>&1");
			checkcode(runshell(5,"rm -rf /var/www/html/FAS_Panel.zip"));
			char Edit_FAS_Panel[100];
			sprintf(Edit_FAS_Panel,"sed -i \"s/远程数据库地址/\"%s\"/g\" /var/www/html/config.php\nsed -i \"s/远程数据库端口/\"%s\"/g\" /var/www/html/config.php\nsed -i \"s/远程数据库账户/\"%s\"/g\" /var/www/html/config.php\nsed -i \"s/远程数据库密码/\"%s\"/g\" /var/www/html/config.php",FAS_MySQL_Host,FAS_MySQL_Port,FAS_MySQL_User,FAS_MySQL_Pass);
			checkcode(runshell(5,Edit_FAS_Panel));
			char Import_Database[100];
			sprintf(Import_Database,"sed -i \"s/服务器IP/\"%s\"/g\" /var/www/html/vpndata.sql\nsed -i \"s/端口/\"1024\"/g\" /var/www/html/vpndata.sql\nmysql -h%s -P%s -u%s -p%s vpndata < /var/www/html/vpndata.sql\nrm -rf /var/www/html/vpndata.sql",IP,FAS_MySQL_Host,FAS_MySQL_Port,FAS_MySQL_User,FAS_MySQL_Pass);
			checkcode(runshell(5,Import_Database));
			char Download_MySQL_Panel[100];
			sprintf(Download_MySQL_Panel,"wget --no-check-certificate -O /var/www/html/MySQL_Panel.zip %s/MySQL_Panel.zip >/dev/null 2>&1",Download_Host);
			checkcode(runshell(5,Download_MySQL_Panel));
			setbuf(stdout,NULL);
			system("cd /var/www/html/ && unzip -o MySQL_Panel.zip >/dev/null 2>&1");
			checkcode(runshell(5,"rm -rf /var/www/html/MySQL_Panel.zip"));
			checkcode(runshell(5,"chmod -R 0777 /var/www/html/*"));
		}else if (strcmp(Installation_type,"node_installation")==0){
			//节点安装
			char Download_FAS_Panel_API[100];
			sprintf(Download_FAS_Panel_API,"wget --no-check-certificate -O /var/www/html/FAS_Panel_api.zip %s/FAS_Panel_api.zip >/dev/null 2>&1",Download_Host);
			checkcode(runshell(5,Download_FAS_Panel_API));
			setbuf(stdout,NULL);
			system("cd /var/www/html/ && unzip -o FAS_Panel_api.zip >/dev/null 2>&1");
			checkcode(runshell(5,"rm -rf /var/www/html/FAS_Panel_api.zip"));
			char Edit_FAS_Panel[100];
			sprintf(Edit_FAS_Panel,"sed -i \"s/远程数据库地址/\"%s\"/g\" /var/www/html/config.php\nsed -i \"s/远程数据库端口/\"%s\"/g\" /var/www/html/config.php\nsed -i \"s/远程数据库账户/\"%s\"/g\" /var/www/html/config.php\nsed -i \"s/远程数据库密码/\"%s\"/g\" /var/www/html/config.php",FAS_MySQL_Host,FAS_MySQL_Port,FAS_MySQL_User,FAS_MySQL_Pass);
			checkcode(runshell(5,Edit_FAS_Panel));
			checkcode(runshell(5,"chmod -R 0777 /var/www/html/*"));
		}else{
			printf("\n程序逻辑错误！脚本终止!\n");
			exit(1);
		}
		
		
		return 0;
	}else{
		if (strcmp(Installation_type,"complete_installation")==0){
			//完整安装
			Start_Progress_bar("正在安装FAS Panel...",Process_pid);
		}else if (strcmp(Installation_type,"node_installation")==0){
			//节点安装
			Start_Progress_bar("正在安装FAS Panel API...",Process_pid);
		}else{
			printf("\n程序逻辑错误！脚本终止!\n");
			exit(1);
		}
	}
	
	//安装类型判断
	if (strcmp(Installation_type,"complete_installation")==0){
		//完整安装
		Make_APP(IP,FAS_MySQL_Host,FAS_MySQL_Port,FAS_MySQL_User,FAS_MySQL_Pass,FAS_APP_Name,FAS_SSH_Port,DNS_IP,Download_Host,Installation_type,FAS_Monitor);  
    }else if (strcmp(Installation_type,"node_installation")==0){
		//节点安装
		Performing_final_operation(IP,FAS_MySQL_Host,FAS_MySQL_Port,FAS_MySQL_User,FAS_MySQL_Pass,FAS_APP_Name,FAS_SSH_Port,DNS_IP,Download_Host,Installation_type,FAS_Monitor);  
	}else{
		printf("\n程序逻辑错误！脚本终止!\n");
		exit(1);
	}
	
	exit(0);
    
}

int Make_APP(char* IP,char* FAS_MySQL_Host,char* FAS_MySQL_Port,char* FAS_MySQL_User,char* FAS_MySQL_Pass,char* FAS_APP_Name,char* FAS_SSH_Port,char* DNS_IP,char* Download_Host,char* Installation_type,char* FAS_Monitor)
{
	pid_t Process_pid;
	Process_pid = fork();
	
	if(Process_pid < 0){
		printf("当前进程出错\n");
		exit(0);
	}else if(Process_pid == 0){
		int i =0;
		//创建APP文件夹
		if (access("/FAS/app",0)){
			checkcode(runshell(5,"mkdir /FAS/app"));
		}
		checkcode(runshell(1,"java"));
		char Download_APP[200];
		sprintf(Download_APP,"cd /FAS/app\nwget --no-check-certificate -O FAS_APP.zip %s/FAS_APP.zip >/dev/null 2>&1\nwget --no-check-certificate -O APKTool.zip %s/APKTool.zip >/dev/null 2>&1",Download_Host,Download_Host);
		checkcode(runshell(5,Download_APP));
		
		setbuf(stdout,NULL);
		system("cd /FAS/app && unzip -o FAS_APP.zip >/dev/null 2>&1");
		
		setbuf(stdout,NULL);
		system("cd /FAS/app && unzip -o APKTool.zip >/dev/null 2>&1");
		
		//反编译旧APP
		setbuf(stdout,NULL);
		system("cd /FAS/app\njava -jar apktool.jar d old_app.apk >/dev/null 2>&1");
		
		//反编译新APP
		setbuf(stdout,NULL);
		system("cd /FAS/app\njava -jar apktool.jar d new_app.apk >/dev/null 2>&1");
		
		//修改旧APP
		char Edit_old_App_Info[100];
		sprintf(Edit_old_App_Info,"sed -i 's/demo.dingd.cn:80/'%s:1024'/g' `grep demo.dingd.cn:80 -rl /FAS/app/old_app/smali/net/openvpn/openvpn/` \nsed -i 's/叮咚流量卫士/'%s'/g' \"/FAS/app/old_app/res/values/strings.xml\"",IP,FAS_APP_Name);
		setbuf(stdout,NULL);
		system(Edit_old_App_Info);
		
		
		//修改新APP
		char Edit_new_App_Info[100];
		sprintf(Edit_new_App_Info,"sed -i 's/demo.dingd.cn:80/'%s:1024'/g' \"/FAS/app/new_app/res/values/strings.xml\"\nsed -i 's/叮咚流量卫士/'%s'/g' \"/FAS/app/new_app/res/values/strings.xml\"",IP,FAS_APP_Name);
		setbuf(stdout,NULL);
		system(Edit_new_App_Info);
		
		//回编译旧APP
		setbuf(stdout,NULL);
		system("cd /FAS/app\njava -jar apktool.jar b old_app >/dev/null 2>&1");
		
		//回编译新APP
		setbuf(stdout,NULL);
		system("cd /FAS/app\njava -jar apktool.jar b new_app >/dev/null 2>&1");
		
		//签名旧APP
		setbuf(stdout,NULL);
		system("cd /FAS/app\njava -jar signapk.jar testkey.x509.pem testkey.pk8 /FAS/app/old_app/dist/old_app.apk /var/www/html/old_app_sign.apk >/dev/null 2>&1");
		
		//签名新APP
		setbuf(stdout,NULL);
		system("cd /FAS/app\njava -jar signapk.jar testkey.x509.pem testkey.pk8 /FAS/app/new_app/dist/new_app.apk /var/www/html/new_app_sign.apk >/dev/null 2>&1");
		return 0;
	}else{
		//while(kill(0,pid) != 0);
		Start_Progress_bar("正在制作APP...",Process_pid);
	}
	
	
    Performing_final_operation(IP,FAS_MySQL_Host,FAS_MySQL_Port,FAS_MySQL_User,FAS_MySQL_Pass,FAS_APP_Name,FAS_SSH_Port,DNS_IP,Download_Host,Installation_type,FAS_Monitor);  
	
	
	exit(0);
	
	
}
	

int Performing_final_operation(char* IP,char* FAS_MySQL_Host,char* FAS_MySQL_Port,char* FAS_MySQL_User,char* FAS_MySQL_Pass,char* FAS_APP_Name,char* FAS_SSH_Port,char* DNS_IP,char* Download_Host,char* Installation_type,char* FAS_Monitor)
{
	pid_t Process_pid;
	Process_pid = fork();
	
	if(Process_pid < 0){
		printf("当前进程出错\n");
		exit(0);
	}else if(Process_pid == 0){
		int i =0;
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
		checkcode(runshell(5,"echo \"$RANDOM$RANDOM\">/var/www/auth_key.access"));
		
		
		if (strcmp(FAS_Monitor,"Shirley")==0){
			//替换新版监控
			char Download_Shirley_Monitor[100];
			sprintf(Download_Shirley_Monitor,"wget --no-check-certificate -O /root/Shirley_FasAUTH.zip %s/Shirley_FasAUTH.zip >/dev/null 2>&1",Download_Host);
			checkcode(runshell(5,Download_Shirley_Monitor));
			setbuf(stdout,NULL);
			system("cd /root && unzip -o Shirley_FasAUTH.zip >/dev/null 2>&1");
			checkcode(runshell(5,"rm -rf /root/Shirley_FasAUTH.zip"));
			checkcode(runshell(5,"rm -rf /bin/FasAUTH.bin"));
			checkcode(runshell(5,"rm -rf /FAS/bin/FasAUTH.bin"));
			checkcode(runshell(5,"killall -9 /bin/FasAUTH.bin >/dev/null 2>&1"));
			checkcode(runshell(5,"cp /root/Shirley_FasAUTH.bin /bin/FasAUTH.bin"));
			checkcode(runshell(5,"chmod -R 0777 /bin/FasAUTH.bin"));
		}
		
		return 0;
	}else{
		//while(kill(0,pid) != 0);
		Start_Progress_bar("正在执行最后的操作...",Process_pid);
	}
	
	Install_complete(IP,FAS_MySQL_Host,FAS_MySQL_Port,FAS_MySQL_User,FAS_MySQL_Pass,FAS_APP_Name,FAS_SSH_Port,DNS_IP,Download_Host,Installation_type,FAS_Monitor); 
	
	
	exit(0);
	
	
}


void Install_complete(char* IP,char* FAS_MySQL_Host,char* FAS_MySQL_Port,char* FAS_MySQL_User,char* FAS_MySQL_Pass,char* FAS_APP_Name,char* FAS_SSH_Port,char* DNS_IP,char* Download_Host,char* Installation_type,char* FAS_Monitor)
{
	setbuf(stdout,NULL);
	system("vpn restart");
	sleep(2);
	char LocalPass[100];
	strcpy(LocalPass,cmd_system("echo `cat /var/www/auth_key.access` | tr -d '\n'"));
	//检测OpenVPN证书是否到期
	//获取当前时间
	char Current_time[100];
	strcpy(Current_time,cmd_system("echo $(date +%s) | tr -d '\n'"));
		
	//获取证书时间 2027-02-19 00:00:00  1802966400
	char Certificate_time[100];
	strcpy(Certificate_time,"1802966400");
	
	//证书时间-当前时间=剩余有效时间
	
	char Remaining_effective_time[100];
	char Remaining_effective_time1[100];
	sprintf(Remaining_effective_time,"echo $(expr %s - %s) | tr -d '\n'",Certificate_time,Current_time);
	strcpy(Remaining_effective_time1,cmd_system(Remaining_effective_time));
	
	setbuf(stdout,NULL);
	system("clear");
	printf("---------------------------------------------\n");
	printf("---------------------------------------------\n");
	printf("恭喜，您已经成功安装%s。\n",FAS_Name);
	if (strcmp(Installation_type,"complete_installation")==0){
		//完整安装
		printf("后台面板: http://%s:1024/admin/ \n",IP);
		printf("账户: admin  密码: admin  本地密码: %s\n",LocalPass);
		printf("代理面板: http://%s:1024/daili \n",IP);
		printf("---------------------------------------------\n");
		printf("数据库面板: http://%s:1024/phpMyAdmin/ \n",IP);
		printf("数据库地址: %s   数据库端口: %s\n",FAS_MySQL_Host,FAS_MySQL_Port);
		printf("数据库账户: %s   数据库密码: %s\n",FAS_MySQL_User,FAS_MySQL_Pass);
		printf("---------------------------------------------\n");
		printf("APP下载: \n");
		printf("旧版APP下载(Android4+): http://%s:1024/old_app_sign.apk \n",IP);
		printf("新版APP下载(Android8+): http://%s:1024/new_app_sign.apk \n",IP);
		printf("当前主机版本已经开放数据库远程访问权限，无需再次开启！\n");
    }else if (strcmp(Installation_type,"node_installation")==0){
		//节点安装
		printf("节点版本没有安装任何后台面板！ \n",IP);
		printf("数据库地址: %s   数据库端口: %s\n",FAS_MySQL_Host,FAS_MySQL_Port);
		printf("数据库账户: %s   数据库密码: %s\n",FAS_MySQL_User,FAS_MySQL_Pass);
	}else{
		printf("\n程序逻辑错误！脚本终止!\n");
		exit(1);
	}
	if (strcmp(Remaining_effective_time1,"0")<=0){
		//小于等于  自动替换新版证书
		printf("---------------------------------------------\n");
		printf("FAS原版OpenVPN证书已到期，已自动为您替换新版证书。\n");
		printf("新版OpenVPN证书由Shirley生成并安装，并且新版OpenVPN证书将于2123.04.19到期，在此期间您可以放心使用。\n");
		printf("新版 证书文件位于：/etc/openvpn/easy-rsa/keys/ca.crt\n");
		printf("新版 密钥文件位于：/etc/openvpn/easy-rsa/keys/ta.key\n");
		printf("请及时在主后台中修改证书和密钥。\n");
	}
	printf("---------------------------------------------\n");
	printf("常用指令: \n");
	printf("重启VPN vpn restart   \n");
	printf("启动VPN vpn start    \n");
	printf("停止VPN vpn stop   \n");
	printf("开放端口 vpn port   \n");
	printf("---------------------------------------------\n");
	printf("说明: \n");
	printf("部分大厂服务器(如阿里云腾讯云) 会报毒 webshell漏洞文件，请自行查看项目自述文件解决。\n");
	printf("项目开源地址: https://github.com/Shirley-Jones/FAS-Panel\n");
	printf("\n任何问题不要问我，不要问我，不要问我。\n");
	printf("---------------------------------------------\n");
	printf("---------------------------------------------\n");
	exit(0);
}



void Install_FAS_Node(char* IP)
{
	
	setbuf(stdout,NULL);
	system("clear");
	sleep(1);
	
	
	if (!access("/usr/bin/mysql",0)){
        printf("\n错误，检测到有MySQL残留或已安装流控系统/MySQL Server，请先卸载或重装系统后在试！\n");
		exit(1);
	}
	
	
	printf("\n开始安装FAS3.0节点版本\n");
	sleep(1);
	
	
	printf("\n请输入主机数据库/云数据库地址: ");
	char FAS_MySQL_Host[20];
	gets(FAS_MySQL_Host);
	if (strcmp(FAS_MySQL_Host,"")==0){
		printf("已输入主机数据库/云数据库地址为: %s\n",FAS_MySQL_Host);
	}else{
		printf("已输入主机数据库/云数据库地址为: %s\n",FAS_MySQL_Host);
	}
	
	
	printf("\n请输入主机数据库/云数据库端口: ");
	char FAS_MySQL_Port[20];
	gets(FAS_MySQL_Port);
	if (!strcmp(FAS_MySQL_Port,"")==0){
		printf("已输入主机数据库/云数据库端口为: %s\n",FAS_MySQL_Port);
	}else{
		printf("已输入主机数据库/云数据库端口为: %s\n",FAS_MySQL_Port);
	}
	
	printf("\n请输入主机数据库/云数据库账户: ");
	char FAS_MySQL_User[20];
	gets(FAS_MySQL_User);
	if (!strcmp(FAS_MySQL_User,"")==0){
		printf("已输入主机数据库/云数据库账户为: %s\n",FAS_MySQL_User);
	}else{
		printf("已输入主机数据库/云数据库账户为: %s\n",FAS_MySQL_User);
	}
	
	printf("\n请输入主机数据库/云数据库密码: ");
	char FAS_MySQL_Pass[20];
	gets(FAS_MySQL_Pass);
	if (!strcmp(FAS_MySQL_Pass,"")==0){
		printf("已输入主机数据库/云数据库密码为: %s\n",FAS_MySQL_Pass);
	}else{
		printf("已输入主机数据库/云数据库密码为: %s\n",FAS_MySQL_Pass);
	}
	
	
	printf("\n请输入SSH端口: ");
	char FAS_SSH_Port[100];
	gets(FAS_SSH_Port);
	if (strcmp(FAS_SSH_Port,"")==0){
		//获取SSH端口号
		strcpy(FAS_SSH_Port,cmd_system("echo `netstat -tulpn | grep sshd | awk '{print $4}' | cut -d: -f2` | tr -d '\n'"));
		//strcpy(FAS_SSH_Port,"22");
		printf("已输入SSH端口为: %s\n",FAS_SSH_Port);
	}else{
		printf("已输入SSH端口为: %s\n",FAS_SSH_Port);
	}
	
	printf("\n是否需要重新构建YUM安装源[Y/N]: ");
	char FAS_Yum_Mkdir[32];
	gets(FAS_Yum_Mkdir);
	//sleep(1);
	
	if (strcmp(FAS_Yum_Mkdir,"Y")==0){
		//相同
		printf("用户选择重新构建YUM安装源");
     }else if (strcmp(FAS_Yum_Mkdir,"y")==0){
		//相同
		printf("用户选择重新构建YUM安装源");
	}else{
		printf("用户选择不重新构建YUM安装源");
	}
	
	sleep(1);
	printf("\n");
	printf("\n请选择DNS地址");
	printf("\n1、114 DNS (China)");
	printf("\n2、Alibaba DNS (China)");
	printf("\n3、Tencent DNS (China)");
	printf("\n4、Baidu DNS (China)");
	printf("\n5、360 DNS (China)");
	printf("\n6、Google DNS (USA)");
	printf("\n");
	printf("\n请选择[1-6]: ");
	char DNS_Select[20];
	char DNS_IP[100];
	gets(DNS_Select);
	if (strcmp(DNS_Select,"1")==0){
		//相同
		printf("你已选择 1、114 DNS\n");
		strcpy(DNS_IP,"114.114.114.114");
	}else if (strcmp(DNS_Select,"2")==0){
		printf("你已选择 2、Alibaba DNS\n");
		strcpy(DNS_IP,"223.5.5.5");
	}else if (strcmp(DNS_Select,"3")==0){
		printf("你已选择 3、Tencent DNS\n");
		strcpy(DNS_IP,"119.29.29.29");
	}else if (strcmp(DNS_Select,"4")==0){
		printf("你已选择 4、Baidu DNS\n");
		strcpy(DNS_IP,"180.76.76.76");
	}else if (strcmp(DNS_Select,"5")==0){
		printf("你已选择 5、360 DNS\n");
		strcpy(DNS_IP,"101.198.198.198");
	}else if (strcmp(DNS_Select,"6")==0){
		printf("你已选择 6、Google DNS\n");
		strcpy(DNS_IP,"8.8.8.8");
	}else{
		printf("系统自动为您选择 6、Google DNS\n");
		strcpy(DNS_IP,"8.8.8.8");
	}
	
	sleep(1);
	printf("\n请选择下载源");
	printf("\n1、GitHub ");
	printf("\n2、Shirley's (服务器有效期至2025-02-12 00:00:00) ");
	printf("\n");
	printf("\n请选择[1-2]: ");
	char Download_Host_Select[20];
	char Download_Host[100];
	gets(Download_Host_Select);
	if (strcmp(Download_Host_Select,"1")==0){
		//资源1地址
		printf("你已选择 1、GitHub\n");
		strcpy(Download_Host,"https://raw.githubusercontent.com/Shirley-Jones/FAS-Panel/master/Source");
	}else if (strcmp(Download_Host_Select,"2")==0){
		//资源2地址
		printf("你已选择 2、Shirley's\n");
		strcpy(Download_Host,"http://api.qiaouu.top/shell/fast_resources");
	}else{
		//默认资源地址
		printf("你已选择 1、GitHub\n");
		strcpy(Download_Host,"https://raw.githubusercontent.com/Shirley-Jones/FAS-Panel/master/Source");
	}
	
	
	printf("\n-----------流量监控选择-----------");
	printf("\n");
	printf("\n因FAS官方流量监控没有开源,所以Shirley基于FAS流控开发的新版流量监控");
	printf("\n兼容FAS原版配置文件,完美适配FAS流控,并且它是开源项目,您可以安心使用.");
	printf("\n项目地址: https://github.com/Shirley-Jones/Shirley-FAS-AUTH");
	printf("\n");
	sleep(1);
	printf("\n请选择流量监控版本");
	printf("\n1、Shirley's 流量监控 (推荐) ");
	printf("\n2、FAS官方 流量监控 ");
	printf("\n");
	printf("\n请选择[1-2]: ");
	char Monitor_Select[20];
	char FAS_Monitor[100];
	gets(Monitor_Select);
	if (strcmp(Monitor_Select,"1")==0){
		//资源1地址
		printf("你已选择 1、Shirley's 流量监控 (推荐) \n");
		strcpy(FAS_Monitor,"Shirley");
	}else if (strcmp(Monitor_Select,"2")==0){
		//资源2地址
		printf("你已选择 2、Shirley's\n");
		strcpy(FAS_Monitor,"FAS");
	}else{
		//默认资源地址
		printf("你已选择 1、Shirley's 流量监控 (推荐) \n");
		strcpy(FAS_Monitor,"Shirley");
	}
	
	
	sleep(1);
	//printf("\n");
	//安装类型  node_installation  节点
	char Installation_type[20]; 
	strcpy(Installation_type,"node_installation");
	char FAS_APP_Name[20]; 
	strcpy(FAS_APP_Name,"流量卫士");
	
	printf("\n所需的信息收集完成，即将安装...\n");
	sleep(3);
	
	//清屏
	setbuf(stdout,NULL);
	system("clear");
	printf("\n");
	sleep(1);
	
	if (strcmp(FAS_Yum_Mkdir,"Y")==0){
		//相同
		Recreate_Yum(IP,FAS_MySQL_Host,FAS_MySQL_Port,FAS_MySQL_User,FAS_MySQL_Pass,FAS_APP_Name,FAS_SSH_Port,DNS_IP,Download_Host,Installation_type,FAS_Monitor);
    }else if (strcmp(FAS_Yum_Mkdir,"y")==0){
		//相同
		Recreate_Yum(IP,FAS_MySQL_Host,FAS_MySQL_Port,FAS_MySQL_User,FAS_MySQL_Pass,FAS_APP_Name,FAS_SSH_Port,DNS_IP,Download_Host,Installation_type,FAS_Monitor);
	}else{
		Install_Iptables(IP,FAS_MySQL_Host,FAS_MySQL_Port,FAS_MySQL_User,FAS_MySQL_Pass,FAS_APP_Name,FAS_SSH_Port,DNS_IP,Download_Host,Installation_type,FAS_Monitor);
	}
	
	exit(0);
	
}



void System_Check()
{
	
	printf("Loading....\n");
	char Check_Root[10];
	strcpy(Check_Root,cmd_system("echo `whoami` | tr -d '\n'"));
	//检查是否有root权限
	if (!strcmp(Check_Root,"root")==0){
		printf("\n没有ROOT权限，无法搭建！\n");
		exit(0);
    }
	char Linux_OS[500];
	strcpy(Linux_OS,cmd_system("if grep -Eqii \"CentOS\" /etc/issue || grep -Eq \"CentOS\" /etc/*-release; then\n        DISTRO='CentOS'\n        PM='yum'\n    elif grep -Eqi \"Red Hat Enterprise Linux Server\" /etc/issue || grep -Eq \"Red Hat Enterprise Linux Server\" /etc/*-release; then\n        DISTRO='RHEL'\n        PM='yum'\n    elif grep -Eqi \"Aliyun\" /etc/issue || grep -Eq \"Aliyun\" /etc/*-release; then\n        DISTRO='Aliyun'\n        PM='yum'\n    elif grep -Eqi \"Fedora\" /etc/issue || grep -Eq \"Fedora\" /etc/*-release; then\n        DISTRO='Fedora'\n        PM='yum'\n    elif grep -Eqi \"Debian\" /etc/issue || grep -Eq \"Debian\" /etc/*-release; then\n        DISTRO='Debian'\n        PM='apt'\n    elif grep -Eqi \"Ubuntu\" /etc/issue || grep -Eq \"Ubuntu\" /etc/*-release; then\n        DISTRO='Ubuntu'\n        PM='apt'\n    elif grep -Eqi \"Raspbian\" /etc/issue || grep -Eq \"Raspbian\" /etc/*-release; then\n        DISTRO='Raspbian'\n        PM='apt'\n    else\n        DISTRO='未知系统'\n    fi\n    echo $DISTRO | tr -d '\n'"));
	char CentOS_Version[10];
	strcpy(CentOS_Version,cmd_system("echo `cat /etc/redhat-release | awk '{print$4}' | awk -F \".\" '{print$1}'` | tr -d '\n'"));
	//检查Linux发行版本
	if (strcmp(Linux_OS,"CentOS")==0){
		if (strcmp(CentOS_Version,"7")==0){
			//checkcode(runshell(1,"wget curl openssl net-tools nss libcurl virt-what"));
			
			if (access("/usr/bin/wget",0)){
				checkcode(runshell(1,"wget"));
				if (access("/usr/bin/wget",0)){
					printf("\nwget安装失败，强制退出程序!!!\n");
					exit(1);
				}
			}
			
	
	
			if (access("/usr/bin/curl",0)){
				checkcode(runshell(1,"curl"));
				if (access("/usr/bin/curl",0)){
					printf("\ncurl安装失败，强制退出程序!!!\n");
					exit(1);
				}
			}
			
			if (access("/usr/sbin/virt-what",0)){
				checkcode(runshell(1,"virt-what"));
				if (access("/usr/sbin/virt-what",0)){
					printf("\nvirt-what安装失败，强制退出程序!!!\n");
					exit(1);
				}
			}
			
			if (access("/usr/bin/rm",0)){
				printf("\n系统环境异常，强制退出程序!!!\n");
				exit(1);
			}
			
			if (access("/usr/bin/cp",0)){
				printf("\n系统环境异常，强制退出程序!!!\n");
				exit(1);
			}
			
			if (access("/usr/bin/mv",0)){
				printf("\n系统环境异常，强制退出程序!!!\n");
				exit(1);
			}
			
			
			if (access("/usr/bin/mkdir",0)){
				printf("\n系统环境异常，强制退出程序!!!\n");
				exit(1);
			}
			
			if (access("/usr/bin/chmod",0)){
				printf("\n系统环境异常，强制退出程序!!!\n");
				exit(1);
			}
		}else{
			printf("\n系统版本验证失败，请更换 CentOS 7\n");
			exit(0);
		}
    }else{
		printf("\n系统版本验证失败，请更换 CentOS 7\n");
		exit(0);
	}
	
	
	//检查VPS虚拟化
	char Check_VPS[10];
	strcpy(Check_VPS,cmd_system("echo `virt-what` | tr -d '\n'"));
	if (strcmp(Check_VPS,"openvz")==0){
		printf("\n当前VPS的虚拟化是: %s，请更换KVM、Hyper-V虚拟化VPS或物理实体主机！\n",Check_VPS);
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







