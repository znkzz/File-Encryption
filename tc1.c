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

1.�����֤��׼ȷ��
2.-del �ṩ����/���ܺ�ɾ���ļ�
3. �ı�����
4.�޸�bug
v1.5 TODO

1.-mess �ṩ˫�ؼ��ܣ���߰�ȫ��
2.����github��ַ

*/
/*

��ע
-mess ѡ��ķ�ֹ�������� ����������
�ȴ���Ե�˰����� 
XD
BY ZNKZZ
23\02\20 16:14
*/
int  main(int argc,char*argv[]) {
	notice("�ļ������� �汾:v1.5 date:2020\\2\\23 by znkzz[������ٸ���]");
	notice("Github:https://github.com/znkzz/File-Encryption");
	if(argc<5) {
		error("�����в�������!");
		exit(-1);
	}
	Key=argv[2];
	short code;
	bool mess=false;

	if(argc>5) {

		if((strcmp(argv[5],"-mess")==0||strcmp(argv[6],"-mess")==0)&&argc==7) {
			mess=true;
			Key_Mess=argv[6];
            	successful("����������!");
                 
              //   successful(argv[6]);
		} else if((strcmp(argv[5],"-mess")==0||strcmp(argv[6],"-mess")==0)&&argc==6) {
			mess=true;
			error("δ��������!");
			exit(-2);
		}
	}

	if(strcmp(argv[1],"-en")==0) {
		code=EncodeFile(argv[3],argv[4],mess);
		if(code==1) {
			notice("�ļ�Ϊ�ջ�����ָ���ļ��ܺ��ļ�!");
		} else if (code==-1) {
			error("�޷����ļ�!");
		} else if (code==-2) {
			error("�޷������ڴ�!");
		} else if (code==-3) {
			error("��ֹ��������!");
		} else if (code==2) {
			error("�ļ�����!(>512MB)");
		} else if (code==0) {
			successful("�������.");
		}
	} else if(strcmp(argv[1],"-de")==0) {
		code=DecodeFile(argv[3],argv[4],mess) ;
		if(code==1) {
			notice("�ļ�Ϊ�ջ�����ָ���Ľ��ܺ���ļ�!");
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
		} else if (code==-5) {
			error("��֧�ֽ���v1.3���ļ�,��ʹ�ñ������1.3�汾���н���.");
		}

	} else {
		error("�����в����Ƿ�!");
		return -1;
	}
	if (argc==6&&(strcmp(argv[5],"-del")==0||strcmp(argv[6],"-del")==0||strcmp(argv[7],"-del")==0)&&code==0) {
		remove(argv[3]);
	} else if(argc==6&&(strcmp(argv[5],"-del")==0||strcmp(argv[6],"-del")==0||strcmp(argv[7],"-del")==0)&&code!=0) {
		error("���ܻ���ܹ����г���,��˲���ɾ��ԭ�ļ�");
	}
	return 0;

//	DecodeFile(argv[3],argv[4]);
//	getchar();

}
short EncodeFile(const  char * File_0,const char *File_E/*,char (*p) (void)*/,const bool mess) {
	FILE *f1=fopen(File_0,"r+b"); //����ʽ��һ���������ļ���ֻ���������
	FILE *f2=fopen(File_E,"w+b") ;//д��ʽ��һ���������ļ���ֻ����д����


	notice("�ļ��Ѵ�");
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
	successful("�ļ������ѳɹ���ȡ.");

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

	successful("�ļ��Ѽ������.����д�ļ��С�����������");
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
	FILE *f1=fopen(File_0,"r+b"); //����ʽ��һ���������ļ���ֻ���������
	FILE *f2=fopen(File_E,"w+b") ;//д��ʽ��һ���������ļ���ֻ����д����

	notice("�ļ��Ѵ�");
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
	successful("�ļ������ѳɹ���ȡ.");



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

	successful("�ļ��ѽ������.����Ч���ļ��С�����������");

	short code;
	if((int)(pDATA[i-4])==10&&(int)(pDATA[i-3])==11&&(int)(pDATA[i-2])==12&&(int)(pDATA[i-1])==13) {

		fwrite(pDATA, sizeof(char), StrLong-4, f2);
		code=0;

	} else {
		if((int)(pDATA[i-1])==10) {
			remove(File_E);
			code=-5;

		} else {
			error("�ļ�Ч��ʧ��!������Ϣ:") ;
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
//��ȡ�ļ���Ϊpath���ļ���С��
	struct stat statbuf;
	stat(path,&statbuf);
	unsigned long size=statbuf.st_size;
	return size;

}
char GetRanDomKey() {
	static unsigned short con; //��Կ���Ȳ���ܳ�
	++con;
	if(con==(strlen(Key)+1)) {
		con=0;
	}

	return	Key[con];
}
char GetMessKey() {
	static unsigned short con; //��Կ���Ȳ���ܳ�


	++con;

	if(con==(strlen(Key_Mess)+1)) {
		con=0;
	}

	return	Key_Mess[con];
}

