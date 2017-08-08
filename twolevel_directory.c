#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct file
{
    char fileName[20];
    struct file *next;
};

struct dir
{
    char dirName[20];
    struct dir *nextDir;
    struct dir *childDir;
    struct file *nextFile;
};

void display_directory(struct dir **next);
void add_directory(struct dir **next);
void add_file(struct dir **next);
void search_file(struct dir **next,char fName[]);
void delete_directory(struct dir **next,char dirName[]);
void delete_file(struct dir **next,char fName[]);
struct dir *search_directory(struct dir **next,char a[]);

int main()
{   
    struct dir *directory,*temp,*subDir;
    directory=(struct dir *)malloc(sizeof(struct dir));
    strcpy(directory->dirName,"root");
    directory->nextDir=NULL;
    directory->childDir=NULL;
    directory->nextFile=NULL;
    int ch;
    char dirName[20],fileName[20];
    while(1)
    {
        printf("\n Press 1 To Create a Directory In Root");
        printf("\n Press 2 To Create a Sub Directory");
        printf("\n Press 3 To Display Directory Structure");
        printf("\n Press 4 To Search a Directory In Root");
        printf("\n Press 5 To Search a Sub Directory");
        printf("\n Press 6 To Delete Directory in Root");
        printf("\n Press 7 To Delete a Sub Directory");
        printf("\n Press 8 To Create a file in Main Directories");
        printf("\n Press 9 To Create a file in a Sub Directory");
        printf("\n Press 10 To Exit");
        printf("\n Enter Your Choice: ");
        scanf("%d",&ch);
        switch(ch)
        {
            case 1:
                add_directory(&directory);
                break;
            case 2:
                printf("Enter Root Directory Name: ");
                scanf("%s",dirName);
                temp=search_directory(&(directory->childDir),dirName);
                if(temp==NULL)
                    return;
                add_directory(&temp);
                break;
            case 3:
                display_directory(&directory);
                break;
            case 4:
                printf("Enter Directory Name: ");
                scanf("%s",dirName);
                temp=search_directory(&(directory->childDir),dirName);
                break;
            case 5:
                printf("Enter Main Directory Name: ");
                scanf("%s",dirName);
                temp=search_directory(&(directory->childDir),dirName);
                if(temp!=NULL)
                {
                    printf("Enter Sub Directory Name: ");
                    scanf("%s",dirName);
                    subDir=search_directory(&(temp->childDir),dirName);
                }
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                printf("Enter Main Directory Name: ");
                scanf("%s",dirName);
                temp=search_directory(&(directory->childDir),dirName);
                if(temp!=NULL)
                {
                    add_file(&temp);
                }
                break;
            case 9:
                printf("Enter Main Directory Name: ");
                scanf("%s",dirName);
                temp=search_directory(&(directory->childDir),dirName);
                if(temp!=NULL)
                {
                    printf("Enter Sub Directory Name: ");
                    scanf("%s",dirName);
                    subDir=search_directory(&(temp->childDir),dirName);
                    add_file(&subDir);
                }
                break;
            case 10:
                exit(0);
                break;
        }
    }
}
void delete_directory(struct dir **next,char dirName[])
{
    struct dir *temp;
	temp = (*next);
	struct dir *ptr;
	ptr = (*next);
	if (temp == NULL)
	{
		printf("No directories in ROOT\n");
		return;
	}
	if (strcmp(temp->dirName, dirName) == 0)
	{   
		(*next) = temp->nextDir;
		free(temp);
		printf("Directory Deleted\n");
		return;
	}
	temp = temp->nextDir;
	while (temp != NULL)
	{
		if (strcmp(temp->dirName, dirName) == 0)
		{
			ptr->nextDir = temp->nextDir;
			free (temp);
			printf("Directory deleted\n");
			return;
		}
		ptr = temp;
		temp = temp->nextDir;
	}
	printf("Directory Not Found");
	return;
}

void delete_file(struct dir **next,char fName[])
{
    struct file *tempfile;
	tempfile = (*next)->nextFile;
	struct file *delfile;
	delfile = tempfile;
    struct dir *temp;
    temp=(*next);
	if (tempfile == NULL)
	{
		printf("\nFile Not Found");
		return;
	}

	if (strcmp(tempfile->fileName, fName) == 0)
	{
        printf("First Case");
		(*next)->nextFile = tempfile->next;
		free(tempfile);
		printf("\nFile Deleted");
		return;
	}

	tempfile = tempfile->next;

	while (tempfile != NULL)
	{
		if (strcmp(tempfile->fileName, fName) == 0)
		{
			delfile->next = tempfile->next;
			free(tempfile);
			printf("Deleted File\n");
			return;
		}
		delfile = tempfile;
		tempfile = tempfile->next;
	}
    temp=temp->nextDir;
    struct file *ftemp;
    struct file *ptr;
    int flag=0;
    while(temp!=NULL)
    {
            ftemp=temp->nextFile;
            ptr=ftemp;
            if(strcmp(ftemp->fileName,fName)==0)
            {
                temp->nextFile=ftemp->next;
                free(ftemp);
                printf("\nFile Deleted");
                return;
            }        
            while(ftemp!=NULL)
            {   
                printf("\n%s",ftemp->fileName);
                if(strcmp(ftemp->fileName,fName)==0)
                {
                    flag=1;
                    printf("\nFile Found in Directory %s",temp->dirName);
                    ptr->next=ftemp->next;
                    free(ftemp);
                    printf("\nFile Deleted");
                    return;
                }
                ptr=ftemp;
                ftemp=ftemp->next;
            }
            temp=temp->nextDir;
    }

	printf("\nFile Not Found");
	return;
}

