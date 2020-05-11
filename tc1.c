#include<stdio.h>
#include<wINDOWS.h>
#include <sys/stat.h>
#include<stdbool.h>
#include"ErrShow.h"
#define _crash_ 1
unsigned long GetrFileSizeK(const char *path) ;
short EncodeFile(const char * File_0,const char *File_E);
short DecodeFile(const  char * File_0,const char *File_E);
char GetRanDomKey(/*const unsigned short ass*/);
char *Key;
/*
v1.4 TODO

1.提高验证的准确性 
2.-del 提供加密/解密后删除文件
3. 文本补充 

v1.5 TODO

1.-mess 提供双重加密，提高安全性 
2.显现github地址 

*/
int  main(int argc,char*argv[]) {
		notice("文件加密器 版本:v1.3.1 date:2020\\5\\11 by znkzz");
	if(argc!=5) {
		error("命令行参数错误!");
		exit(-1);
	}
	Key=argv[2];

	if(strcmp(argv[1],"-en")==0) {
		short code=	EncodeFile(argv[3],argv[4])	;
		if(code==1) {
			notice("文件为空!");
		} else if (code==-1) {
			error("无法打开文件!");
		} else if (code==-2) {
			error("无法分配内存!");
		} else if (code==2) {
			error("文件过大!(>512MB)");
		} else if (code==0) {
			successful("加密完成.");
		}
	} else if(strcmp(argv[1],"-de")==0) {
		short code=	DecodeFile(argv[3],argv[4]) ;
		if(code==1) {
			notice("文件为空!");
		} else if (code==-1) {
			error("无法打开文件!");
		} else if (code==-2) {
			error("无法分配内存!");
		} else if (code==-3) {
			error("加密文件过大![不是源文件?]");
		} else if (code==0) {
			successful("解密完成.");
		} else if (code==-4) {
			error("文件的密钥不正确.");
		}
		return 0;
	} else {
		error("命令行参数非法!");
		return -1;
	}


//	DecodeFile(argv[3],argv[4]);
//	getchar();

}
short EncodeFile(const  char * File_0,const char *File_E/*,char (*p) (void)*/) {
	FILE *f1=fopen(File_0,"r+b"); //读方式打开一个二进制文件，只允许读数据
	FILE *f2=fopen(File_E,"w+b") ;//写方式打开一个二进制文件，只允许写数据

	notice("文件已打开");
	unsigned long StrLong=GetrFileSizeK(File_0);

	if(StrLong==0) {
		fwrite("", sizeof(char), StrLong, f2);
		fclose(f1);
		fclose(f2);
		return 1;
	}
	if(StrLong>512*1024*1024) {
		fclose(f1);
		fclose(f2);
		return 2;
	}

	if(!StrLong||!f1||!f2) {
		return -1;
	}
	char *pDATA = (char *)malloc(StrLong*sizeof(char));

	if(pDATA==NULL) {
		return -2;
	}
	successful("文件内容已成功读取.");
	fread(pDATA, sizeof(char), StrLong, f1);



	for (unsigned long i = 0; i < StrLong; i++) {

		pDATA[i]^=(GetRanDomKey());

	}
	successful("文件已加密完毕.正在写文件中······");
	fwrite(pDATA, sizeof(char), StrLong, f2);
	fputc(10^(int)GetRanDomKey(),f2);
	free(pDATA);
	fclose(f1);
	fclose(f2);
	return 0;
}



short DecodeFile(const  char * File_0,const char *File_E/*,char (*p) (void)*/) {
	FILE *f1=fopen(File_0,"r+b"); //读方式打开一个二进制文件，只允许读数据
	FILE *f2=fopen(File_E,"w+b") ;//写方式打开一个二进制文件，只允许写数据

	notice("文件已打开");
	unsigned long StrLong=GetrFileSizeK(File_0);

	if(!StrLong||!f1||!f2) {
		remove(File_E);
		return -1;
	}

	if(StrLong==0) {

		return 1;
	}
	if(StrLong>512*1024*1024) {
		fclose(f1);
		fclose(f2);
		remove(File_E);
		return -3;
	}

	char *pDATA = (char *)malloc(StrLong*sizeof(char));

	if(pDATA==NULL) {
		fclose(f1);
		fclose(f2);
		remove(File_E);
		return -2;
	}
	fread(pDATA, sizeof(char), StrLong, f1);
	successful("文件内容已成功读取.");

	unsigned long i;
	char t;
	for (i = 0; i < StrLong; i++) {
t=GetRanDomKey();
		pDATA[i]^=t;
printf("%c\n",t);
Sleep(300);
	}
	successful("文件已解密完毕.正在效验文件中······");

	short code;
	if((int)(pDATA[i-1])==10) {
		fwrite(pDATA, sizeof(char), StrLong-1, f2);
		code=0;
		
	} else {
		error("文件效验失败!错误信息:") ;
		printf("[%c * %d]\n",pDATA[i],(int)(pDATA[i]));
		printf("[%c * %d]\n",pDATA[i+1],(int)(pDATA[i+1]));
		printf("[%c * %d]\n",pDATA[i-1],(int)(pDATA[i-1]));
		remove(File_E);
		code=-4;
	}

	free(pDATA);
	fclose(f1);
	fclose(f2);
	return code;
}



unsigned long GetrFileSizeK(const char *path) {
//获取文件名为path的文件大小。
	struct stat statbuf;
	stat(path,&statbuf);
	unsigned long size=statbuf.st_size;
	return size;

}
char GetRanDomKey() {
	static unsigned short con; //密钥长度不会很长
	++con;
	if(con==(strlen(Key))) {
		con=0;
	}

	return	Key[con];
}

