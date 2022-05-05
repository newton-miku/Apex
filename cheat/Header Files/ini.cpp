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
    * 函数名：         GetIniKeyString
    * 入口参数：        title
    *                      配置文件中一组数据的标识
    *                  key
    *                      这组数据中要读出的值的标识
    *                  filename
    *                      要读取的文件路径
    * 返回值：         找到需要查的值则返回正确结果 0
    *                  否则返回-1
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
        // 这是注释行  
        if (0 == strncmp("//", sLine, 2)) continue;
        if ('#' == sLine[0])              continue;
        wTmp = strchr(sLine, '=');
        if ((NULL != wTmp) && (1 == flag)) {
            if (0 == strncmp(key, sLine, strlen(key))) { // 长度依文件读取的为准  
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
            if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // 长度依文件读取的为准  
                flag = 1; // 找到标题位置  
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
        // 这是注释行  
        if (0 == strncmp("//", sLine, 2)) continue;
        if ('#' == sLine[0])              continue;
        wTmp = strchr(sLine, '=');
        if ((NULL != wTmp) && (1 == flag)) {
            if (0 == strncmp(key, sLine, strlen(key))) { // 长度依文件读取的为准  
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
            if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // 长度依文件读取的为准  
                flag = 1; // 找到标题位置  
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
        // 这是注释行  
        if (0 == strncmp("//", sLine, 2)) continue;
        if ('#' == sLine[0])              continue;
        wTmp = strchr(sLine, '=');
        if ((NULL != wTmp) && (1 == flag)) {
            if (0 == strncmp(key, sLine, strlen(key))) { // 长度依文件读取的为准  
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
            if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // 长度依文件读取的为准  
                flag = 1; // 找到标题位置  
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
        // 这是注释行  
        if (0 == strncmp("//", sLine, 2)) continue;
        if ('#' == sLine[0])              continue;
        wTmp = strchr(sLine, '=');
        if ((NULL != wTmp) && (1 == flag)) {
            if (0 == strncmp(key, sLine, strlen(key))) { // 长度依文件读取的为准  
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
            if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // 长度依文件读取的为准  
                flag = 1; // 找到标题位置  
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
        // 这是注释行  
        if (0 == strncmp("//", sLine, 2)) continue;
        if ('#' == sLine[0])              continue;
        wTmp = strchr(sLine, '=');
        if ((NULL != wTmp) && (1 == flag)) {
            if (0 == strncmp(key, sLine, strlen(key))) { // 长度依文件读取的为准  
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
            if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // 长度依文件读取的为准  
                flag = 1; // 找到标题位置  
            }
        }
    }
    fclose(fp);
    return -1;
}

/*
    * 函数名：         PutIniKeyString
    * 入口参数：        title
    *                      配置文件中一组数据的标识
    *                  key
    *                      这组数据中要读出的值的标识
    *                  val
    *                      更改后的值
    *                  filename
    *                      要读取的文件路径
    * 返回值：         成功返回  0
    *                  否则返回 -1
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
        }else return -1;// 读取原文件  
    sprintf(sLine, "%s.tmp", filename);
    if (NULL == (fpw = fopen(sLine, "w")))
        return -1;// 写入临时文件        
    while (NULL != fgets(sLine, 1024, fpr)) {
        if (2 != flag) { // 如果找到要修改的那一行，则不会执行内部的操作  
            wTmp = strchr(sLine, '=');
            if (flag == 1) {
                if (NULL != wTmp)//此行是否有=
                {
                    if (0 == strncmp(key, sLine, strlen(key))) { // 长度依文件读取的为准 
                        flag = 2;// 更改值，方便写入文件  
                        sprintf(wTmp + 1, " %s\n", val);
                    }
                }
                else
                {
                    flag = 2;// 完成增加，不再检查内容
                    sprintf(sLine, "%s = %s\n\n", key, val);//将内容写入文件
                }
            }
            else {
                if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // 长度依文件读取的为准
                    flag = 1; // 找到标题位置  
                }
            }
        }
        fputs(sLine, fpw); // 写入临时文件 
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
    return rename(sLine, filename);// 将临时文件更新到原文件  
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
        }else return -1;// 读取原文件  
    sprintf(sLine, "%s.tmp", filename);
    if (NULL == (fpw = fopen(sLine, "w")))
        return -1;// 写入临时文件        
    while (NULL != fgets(sLine, 1024, fpr)) {
        if (2 != flag) { // 如果找到要修改的那一行，则不会执行内部的操作  
            wTmp = strchr(sLine, '=');
            if (flag == 1) {
                if (NULL != wTmp)//此行是否有=
                {
                    if (0 == strncmp(key, sLine, strlen(key))) { // 长度依文件读取的为准 
                        flag = 2;// 更改值，方便写入文件  
                        sprintf(wTmp + 1, " %s\n", val);
                    }
                }
                else
                {
                    flag = 2;// 完成增加，不再检查内容
                    sprintf(sLine, "%s = %s\n\n", key, val);//将内容写入文件
                }
            }
            else {
                if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // 长度依文件读取的为准
                    flag = 1; // 找到标题位置  
                }
            }
        }
        fputs(sLine, fpw); // 写入临时文件 
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
    return rename(sLine, filename);// 将临时文件更新到原文件  
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
        else return -1;// 读取原文件 
    sprintf(sLine, "%s.tmp", filename);
    if (NULL == (fpw = fopen(sLine, "w")))
        return -1;// 写入临时文件        
    while (NULL != fgets(sLine, 1024, fpr)) {
        if (2 != flag) { // 如果找到要修改的那一行，则不会执行内部的操作  
            wTmp = strchr(sLine, '=');
            if (flag == 1) {
                if (NULL != wTmp)//此行是否有=
                {
                    if (0 == strncmp(key, sLine, strlen(key))) { // 长度依文件读取的为准 
                        flag = 2;// 更改值，方便写入文件  
                        sprintf(wTmp + 1, " %s\n", val);
                    }
                }
                else
                {
                    flag = 2;// 完成增加，不再检查内容
                    sprintf(sLine, "%s = %s\n\n", key, val);//将内容写入文件
                }
            }
            else {
                if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // 长度依文件读取的为准
                    flag = 1; // 找到标题位置  
                }
            }
        }
        fputs(sLine, fpw); // 写入临时文件 
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
    return rename(sLine, filename);// 将临时文件更新到原文件  
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
        else return -1;// 读取原文件  
    sprintf(sLine, "%s.tmp", filename);
    if (NULL == (fpw = fopen(sLine, "w")))
        return -1;// 写入临时文件        
    while (NULL != fgets(sLine, 1024, fpr)) {
        if (2 != flag) { // 如果找到要修改的那一行，则不会执行内部的操作  
            wTmp = strchr(sLine, '=');
            if (flag == 1) {
                if (NULL != wTmp)//此行是否有=
                {
                    if (0 == strncmp(key, sLine, strlen(key))) { // 长度依文件读取的为准 
                        flag = 2;// 更改值，方便写入文件  
                        sprintf(wTmp + 1, " %s\n", val);
                    }
                }
                else
                {
                    flag = 2;// 完成增加，不再检查内容
                    sprintf(sLine, "%s = %s\n\n", key, val);//将内容写入文件
                }
            }
            else {
                if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // 长度依文件读取的为准
                    flag = 1; // 找到标题位置  
                }
            }
        }
        fputs(sLine, fpw); // 写入临时文件 
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
    return rename(sLine, filename);// 将临时文件更新到原文件  
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
        else return -1;// 读取原文件 
    sprintf(sLine, "%s.tmp", filename);
    if (NULL == (fpw = fopen(sLine, "w")))
        return -1;// 写入临时文件        
    while (NULL != fgets(sLine, 1024, fpr)) {
        if (2 != flag) { // 如果找到要修改的那一行，则不会执行内部的操作  
            wTmp = strchr(sLine, '=');
            if (flag == 1) {
                if (NULL != wTmp)//此行是否有=
                {
                    if (0 == strncmp(key, sLine, strlen(key))) { // 长度依文件读取的为准 
                        flag = 2;// 更改值，方便写入文件  
                        sprintf(wTmp + 1, " %s\n", val);
                    }
                }
                else
                {
                    flag = 2;// 完成增加，不再检查内容
                    sprintf(sLine, "%s = %s\n\n", key, val);//将内容写入文件
                }
            }
            else {
                if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // 长度依文件读取的为准
                    flag = 1; // 找到标题位置  
                }
            }
        }
        fputs(sLine, fpw); // 写入临时文件 
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
    return rename(sLine, filename);// 将临时文件更新到原文件  
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
        else return -1;// 读取原文件 
    sprintf(sLine, "%s.tmp", filename);
    if (NULL == (fpw = fopen(sLine, "w")))
        return -1;// 写入临时文件        
    while (NULL != fgets(sLine, 1024, fpr)) {
        if (2 != flag) { // 如果找到要修改的那一行，则不会执行内部的操作  
            wTmp = strchr(sLine, '=');
            if(flag ==1){
                if (NULL != wTmp)//此行是否有=
                {
                    if (0 == strncmp(key, sLine, strlen(key))) { // 长度依文件读取的为准 
                        flag = 2;// 更改值，方便写入文件  
                        sprintf(wTmp + 1, " %s\n", val);
                    }
                }
                else
                {
                    flag = 2;// 完成增加，不再检查内容
                    sprintf(sLine, "%s = %s\n\n", key, val);//将内容写入文件
                }
            }
            else {
                if (0 == strncmp(sTitle, sLine, strlen(sTitle))) { // 长度依文件读取的为准
                    flag = 1; // 找到标题位置  
                }
            }
        }
        fputs(sLine, fpw); // 写入临时文件 
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
    return rename(sLine, filename);// 将临时文件更新到原文件  
}
