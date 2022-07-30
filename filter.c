#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define STRING_MAX 1024



void replace(char *src, char *str, char *rep) {
  char *p = strstr(src, str);
  if(p) {
    int len = strlen(src)+strlen(rep)-strlen(str);
    char r[len];
    memset(r, 0, len);
    if( p >= src ) {
     strncpy(r, src, p-src);
     r[p-src]='\0';
     strncat(r, rep, strlen(rep));
     strncat(r, p+strlen(str), p+strlen(str)-src+strlen(src));
     strcpy(src, r);
     replace(p+strlen(rep), str, rep);
    }
  }
}



void append(char ***lines,size_t *nlines,char *line) {
	(*lines)=realloc(*lines,sizeof(**lines)*((*nlines)+1));
	(*lines)[*nlines]=strdup(line);
	(*nlines)++;
}



char *hide(const char *s,char c) {
	char *r=strdup(s);
	for(int i=0;i<strlen(r);i++) {
		if(isalnum(r[i])) r[i]=c;
	}
	return r;
}



int main(int argc,char **argv) {

	FILE *fin,*fout;

	char **lines=NULL;
	size_t nlines=0;
	
	char line[STRING_MAX];
	char newLine[STRING_MAX];

	if(argc<4) {
		printf("Syntax: %s listfile srcfile dstfile\n",argv[0]);
		return 1;
	}

	if((fin=fopen(argv[1],"rt"))==NULL) {
		perror(argv[1]);
		return 2;
	}
	
	while(fgets(line,STRING_MAX,fin)!=NULL) {
		char *p=strchr(line,'\n'); if(p) *p='\0';
		append(&lines,&nlines,line);
	}	

	fclose(fin);

//*
	if((fin=fopen(argv[2],"rt"))==NULL) {
		perror(argv[2]);
		return 3;
	}

	if((fout=fopen(argv[3],"wt"))==NULL) {
		perror(argv[3]);
		return 4;
	}

	while(fgets(line,STRING_MAX,fin)!=NULL) {
		char *p=strchr(line,'\n'); if(p) *p='\0';
		for(int i=0;i<nlines;i++) {
			replace(line,lines[i],hide(lines[i],'*'));
		}
		fprintf(fout,"%s\n",line);
	}

	fclose(fin);
	fclose(fout);

//*/

	return 0;
	
}