void search_file(struct dir **directory,char fName[])
{
    struct dir *temp;
    struct file *ftemp;
    temp=(*directory);
    int flag=0;
    if(temp==NULL)
    {
        printf("\nRoot has No Directories\n");
        return;
    }
    else
    {
        while(temp!=NULL)
        {
            ftemp=temp->nextFile;
            while(ftemp!=NULL)
            {
                if(strcmp(ftemp->fileName,fName)==0)
                {
                    flag=1;
                    printf("\nFile Found in Directory %s",temp->dirName);
                    break;
                }
                ftemp=ftemp->next;
            }
            if(flag)
            {
                    break;
            }
            temp=temp->nextDir;
        }
        if(!flag)
        {
            printf("\nFile Not Found\n");
        }
    }
}

void add_file(struct dir **directory)
{
    struct dir* nextDir;
    nextDir=(*directory);
    if(nextDir==NULL)
    {
        printf("\nCannot Add File To the Specified Directory");
        return;
    }
    struct file *temp, *newfile;
	temp = (*directory)->nextFile;
	newfile = (struct file *)malloc(sizeof(struct file));
    char dirName[20];
    printf("\nEnter File Name: ");
    scanf("%s",dirName);
	strcpy(newfile->fileName, dirName);
	newfile->next = NULL;
	if (temp == NULL)
	{
		(*directory)->nextFile = newfile;
		printf("File created\n");
		return;
	}
	while (temp->next != NULL)
	{
		temp = temp->next;
	}
	temp->next = newfile;
	printf("File created\n");
	return;
}

void display_directory(struct dir **directory)
{
    struct dir *temp;
    struct file *ftemp;
    temp=(*directory)->childDir;
    if(temp==NULL)
    {
        printf("\nRoot has No Directories\n");
        return;
    }
    else
    {
        printf("\nDirectories And Files:\n--------------------\n");
        while(temp!=NULL)
        {
            printf("\n%s",temp->dirName);
            struct dir *children;
            children=temp->childDir;
            while(children!=NULL)
            {
                printf("\n  |");
                printf("\n  --->%s(sub-dir)",children->dirName);
                struct file * childFiles;
                childFiles=children->nextFile;
                while(childFiles!=NULL)
                {
                    printf("\n\t  |");
                    printf("\n\t  --->%s(sub-dir-file)",childFiles->fileName);
                    childFiles=childFiles->next;
                }
                ftemp=temp->nextFile;
                while(ftemp!=NULL)
                {
                    printf("\n  |");
                    printf("\n  --->%s(file)",ftemp->fileName);
                    ftemp=ftemp->next;
                }
                children=children->nextDir;
            }
            
            printf("\n");
            temp=temp->nextDir;
        }
        // printf("\nFiles:\n-------------\n");
        // temp=(*directory);
        // ftemp=temp->nextFile;
        // while(ftemp!=NULL)
        // {
        //         printf("\n%s",ftemp->fileName);
        //         ftemp=ftemp->next;
        // }
    }
}

void add_directory(struct dir **next)
{
    struct dir *temp;
    temp=(*next)->childDir;
    struct dir *newDirectory;
    char dirName[20];
    if(temp==NULL)
    {   
        temp=(struct dir*)malloc(sizeof(struct dir));
        printf("\nEnter Directory Name: ");
        scanf("%s",dirName);
        strcpy(temp->dirName,dirName);
        temp->nextFile=NULL;
        temp->nextDir=NULL;
        temp->childDir=NULL;
        (*next)->childDir=temp;
        printf("\nDirectory Created\n");
        return;
    }
    else
    {
        while(temp->nextDir!=NULL)
        {
            temp=temp->nextDir;
        }
        newDirectory=(struct dir*)malloc(sizeof(struct dir));
        printf("\nEnter Directory Name: ");
        scanf("%s",dirName);
        strcpy(newDirectory->dirName,dirName);
        newDirectory->nextDir=NULL;
        newDirectory->nextFile=NULL;
        newDirectory->childDir=NULL;
        temp->nextDir=newDirectory;
        printf("\nDirectory Created\n");
        return;
    }
}

struct dir *search_directory(struct dir **directory,char dirName[])
{
    struct dir *temp;
    temp=(*directory);
    struct dir *ret_dir;
    ret_dir=(*directory);
    if(strcmp(dirName,"root")==0)
    {
        printf("\nRoot Directory");
        return temp;
    }
    if(temp==NULL)
    {
        printf("\nNo Such Directory in Root\n");
        ret_dir=NULL;
    }
    else{
        int flag=0;
        while(temp!=NULL)
        {   
            if(strcmp(temp->dirName,dirName)==0)
            {
                // printf("Directory Name is %s \t %s:",temp->dirName,dirName);
                flag=1;
                ret_dir=temp;
                break;
            }
            temp=temp->nextDir;
        }
        if(flag)
        {
            printf("\nDirectory Found in %s\n",temp->dirName);
        }
        else
        {
            ret_dir=NULL;
            printf("\nDirectory Not Found\n");
        }
    }
    return ret_dir;
}