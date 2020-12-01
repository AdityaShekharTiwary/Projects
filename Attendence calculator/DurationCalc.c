#include<stdio.h>
#include<string.h>

typedef struct { char var[2][30]; }username;
username usr[58];
typedef struct { char fields[3][30]; }record;
typedef struct { int t[3]; }time;
time tmp;

int getList()
{    
	FILE *user = fopen("Username_list.txt","r");
    if(user == NULL){
        printf("Username List File could not be opened!!");
        return 0;
	}
    int i = 0;
    while(i < 58)
		fscanf(user,"%[^\t] %[^\n] ",usr[i++].var[0],usr[i].var[1]);
	fclose(user);
	return 1;
}

time conv(char* date){
	int i,count = 0;
	char w[3] = "";
	for(i = 0; count < 3;i++)
		if(*(date+i) != ':' && *(date+i) != ' ')
			strncat(w,(date+i),1);
		else{
			tmp.t[count++] = atoi(w);
			strcpy(w,"");
		}
	strncat(w,date+i,2);
	if(tmp.t[0] != 12 && strcmp(w,"PM") == 0)
		tmp.t[0] += 12;
	else if(tmp.t[0] == 12 && strcmp(w,"AM") == 0)
		tmp.t[0] -= 12;	
	return tmp;
}

double calcTime(char* exit, char* entry){
	time ex = conv(exit);
	time ent = conv(entry);
	if(ex.t[2] < ent.t[2]){
		ex.t[2] += 60;
		ex.t[1]--;
	}
	if(ex.t[1] < ent.t[1]){
		ex.t[1] += 60;
		ex.t[0]--;
	}
	return (ex.t[0] - ent.t[0])*60 + (ex.t[1] - ent.t[1]) + (ex.t[2] - ent.t[2])/60.0;
}

char* replaceUser(char* name)
{
	int i = 0;
	for(i = 0; i < 58; i++)
		if(strcmpi(name,usr[i].var[1]) == 0)
			return usr[i].var[0];
	return name;
}

int main(){
	char fileadd[256],end_time[20], lastname[50] = "";
	
    if(!getList())
		return 0;
	
	printf("\n\t\tWELCOME TO THE PORTAL\n");
	printf("\t   ==============================\n\n");
	printf("Enter the full address of the attendance list\n(or only the file name, if the file exists in the same directory as this C program):\n");
    gets(fileadd);
    
    FILE *fp = fopen(fileadd,"r");
    if(fp == NULL){
        printf("FILE NOT FOUND.");
        return 0;
    }
    printf("\nSuccessfully opened %s.\n",fileadd);
    
    printf("\nEnter the time of ending of the class (format= 'HH:MM:SS AM/PM') eg : 1:29:36 PM :\n");
    gets(end_time);
    
    record entry[2];
    //FILE needs to be re-created as the original has a lot of formatting due to which data access fromt the file becomes much complicated.
    
    FILE *copy = fopen("formatted.txt","w");
    FILE *out = fopen("DurationList.csv","w");
    if(copy == NULL || out == NULL){
    	printf("ERROR\n");
    	return 0;
	}
	
    int count = 0,fcount = 0,rcount = 0,ecount = 0;
    char w[50] = "", c = fgetc(fp);
    double duration = 0;
    
    printf("\n SL.\t    %-35s %s\n","NAME","DURATION");
    printf("==============================================================\n");
    fprintf(out,"%s,%s,%s\n","Sl. No.","NAME","DURATION (in Mins)");
    
    while(!feof(fp)){
    	if(c > 0){
    		if(c != '\t' && c != '\n' && c != ',')
    		{
    			if(c == '(' && fcount == 0)
    			{
    				while(fgetc(fp) != ')');
    				count--;
				}
				else
    				w[count++] = c;
    		}
    		else{
    			if(c == ','){
    				strcpy(w,"");
    				count = 0;
    				fgetc(fp);fgetc(fp);
    				c = fgetc(fp);
    				continue;
				}
    			w[count] = '\0';
    			strcpy(entry[0].fields[fcount],w);
    			if(fcount++ == 2){
    				fprintf(copy,"%s\t%s\t%s\n",entry[0].fields[0],entry[0].fields[1],entry[0].fields[2]);
    				if(entry[1].fields[0] == "" || strcmp(entry[1].fields[0],"Full Name") == 0){}
    				else if(strcmp(entry[1].fields[0],entry[0].fields[0]) == 0){
    					if((strcmp(entry[1].fields[1],"Joined") == 0 || strcmp(entry[1].fields[1],"Joined before") == 0) && strcmp(entry[0].fields[1],"Left") == 0)
    						duration += calcTime(entry[0].fields[2],entry[1].fields[2]);
					}
					else if(rcount > 1) {
						if(strcmp(entry[1].fields[1],"Joined") == 0 || strcmp(entry[1].fields[1],"Joined before") == 0 || duration == 0)
							duration += calcTime(end_time,entry[1].fields[2]);
						strcpy(entry[1].fields[0],replaceUser(entry[1].fields[0]));
						printf("%3d\t%-35s %9.5f Mins\n",++ecount,entry[1].fields[0],duration);
						fprintf(out,"%d,%s,%lf\n",ecount,entry[1].fields[0],duration);
						duration = 0;
					}
					entry[1] = entry[0];
					rcount++;
				}
    			fcount %= 3;
    			strcpy(w,"");
    			count = 0;
			}
    	}
		c = fgetc(fp);
	}
	
	if(strcmp(entry[1].fields[1],"Joined") == 0 || strcmp(entry[1].fields[1],"Joined before") == 0)
		duration += calcTime(end_time,entry[1].fields[2]);
	strcpy(entry[1].fields[0],replaceUser(entry[1].fields[0]));
	printf("%3d\t%-35s %9.5f Mins\n",++ecount,entry[1].fields[0],duration);
	fprintf(out,"%d,%s,%f\n",ecount,entry[1].fields[0],duration);
	
	printf("\n\nThe above data can also be found in the DurationList.csv file which has been created in the local directory of this C program.\n");
	printf("\nThe de-formatted file of the original %s file has also been created for any further use. With name formatted.txt, saved in the local directory.\n",fileadd);
	
	fclose(fp);
	fclose(copy);
	fclose(out);
	
	return 0;
}
