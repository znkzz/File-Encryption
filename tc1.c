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

1.�����֤��׼ȷ�� 
2.-del �ṩ����/���ܺ�ɾ���ļ�
3. �ı����� 

v1.5 TODO

1.-mess �ṩ˫�ؼ��ܣ���߰�ȫ�� 
2.����github��ַ 

*/
int  main(int argc,char*argv[]) {
		notice("�ļ������� �汾:v1.3.1 date:2020\\5\\11 by znkzz");
	if(argc!=5) {
		error("�����в�������!");
		exit(-1);
	}
	Key=argv[2];

	if(strcmp(argv[1],"-en")==0) {
		short code=	EncodeFile(argv[3],argv[4])	;
		if(code==1) {
			notice("�ļ�Ϊ��!");
		} else if (code==-1) {
			error("�޷����ļ�!");
		} else if (code==-2) {
			error("�޷������ڴ�!");
		} else if (code==2) {
			error("�ļ�����!(>512MB)");
		} else if (code==0) {
			successful("�������.");
		}
	} else if(strcmp(argv[1],"-de")==0) {
		short code=	DecodeFile(argv[3],argv[4]) ;
		if(code==1) {
			notice("�ļ�Ϊ��!");
		} else if (code==-1) {
			error("�޷����ļ�!");
		} else if (code==-2) {
			error("�޷������ڴ�!");
		} else if (code==-3) {
			error("�����ļ�����![����Դ�ļ�?]");
		} else if (code==0) {
			successful("�������.");
		} else if (code==-4) {
			error("�ļ�����Կ����ȷ.");
		}
		return 0;
	} else {
		error("�����в����Ƿ�!");
		return -1;
	}


//	DecodeFile(argv[3],argv[4]);
//	getchar();

}
short EncodeFile(const  char * File_0,const char *File_E/*,char (*p) (void)*/) {
	FILE *f1=fopen(File_0,"r+b"); //����ʽ��һ���������ļ���ֻ���������
	FILE *f2=fopen(File_E,"w+b") ;//д��ʽ��һ���������ļ���ֻ����д����

	notice("�ļ��Ѵ�");
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
	successful("�ļ������ѳɹ���ȡ.");
	fread(pDATA, sizeof(char), StrLong, f1);



	for (unsigned long i = 0; i < StrLong; i++) {

		pDATA[i]^=(GetRanDomKey());

	}
	successful("�ļ��Ѽ������.����д�ļ��С�����������");
	fwrite(pDATA, sizeof(char), StrLong, f2);
	fputc(10^(int)GetRanDomKey(),f2);
	free(pDATA);
	fclose(f1);
	fclose(f2);
	return 0;
}



short DecodeFile(const  char * File_0,const char *File_E/*,char (*p) (void)*/) {
	FILE *f1=fopen(File_0,"r+b"); //����ʽ��һ���������ļ���ֻ���������
	FILE *f2=fopen(File_E,"w+b") ;//д��ʽ��һ���������ļ���ֻ����д����

	notice("�ļ��Ѵ�");
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
	successful("�ļ������ѳɹ���ȡ.");

	unsigned long i;
	char t;
	for (i = 0; i < StrLong; i++) {
t=GetRanDomKey();
		pDATA[i]^=t;
printf("%c\n",t);
Sleep(300);
	}
	successful("�ļ��ѽ������.����Ч���ļ��С�����������");

	short code;
	if((int)(pDATA[i-1])==10) {
		fwrite(pDATA, sizeof(char), StrLong-1, f2);
		code=0;
		
	} else {
		error("�ļ�Ч��ʧ��!������Ϣ:") ;
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
//��ȡ�ļ���Ϊpath���ļ���С��
	struct stat statbuf;
	stat(path,&statbuf);
	unsigned long size=statbuf.st_size;
	return size;

}
char GetRanDomKey() {
	static unsigned short con; //��Կ���Ȳ���ܳ�
	++con;
	if(con==(strlen(Key))) {
		con=0;
	}

	return	Key[con];
}

