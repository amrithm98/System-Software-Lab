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
    struct dir *mainDir;
    struct file *nextFile;
};

void display_directory(struct dir **next);
void disp_cur_dir(struct dir **next);
void add_directory(struct dir **next);
void add_file(struct dir **next);
void search_file(struct dir **next,char fName[]);
void delete_directory(struct dir **next,char dirName[]);
void delete_file(struct dir **next,char fName[]);
struct dir *search_directory(struct dir **next,char a[]);

int main()
{   
    struct dir *directory,*temp,*subDir,*current;
    directory=(struct dir *)malloc(sizeof(struct dir));
    strcpy(directory->dirName,"root");
    directory->nextDir=NULL;
    directory->childDir=NULL;
    directory->nextFile=NULL;
    directory->mainDir=NULL;
    current=directory;
    int ch;
    char dirName[20],fileName[20];
    while(1)
    {
        printf("\n Press 1 To Create a Directory Here");
        printf("\n Press 2 To Change Directory");
        printf("\n Press 3 To Display Current Directory");
        printf("\n Press 4 To Change To Parent Directory");
        printf("\n Press 5 To Display Contents Of Present Directory");
        printf("\n Press 6 Search for a directory here");
        printf("\n Press 7 To Delete a Directory Here");
        printf("\n Press 8 To Create a file Here");
        printf("\n Press 9 To Create a file In Sub Directory");
        printf("\n Press 10 To Delete a file");
        printf("\n Press 11 To Exit");
        printf("\n Enter Your Choice: ");
        scanf("%d",&ch);
        switch(ch)
        {
            case 1:
                add_directory(&(current));
                break;
            case 2:
                printf("Enter Directory Name: ");
                scanf("%s",dirName);
                temp=search_directory(&(current->childDir),dirName);
                if(temp==NULL)
                    break;
                current=temp;
                disp_cur_dir(&current);
                break;
            case 3:
                disp_cur_dir(&current);
                break;
            case 4:
                if (current->mainDir == NULL)
				{
					printf("End Reached. No parent left.\n");
					break;
				}
				current = current->mainDir;
                printf("\nDirectory Changed");
				break;
            case 5:
                display_directory(&current);
                break;
            case 6:
                printf("Enter Directory Name: ");
                scanf("%s",dirName);
                temp=search_directory(&(current->childDir),dirName);
                break;
            case 7:
                printf("Enter Main Directory Name: ");
                scanf("%s",dirName);
                delete_directory(&(current->childDir),dirName);
                break;
            case 8:
                add_file(&current);
                break;
            case 9:
                printf("Enter Main Directory Name: ");
                scanf("%s",dirName);
                temp=search_directory(&(current->childDir),dirName);
                if(temp!=NULL)
                {
                    add_file(&temp);
                }
                break;
            case 10:
                printf("Enter File Name: ");
                scanf("%s",fileName);
                delete_file(&(current->childDir),fileName);
                break;
            case 11:
                exit(0);
                break;
        }
    }
}

void disp_cur_dir(struct dir **next)
{
    struct dir *temp;
    temp=(*next)->mainDir;
    printf("\nPresent Directory is : %s-->",(*next)->dirName);
    struct dir *current;
    while(temp!=NULL)
    {
        printf("%s-->",temp->dirName);
        temp=temp->mainDir;
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
        printf("FIRST\n");
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
                children=children->nextDir;
            }
            ftemp=temp->nextFile;
                while(ftemp!=NULL)
                {
                    printf("\n  |");
                    printf("\n  --->%s(file)",ftemp->fileName);
                    ftemp=ftemp->next;
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
        temp->mainDir=(*next);
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
        newDirectory->mainDir=(*next);
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
            printf("\nDirectory Found :%s\n",temp->dirName);
        }
        else
        {
            ret_dir=NULL;
            printf("\nDirectory Not Found\n");
        }
    }
    return ret_dir;
}