#include<stdio.h>
#include<wINDOWS.h>
#include <sys/stat.h>
#include<stdbool.h>
#include"ErrShow.h"
#define _crash_ 1
unsigned long GetFileSizeK(const char *path) ;
short EncodeFile(const char * File_0,const char *File_E,const bool mess);
short DecodeFile(const  char * File_0,const char *File_E,const bool mess);
char GetRanDomKey(/*const unsigned short ass*/);
char GetMessKey() ;
char *Key;
char *Key_Mess;
/*
v1.4 TODO

1.提高验证的准确性
2.-del 提供加密/解密后删除文件
3. 文本补充
4.修复bug
v1.5 TODO

1.-mess 提供双重加密，提高安全性
2.显现github地址

*/
/*

备注
-mess 选项的防止加密套娃 我做不出来
等待有缘人帮我做 
XD
BY ZNKZZ
23\02\20 16:14
*/
int  main(int argc,char*argv[]) {
	notice("文件加密器 版本:v1.5 date:2020\\2\\23 by znkzz[软件不再更新]");
	notice("Github:https://github.com/znkzz/File-Encryption");
	if(argc<5) {
		error("命令行参数错误!");
		exit(-1);
	}
	Key=argv[2];
	short code;
	bool mess=false;

	if(argc>5) {

		if((strcmp(argv[5],"-mess")==0||strcmp(argv[6],"-mess")==0)&&argc==7) {
			mess=true;
			Key_Mess=argv[6];
            	successful("已设置密码!");
                 
              //   successful(argv[6]);
		} else if((strcmp(argv[5],"-mess")==0||strcmp(argv[6],"-mess")==0)&&argc==6) {
			mess=true;
			error("未设置密码!");
			exit(-2);
		}
	}

	if(strcmp(argv[1],"-en")==0) {
		code=EncodeFile(argv[3],argv[4],mess);
		if(code==1) {
			notice("文件为空或已有指定的加密后文件!");
		} else if (code==-1) {
			error("无法打开文件!");
		} else if (code==-2) {
			error("无法分配内存!");
		} else if (code==-3) {
			error("禁止加密套娃!");
		} else if (code==2) {
			error("文件过大!(>512MB)");
		} else if (code==0) {
			successful("加密完成.");
		}
	} else if(strcmp(argv[1],"-de")==0) {
		code=DecodeFile(argv[3],argv[4],mess) ;
		if(code==1) {
			notice("文件为空或已有指定的解密后的文件!");
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
		} else if (code==-5) {
			error("不支持解密v1.3的文件,请使用本软件的1.3版本进行解密.");
		}

	} else {
		error("命令行参数非法!");
		return -1;
	}
	if (argc==6&&(strcmp(argv[5],"-del")==0||strcmp(argv[6],"-del")==0||strcmp(argv[7],"-del")==0)&&code==0) {
		remove(argv[3]);
	} else if(argc==6&&(strcmp(argv[5],"-del")==0||strcmp(argv[6],"-del")==0||strcmp(argv[7],"-del")==0)&&code!=0) {
		error("解密或加密过程中出错,因此不能删除原文件");
	}
	return 0;

//	DecodeFile(argv[3],argv[4]);
//	getchar();

}
short EncodeFile(const  char * File_0,const char *File_E/*,char (*p) (void)*/,const bool mess) {
	FILE *f1=fopen(File_0,"r+b"); //读方式打开一个二进制文件，只允许读数据
	FILE *f2=fopen(File_E,"w+b") ;//写方式打开一个二进制文件，只允许写数据


	notice("文件已打开");
	unsigned long StrLong=GetFileSizeK(File_0);

	if(StrLong==0) {
		fwrite("", sizeof(char), StrLong, f2);
		fclose(f1);
		fclose(f2);
		return 1;
	}

	if(StrLong>512*1024*1024) {
		remove(File_E);
		fclose(f1);
		fclose(f2);
		return 2;
	}

	if(!StrLong||!f1||!f2) {
		remove(File_E);
		return -1;
	}
	char *pDATA = (char *)malloc(StrLong*sizeof(char));

	if(pDATA==NULL) {
		remove(File_E);
		return -2;
	}
	successful("文件内容已成功读取.");

	fread(pDATA, sizeof(char), StrLong, f1);

unsigned long i ;

	for (i = 0; i < StrLong; i++) {

		pDATA[i]^=(GetRanDomKey());


	}

	if(mess==true) {
		for ( i = 0; i < StrLong; i++) {

			pDATA[i]^=(GetMessKey());

		}
	}

	successful("文件已加密完毕.正在写文件中······");
  fwrite(pDATA, sizeof(char), StrLong, f2);
	if(mess==true) {
		fputc(10^(int)GetMessKey(),f2);
		fputc(11^(int)GetMessKey(),f2);
		fputc(12^(int)GetMessKey(),f2);
		fputc(13^(int)GetMessKey(),f2);
	} else {
		fputc(10^(int)GetRanDomKey(),f2);
		fputc(11^(int)GetRanDomKey(),f2);
		fputc(12^(int)GetRanDomKey(),f2);
		fputc(13^(int)GetRanDomKey(),f2);
	}
	if(((int)(pDATA[i-4])==10&&(int)(pDATA[i-3])==11&&(int)(pDATA[i-2])==12&&(int)(pDATA[i-1])==13)||(int)(pDATA[i-1])==10) {

		free(pDATA);
		fclose(f1);
		fclose(f2);
		remove(File_E);
		return -3;
	}
	


	free(pDATA);
	fclose(f1);
	fclose(f2);
	return 0;
}

