/*
*	Centos服务器宽带监控程序
*	
*	使用方法：
*		首先在新建/etc/rate.d/目录 并且赋予0777权限
*		运行本程序 会在上述提到的目录中 创建 hour.json 和 minute.json 文件 
*		前者记录 当天24小时 的宽带流速 后者记录 30分钟以内宽带流速（均为平均值）
*		RX是下行  TX是上行 对应的数据 *8/1024/1024 得到即为 Mbps/s
*		数据以JSON的格式保存
*		程序有诸多不完善之处 还望大虾自己研究 修改
*
*/
#include <stdio.h> 
#include <string.h> 
#include <time.h> 
#include <stdlib.h>
#include <signal.h>
 
 
#define SLEEP_TIME 1
 
 
char* get(char* str);
char* getstr();
void getinfo(char* line);
void start();
void start_service();
 
struct dev{
	char devname[128];
	long info[16];
};
 
struct RATE_LOG{
	int day ;
	int hour;
	int minute;
	int second;
	long rx;
	long tx;
};
 
char * pTmp;
struct dev eth0;
 
struct RATE_LOG minute_bw[30];
struct RATE_LOG hour_bw[24];
 
long last_tx = 0;
long last_rx = 0;
 
 
 
int main(){
	
	signal(SIGCLD, SIG_IGN);//通知内核 子进程一旦结束立即释放资源
	
	printf("Centos 实时宽带检测系统 测试版\n");
	pid_t fpid = fork();
	if(fpid == 0){
		start_service();
	}
	printf("\n流控监控服务     [已启动]\nPID:        %d\n",fpid);
	return 0;
}
 
void start_service(){
	int d = 0;
	int last_day = 0;
	int last_minute = 0;
	int last_second = 0;
	int last_hour = 0;
		
	int minute = 0;
	int day = 0;
	int hour = 0;
	
	long hour_rx_count = 0;
	long hour_tx_count = 0;
		
	time_t timep;  
	struct tm *p;  
	int j;
	
	/*
		初始化 分钟宽带 监控数据
	*/
	for(j=0;j<30;j++){
		minute_bw[j].rx = 0;
		minute_bw[j].tx = 0;
		minute_bw[j].minute = 0;
		minute_bw[j].hour = 0;
		
	}
	
	/*
		初始化 小时宽带 监控数据
	*/
	for(j=0;j<24;j++){
		hour_bw[j].hour = j+1;
		hour_bw[j].rx = 0;
		hour_bw[j].tx = 0;
	}
	
	
	time(&timep);  
	p = localtime(&timep);
	last_minute = p->tm_min;
	last_day = p->tm_mday;
	last_hour = p->tm_hour;
		
	start();
 
	last_rx = eth0.info[0];
	last_tx = eth0.info[8];
	
	int k = 0;//一共逝去了多少分钟
	
	//system("clear");
	while(1){	
	
		time(&timep);  
		p = localtime(&timep);
		minute = p->tm_min;
		day = p->tm_mday;
		hour = p->tm_hour;
		
		if(minute != last_minute)
		{
			//过去了一分钟
			int rx_bw = (eth0.info[0] - last_rx)/60; //计算得出下行宽带平均每秒钟使用的流量
			int tx_bw = (eth0.info[8] - last_tx)/60;  //计算得出上行宽带平均每秒钟使用的流量
			
			hour_rx_count += rx_bw;
			hour_tx_count += tx_bw;
			
			
			last_rx = eth0.info[0];
			last_tx = eth0.info[8];
			int i;
			for(i=0;i<29;i++){
				minute_bw[i] = minute_bw[i+1];
			}
			minute_bw[29].rx = rx_bw;
			minute_bw[29].tx = tx_bw;
			minute_bw[29].minute = minute;
			minute_bw[29].hour = hour;
			if(fork() == 0){
				//格式化成JSON数据保存 之所以在单独的线程中 是为了尽量避免影响流量统计的延时
				char buffer[512];
				FILE *fp = fopen("/etc/rate.d/minute.json","w");
				fputs("{",fp);
				for(i=0;i<30;i++){
					if(i == 29){
						sprintf(buffer,"\"%d:%d\":{\"rx\":\"%d\",\"tx\":\"%d\"}\n",minute_bw[i].hour,minute_bw[i].minute,minute_bw[i].rx,minute_bw[i].tx);
					}else{
						sprintf(buffer,"\"%d:%d\":{\"rx\":\"%d\",\"tx\":\"%d\"},\n",minute_bw[i].hour,minute_bw[i].minute,minute_bw[i].rx,minute_bw[i].tx);
					}
					fputs(buffer,fp);
				}
				fputs("}",fp);
				fclose(fp);
				exit(getpid());
			}
			k++;
		}
		
		if(hour != last_hour){
			hour_bw[hour-1].hour = hour;
			hour_bw[hour-1].rx = hour_rx_count / k; 
			hour_bw[hour-1].tx = hour_tx_count / k;
			k=0;
			hour_rx_count = 0;
			hour_tx_count = 0;
			if(fork() == 0){
				//格式化成JSON数据保存 之所以在单独的线程中 是为了尽量避免影响流量统计的延时
				int i;
				char buffer[512];
				FILE *fp = fopen("/etc/rate.d/hour.json","w");
				fputs("{",fp);
				for(i=0;i<24;i++){
					if(i == 23){
						sprintf(buffer,"\"%d\":{\"rx\":\"%d\",\"tx\":\"%d\"}\n",hour_bw[i].hour,hour_bw[i].rx,hour_bw[i].tx);
					}else{
						sprintf(buffer,"\"%d\":{\"rx\":\"%d\",\"tx\":\"%d\"},\n",hour_bw[i].hour,hour_bw[i].rx,hour_bw[i].tx);
					}
					fputs(buffer,fp);
				}
				fputs("}",fp);
				fclose(fp);
				exit(getpid());
			}
		}
		
		
		last_day = day;
		last_minute = minute;
		last_hour = hour;
		start();
		sleep(SLEEP_TIME);
		
	}
}
 
//解析/proc/net/dev文件 并将流量数据格式化 这里的识别逻辑有待优化
void start(){
	
	char buffer[1024];
	FILE *fp = fopen("/proc/net/dev","r");
	fgets(buffer,1024,fp);
	fgets(buffer,1024,fp);
	while(fgets(buffer,1024,fp)){
		char *p = strstr(buffer,"tun");
		if(NULL == strstr(buffer,"tun") && NULL == strstr(buffer,"lo"))
		{
			//排除虚拟网卡和lo网卡 剩下的应该就是真实的网卡了
			getinfo(buffer);
			break; //跳出循环 不统计内网网卡
		} 	
		//strstr
	}
	fclose(fp);
 
 
}
 
//解析信息并放置到结构体当中
void getinfo(char* line){
	strcpy(eth0.devname,get(line));
	int i = 0;
	for(i=0;i<16;i++){
		eth0.info[i] = strtol(getstr(),(char**)NULL,10);
	//	printf("%d ",eth0.info[i]);
	}
	
}
 
char* get(char* str){
	pTmp = str;
	return getstr();
}
 
char* getstr(){
	char* pStr = pTmp;
	//int len = strlen(str); 
	while(*pTmp == ' '){
		pTmp++;
	} 
	while(*pTmp != '\0'){
		if(*pTmp == ' '){
			*pTmp = '\0';
			break;
		}
		pTmp++;
	
	}
	pTmp++;
	return pStr;
}