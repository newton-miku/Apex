/*ini.c*/
#include <stdio.h>  
#include <string.h>        
#include <cstdint>
#include <stdlib.h>
#include <string>

int errorno;
char* bool2c(bool a)
{
    char* b;
    if (a) b = (char*)"true";
    else b = (char*)"false";
    return b;
}
bool c2bool(char* a)
{
    char c[10]="true",d[10]="false";
    //printf("%s\t%d\t%d\t", a,strcmp(a, c), strcmp(a, d));
    if (a == NULL)return false;
    if (strcmp(a, c)==0)return true;
    if (strcmp(a, d)==0)return false;
}
/*
    * ��������         GetIniKeyString
    * ��ڲ�����        title
    *                      �����ļ���һ�����ݵı�ʶ
    *                  key
    *                      ����������Ҫ������ֵ�ı�ʶ
    *                  filename
    *                      Ҫ��ȡ���ļ�·��
    * ����ֵ��         �ҵ���Ҫ���ֵ�򷵻���ȷ��� 0
    *                  ���򷵻�-1
    */
int GetIniKeyString(const char* title, const char* key, const char* filename, char* buf)
{
    FILE* fp;
    int  flag = 0;
    char sTitle[64], * wTmp;
    char sLine[1024];
    sprintf(sTitle, "[%s]", title);

    if (NULL == (fp = fopen(filename, "r"))) {
        return -2;
    }
    while (NULL != fgets(sLine, 1024, fp)) {
        // ����ע����  
        if (0 == strncmp("//", sLine, 2)) continue;
        if ('#' == sLine[0])              continue;
        wTmp = strchr(sLine, '=');
        if ((NULL != wTmp) && (1 == flag)) {
            if (0 == strncmp(key, sLine, strlen(key))) { // �������ļ���ȡ��Ϊ׼  
                sLine[strlen(sLine) - 1] = '\0';
                fclose(fp);
                while (*(wTmp + 1) == ' ') {
                    wTmp++;
                }
                strcpy(buf, wTmp + 1);
                //printf("%s\n", buf);
                return 0;
            }
        }
        else {
            if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // �������ļ���ȡ��Ϊ׼  
                flag = 1; // �ҵ�����λ��  
            }
        }
    }
    fclose(fp);
    return -1;
}
int GetIniKeyString(const char* title, const char* key, const char* filename, int8_t* num)
{
    char buf[10];
    FILE* fp;
    int  flag = 0;
    char sTitle[64], * wTmp;
    char sLine[1024];
    sprintf(sTitle, "[%s]", title);

    if (NULL == (fp = fopen(filename, "r"))) {
        return -2;
    }
    while (NULL != fgets(sLine, 1024, fp)) {
        // ����ע����  
        if (0 == strncmp("//", sLine, 2)) continue;
        if ('#' == sLine[0])              continue;
        wTmp = strchr(sLine, '=');
        if ((NULL != wTmp) && (1 == flag)) {
            if (0 == strncmp(key, sLine, strlen(key))) { // �������ļ���ȡ��Ϊ׼  
                sLine[strlen(sLine) - 1] = '\0';
                fclose(fp);
                while (*(wTmp + 1) == ' ') {
                    wTmp++;
                }
                strcpy(buf, wTmp + 1);
                strcpy((char*)num, buf);
                return 0;
            }
        }
        else {
            if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // �������ļ���ȡ��Ϊ׼  
                flag = 1; // �ҵ�����λ��  
            }
        }
    }
    fclose(fp);
    return -1;
}
int GetIniKeyString(const char* title, const char* key, const char* filename, float* fnum)
{
    char buf[20];
    FILE* fp;
    int  flag = 0;
    char sTitle[64], * wTmp;
    char sLine[1024];
    sprintf(sTitle, "[%s]", title);

    if (NULL == (fp = fopen(filename, "r"))) {
        return -2;
    }
    while (NULL != fgets(sLine, 1024, fp)) {
        // ����ע����  
        if (0 == strncmp("//", sLine, 2)) continue;
        if ('#' == sLine[0])              continue;
        wTmp = strchr(sLine, '=');
        if ((NULL != wTmp) && (1 == flag)) {
            if (0 == strncmp(key, sLine, strlen(key))) { // �������ļ���ȡ��Ϊ׼  
                sLine[strlen(sLine) - 1] = '\0';
                fclose(fp);
                while (*(wTmp + 1) == ' ') {
                    wTmp++;
                }
                strcpy(buf, wTmp + 1);
                *fnum =atof(buf);
                
                //printf("%f\t%f\n", *fnum, atof(buf));
                return 0;
            }
        }
        else {
            if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // �������ļ���ȡ��Ϊ׼  
                flag = 1; // �ҵ�����λ��  
            }
        }
    }
    fclose(fp);
    return -1;
}
int GetIniKeyString(const char* title, const char* key, const char* filename, float* fnum, int times)
{
    char buf[10];
    FILE* fp;
    int  flag = 0;
    char sTitle[64], * wTmp;
    char sLine[1024];
    sprintf(sTitle, "[%s]", title);

    if (NULL == (fp = fopen(filename, "r"))) {
        return -2;
    }
    while (NULL != fgets(sLine, 1024, fp)) {
        // ����ע����  
        if (0 == strncmp("//", sLine, 2)) continue;
        if ('#' == sLine[0])              continue;
        wTmp = strchr(sLine, '=');
        if ((NULL != wTmp) && (1 == flag)) {
            if (0 == strncmp(key, sLine, strlen(key))) { // �������ļ���ȡ��Ϊ׼  
                sLine[strlen(sLine) - 1] = '\0';
                fclose(fp);
                while (*(wTmp + 1) == ' ') {
                    wTmp++;
                }
                strcpy(buf, wTmp + 1);
                float a = atof(buf);
                *fnum = a / times;
                return 0;
            }
        }
        else {
            if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // �������ļ���ȡ��Ϊ׼  
                flag = 1; // �ҵ�����λ��  
            }
        }
    }
    fclose(fp);
    return -1;
}
int GetIniKeyString(const char* title, const char* key, const char* filename, int* num)
{
    char buf[10];
    FILE* fp;
    int  flag = 0;
    char sTitle[64], * wTmp;
    char sLine[1024];
    sprintf(sTitle, "[%s]", title);

    if (NULL == (fp = fopen(filename, "r"))) {
        return -2;
    }
    while (NULL != fgets(sLine, 1024, fp)) {
        // ����ע����  
        if (0 == strncmp("//", sLine, 2)) continue;
        if ('#' == sLine[0])              continue;
        wTmp = strchr(sLine, '=');
        if ((NULL != wTmp) && (1 == flag)) {
            if (0 == strncmp(key, sLine, strlen(key))) { // �������ļ���ȡ��Ϊ׼  
                sLine[strlen(sLine) - 1] = '\0';
                fclose(fp);
                while (*(wTmp + 1) == ' ') {
                    wTmp++;
                }
                strcpy(buf, wTmp + 1);
                *num = atoi(buf);
                return 0;
            }
        }
        else {
            if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // �������ļ���ȡ��Ϊ׼  
                flag = 1; // �ҵ�����λ��  
            }
        }
    }
    fclose(fp);
    return -1;
}