short DecodeFile(const  char * File_0,const char *File_E/*,char (*p) (void)*/,const bool mess) {
	FILE *f1=fopen(File_0,"r+b"); //读方式打开一个二进制文件，只允许读数据
	FILE *f2=fopen(File_E,"w+b") ;//写方式打开一个二进制文件，只允许写数据

	notice("文件已打开");
	unsigned long StrLong=GetFileSizeK(File_0);
	

	if(!StrLong||!f1||!f2) {
		remove(File_E);
		return -1;
	}

	if(StrLong==0) {

		fclose(f1);//bug fix 2.19
		fclose(f2);
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
	if(mess==true) {

		for ( i = 0; i < StrLong; i++) {

			pDATA[i]^=(GetMessKey());

		}

		for (unsigned long x = 0; x < StrLong-4; x++) {

			pDATA[x]^=(GetRanDomKey());
// printf("%d\t",pDATA[i]);

		}
	} else {

		for (i = 0; i < StrLong; i++) {

			pDATA[i]^=(GetRanDomKey());


		}
	}

	successful("文件已解密完毕.正在效验文件中······");

	short code;
	if((int)(pDATA[i-4])==10&&(int)(pDATA[i-3])==11&&(int)(pDATA[i-2])==12&&(int)(pDATA[i-1])==13) {

		fwrite(pDATA, sizeof(char), StrLong-4, f2);
		code=0;

	} else {
		if((int)(pDATA[i-1])==10) {
			remove(File_E);
			code=-5;

		} else {
			error("文件效验失败!错误信息:") ;
			printf("[%c * %d]\n",pDATA[i],(int)(pDATA[i]));
			printf("[%c * %d]\n",pDATA[i+1],(int)(pDATA[i+1]));
			printf("[%c * %d]\n",pDATA[i-1],(int)(pDATA[i-1]));
			remove(File_E);
			code=-4;
		}
	}

	free(pDATA);
	fclose(f1);
	fclose(f2);
	return code;
}



unsigned long GetFileSizeK(const char *path) {
//获取文件名为path的文件大小。
	struct stat statbuf;
	stat(path,&statbuf);
	unsigned long size=statbuf.st_size;
	return size;

}
char GetRanDomKey() {
	static unsigned short con; //密钥长度不会很长
	++con;
	if(con==(strlen(Key)+1)) {
		con=0;
	}

	return	Key[con];
}
char GetMessKey() {
	static unsigned short con; //密钥长度不会很长


	++con;

	if(con==(strlen(Key_Mess)+1)) {
		con=0;
	}

	return	Key_Mess[con];
}