/*
    * ��������         PutIniKeyString
    * ��ڲ�����        title
    *                      �����ļ���һ�����ݵı�ʶ
    *                  key
    *                      ����������Ҫ������ֵ�ı�ʶ
    *                  val
    *                      ���ĺ��ֵ
    *                  filename
    *                      Ҫ��ȡ���ļ�·��
    * ����ֵ��         �ɹ�����  0
    *                  ���򷵻� -1
    */
int PutIniKeyString(const char* title, const char* key, char* val, const char* filename)
{
    FILE* fpr, * fpw;
    int  flag = 0;
    int i = 0;
    char sLine[1024], sTitle[32], * wTmp; 
    sprintf(sTitle, "[%s]", title);
    if (NULL == (fpr = fopen(filename, "r")))
        if (NULL != (fpr = fopen(filename, "w")))
        {
            //sprintf(sLine, "%s\n%s = %s\n\n", sTitle, key, val);
            fprintf(fpr,"%s\n%s = %s\n\n", sTitle, key, val);
            fputs(sLine, fpr);
            fclose(fpr);
            return 0;
        }else return -1;// ��ȡԭ�ļ�  
    sprintf(sLine, "%s.tmp", filename);
    if (NULL == (fpw = fopen(sLine, "w")))
        return -1;// д����ʱ�ļ�        
    while (NULL != fgets(sLine, 1024, fpr)) {
        if (2 != flag) { // ����ҵ�Ҫ�޸ĵ���һ�У��򲻻�ִ���ڲ��Ĳ���  
            wTmp = strchr(sLine, '=');
            if (flag == 1) {
                if (NULL != wTmp)//�����Ƿ���=
                {
                    if (0 == strncmp(key, sLine, strlen(key))) { // �������ļ���ȡ��Ϊ׼ 
                        flag = 2;// ����ֵ������д���ļ�  
                        sprintf(wTmp + 1, " %s\n", val);
                    }
                }
                else
                {
                    flag = 2;// ������ӣ����ټ������
                    sprintf(sLine, "%s = %s\n\n", key, val);//������д���ļ�
                }
            }
            else {
                if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // �������ļ���ȡ��Ϊ׼
                    flag = 1; // �ҵ�����λ��  
                }
            }
        }
        fputs(sLine, fpw); // д����ʱ�ļ� 
    }
    if (flag == 0)
    {
        sprintf(sLine, "%s\n%s = %s\n\n", sTitle, key, val);
        fputs(sLine, fpw);
    }
    fclose(fpr);
    fclose(fpw);
    sprintf(sLine, "%s.tmp", filename);
    remove(filename);
    return rename(sLine, filename);// ����ʱ�ļ����µ�ԭ�ļ�  
}
int PutIniKeyString(const char* title, const char* key, int8_t a, const char* filename)
{
    char val[10];
    strcpy(val, std::to_string(a).c_str());
    FILE* fpr, * fpw;
    int  flag = 0;
    int i = 0;
    char sLine[1024], sTitle[32], * wTmp; 
    sprintf(sTitle, "[%s]", title);
    if (NULL == (fpr = fopen(filename, "r")))
        if (NULL != (fpr = fopen(filename, "w")))
        {
            //sprintf(sLine, "%s\n%s = %s\n\n", sTitle, key, val);
            fprintf(fpr,"%s\n%s = %s\n\n", sTitle, key, val);
            fputs(sLine, fpr);
            fclose(fpr);
            return 0;
        }else return -1;// ��ȡԭ�ļ�  
    sprintf(sLine, "%s.tmp", filename);
    if (NULL == (fpw = fopen(sLine, "w")))
        return -1;// д����ʱ�ļ�        
    while (NULL != fgets(sLine, 1024, fpr)) {
        if (2 != flag) { // ����ҵ�Ҫ�޸ĵ���һ�У��򲻻�ִ���ڲ��Ĳ���  
            wTmp = strchr(sLine, '=');
            if (flag == 1) {
                if (NULL != wTmp)//�����Ƿ���=
                {
                    if (0 == strncmp(key, sLine, strlen(key))) { // �������ļ���ȡ��Ϊ׼ 
                        flag = 2;// ����ֵ������д���ļ�  
                        sprintf(wTmp + 1, " %s\n", val);
                    }
                }
                else
                {
                    flag = 2;// ������ӣ����ټ������
                    sprintf(sLine, "%s = %s\n\n", key, val);//������д���ļ�
                }
            }
            else {
                if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // �������ļ���ȡ��Ϊ׼
                    flag = 1; // �ҵ�����λ��  
                }
            }
        }
        fputs(sLine, fpw); // д����ʱ�ļ� 
    }
    if (flag == 0)
    {
        sprintf(sLine, "%s\n%s = %s\n\n", sTitle, key, val);
        fputs(sLine, fpw);
    }
    fclose(fpr);
    fclose(fpw);
    sprintf(sLine, "%s.tmp", filename);
    remove(filename);
    return rename(sLine, filename);// ����ʱ�ļ����µ�ԭ�ļ�  
}
int PutIniKeyString(const char* title, const char* key, const char* val, const char* filename)
{
    FILE* fpr, * fpw;
    int  flag = 0;
    char sLine[1024], sTitle[32], * wTmp;
    sprintf(sTitle, "[%s]", title);
    if (NULL == (fpr = fopen(filename, "r")))
        if (NULL != (fpr = fopen(filename, "w")))
        {
            
            //sprintf(sLine, "%s\n%s = %s\n\n", sTitle, key, val);
            fprintf(fpr, "%s\n%s = %s\n\n", sTitle, key, val);
            fputs(sLine, fpr);
            fclose(fpr);
            return 0;
        }
        else return -1;// ��ȡԭ�ļ� 
    sprintf(sLine, "%s.tmp", filename);
    if (NULL == (fpw = fopen(sLine, "w")))
        return -1;// д����ʱ�ļ�        
    while (NULL != fgets(sLine, 1024, fpr)) {
        if (2 != flag) { // ����ҵ�Ҫ�޸ĵ���һ�У��򲻻�ִ���ڲ��Ĳ���  
            wTmp = strchr(sLine, '=');
            if (flag == 1) {
                if (NULL != wTmp)//�����Ƿ���=
                {
                    if (0 == strncmp(key, sLine, strlen(key))) { // �������ļ���ȡ��Ϊ׼ 
                        flag = 2;// ����ֵ������д���ļ�  
                        sprintf(wTmp + 1, " %s\n", val);
                    }
                }
                else
                {
                    flag = 2;// ������ӣ����ټ������
                    sprintf(sLine, "%s = %s\n\n", key, val);//������д���ļ�
                }
            }
            else {
                if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // �������ļ���ȡ��Ϊ׼
                    flag = 1; // �ҵ�����λ��  
                }
            }
        }
        fputs(sLine, fpw); // д����ʱ�ļ� 
    }
    if (flag == 0)
    {
        sprintf(sLine, "%s\n%s = %s\n\n", sTitle, key, val);
        fputs(sLine, fpw);
    }
    fclose(fpr);
    fclose(fpw);
    sprintf(sLine, "%s.tmp", filename);
    remove(filename);
    return rename(sLine, filename);// ����ʱ�ļ����µ�ԭ�ļ�  
}
int PutIniKeyString(const char* title, const char* key, bool a, const char* filename)
{
    char* val = bool2c(a);
    FILE* fpr, * fpw;
    int  flag = 0;
    char sLine[1024], sTitle[32], * wTmp;
    sprintf(sTitle, "[%s]", title);
    if (NULL == (fpr = fopen(filename, "r")))
        if (NULL != (fpr = fopen(filename, "w")))
        {
            
            //sprintf(sLine, "%s\n%s = %s\n\n", sTitle, key, val);
            fprintf(fpr, "%s\n%s = %s\n\n", sTitle, key, val);
            fputs(sLine, fpr);
            fclose(fpr);
            return 0;
        }
        else return -1;// ��ȡԭ�ļ�  
    sprintf(sLine, "%s.tmp", filename);
    if (NULL == (fpw = fopen(sLine, "w")))
        return -1;// д����ʱ�ļ�        
    while (NULL != fgets(sLine, 1024, fpr)) {
        if (2 != flag) { // ����ҵ�Ҫ�޸ĵ���һ�У��򲻻�ִ���ڲ��Ĳ���  
            wTmp = strchr(sLine, '=');
            if (flag == 1) {
                if (NULL != wTmp)//�����Ƿ���=
                {
                    if (0 == strncmp(key, sLine, strlen(key))) { // �������ļ���ȡ��Ϊ׼ 
                        flag = 2;// ����ֵ������д���ļ�  
                        sprintf(wTmp + 1, " %s\n", val);
                    }
                }
                else
                {
                    flag = 2;// ������ӣ����ټ������
                    sprintf(sLine, "%s = %s\n\n", key, val);//������д���ļ�
                }
            }
            else {
                if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // �������ļ���ȡ��Ϊ׼
                    flag = 1; // �ҵ�����λ��  
                }
            }
        }
        fputs(sLine, fpw); // д����ʱ�ļ� 
    }
    if (flag == 0)
    {
        sprintf(sLine, "%s\n%s = %s\n\n", sTitle, key, val);
        fputs(sLine, fpw);
    }
    fclose(fpr);
    fclose(fpw);
    sprintf(sLine, "%s.tmp", filename);
    remove(filename);
    return rename(sLine, filename);// ����ʱ�ļ����µ�ԭ�ļ�  
}
int PutIniKeyString(const char* title, const char* key, float a, const char* filename)
{
    char val[30];
    strcpy(val,std::to_string((double)a).c_str());
    FILE* fpr, * fpw;
    int  flag = 0;
    char sLine[1024], sTitle[32], * wTmp;
    sprintf(sTitle,"[%s]", title);
    if (NULL == (fpr = fopen(filename, "r")))
        if (NULL != (fpr = fopen(filename, "w")))
        {
            
            //sprintf(sLine, "%s\n%s = %s\n\n", sTitle, key, val);
            fprintf(fpr, "%s\n%s = %s\n\n", sTitle, key, val);
            fputs(sLine, fpr);
            fclose(fpr);
            return 0;
        }
        else return -1;// ��ȡԭ�ļ� 
    sprintf(sLine, "%s.tmp", filename);
    if (NULL == (fpw = fopen(sLine, "w")))
        return -1;// д����ʱ�ļ�        
    while (NULL != fgets(sLine, 1024, fpr)) {
        if (2 != flag) { // ����ҵ�Ҫ�޸ĵ���һ�У��򲻻�ִ���ڲ��Ĳ���  
            wTmp = strchr(sLine, '=');
            if (flag == 1) {
                if (NULL != wTmp)//�����Ƿ���=
                {
                    if (0 == strncmp(key, sLine, strlen(key))) { // �������ļ���ȡ��Ϊ׼ 
                        flag = 2;// ����ֵ������д���ļ�  
                        sprintf(wTmp + 1, " %s\n", val);
                    }
                }
                else
                {
                    flag = 2;// ������ӣ����ټ������
                    sprintf(sLine, "%s = %s\n\n", key, val);//������д���ļ�
                }
            }
            else {
                if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // �������ļ���ȡ��Ϊ׼
                    flag = 1; // �ҵ�����λ��  
                }
            }
        }
        fputs(sLine, fpw); // д����ʱ�ļ� 
    }
    if (flag == 0)
    {
        sprintf(sLine, "%s\n%s = %s\n\n", sTitle, key, val);
        fputs(sLine, fpw);
    }
    fclose(fpr);
    fclose(fpw);
    sprintf(sLine, "%s.tmp", filename);
    remove(filename);
    return rename(sLine, filename);// ����ʱ�ļ����µ�ԭ�ļ�  
}
int PutIniKeyString(const char* title, const char* key, int a, const char* filename)
{
    char val[30];
    strcpy(val,(char*)std::to_string(a).c_str());
    FILE* fpr, * fpw;
    int  flag = 0,change = 0;
    int i = 0;
    char sLine[1024], sTitle[32], * wTmp,*left,*right;
    sprintf(sTitle, "[%s]", title);
    if (NULL == (fpr = fopen(filename, "r")))
        if (NULL != (fpr = fopen(filename, "w")))
        {
            
            //sprintf(sLine, "%s\n%s = %s\n\n", sTitle, key, val);
            fprintf(fpr, "%s\n%s = %s\n\n", sTitle, key, val);
            fputs(sLine, fpr);
            fclose(fpr);
            return 0;
        }
        else return -1;// ��ȡԭ�ļ� 
    sprintf(sLine, "%s.tmp", filename);
    if (NULL == (fpw = fopen(sLine, "w")))
        return -1;// д����ʱ�ļ�        
    while (NULL != fgets(sLine, 1024, fpr)) {
        if (2 != flag) { // ����ҵ�Ҫ�޸ĵ���һ�У��򲻻�ִ���ڲ��Ĳ���  
            wTmp = strchr(sLine, '=');
            if(flag ==1){
                if (NULL != wTmp)//�����Ƿ���=
                {
                    if (0 == strncmp(key, sLine, strlen(key))) { // �������ļ���ȡ��Ϊ׼ 
                        flag = 2;// ����ֵ������д���ļ�  
                        sprintf(wTmp + 1, " %s\n", val);
                    }
                }
                else
                {
                    flag = 2;// ������ӣ����ټ������
                    sprintf(sLine, "%s = %s\n\n", key, val);//������д���ļ�
                }
            }
            else {
                if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // �������ļ���ȡ��Ϊ׼
                    flag = 1; // �ҵ�����λ��  
                }
            }
        }
        fputs(sLine, fpw); // д����ʱ�ļ� 
    }
    if(flag == 0)
    {
        sprintf(sLine,"%s\n%s = %s\n\n", sTitle, key, val);
        fputs(sLine, fpw);
    }
    fclose(fpr);
    fclose(fpw);
    sprintf(sLine, "%s.tmp", filename);
    remove(filename);
    return rename(sLine, filename);// ����ʱ�ļ����µ�ԭ�ļ�  
}
