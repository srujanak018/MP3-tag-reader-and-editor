#include <string.h>
#include <stdlib.h>
#include "type.h"
#include "edit.h"

Status readandvalidateedit(char *argv[],Toeditmp3 *edit){
    if(argv[1]==NULL || argv[2]==NULL || argv[3]==NULL ||argv[4]==NULL){
        return e_failure;
    }
    
    //check file
    if(strstr(argv[3],".mp3")==NULL){
        return e_failure;
    }

    //decide which tag to edit
    if(strcmp(argv[2],"-t")==0){
        strcpy(edit->tag,"TIT2");
    }else if(strcmp(argv[2],"-a")==0){
        strcpy(edit->tag,"TPE1");
    }else if(strcmp(argv[2],"-A")==0){
        strcpy(edit->tag,"TALB");
    }else if(strcmp(argv[2],"-y")==0){
        strcpy(edit->tag,"TYER");
    }else if(strcmp(argv[2],"-g")==0){
        strcpy(edit->tag,"TCON");
    }else if(strcmp(argv[2],"-c")==0){
        strcpy(edit->tag,"COMM");
    }else{
        return e_failure;
    }

    edit->srcmp3_fname=argv[3];
    edit->newvalue=argv[4];

    return e_success;
}

Status openeditfile(Toeditmp3 *edit){
    edit->fptr_mp3=fopen(edit->srcmp3_fname,"r+");
    if(edit->fptr_mp3==NULL){
        return e_failure;
    }

    return e_success;
}

Status toedit(Toeditmp3 *edit){
    char frame[5];
    rewind(edit->fptr_mp3);   //move pointer tot= the begining

    //to check id3 tag
    fread(frame,1,3,edit->fptr_mp3);
    frame[3]='\0';

    if(strcmp(frame,"ID3")!=0){
        printf("Error:ID3 tag not found\n");
        return e_failure;
    }
    //skip remainining 7 byte
    fseek(edit->fptr_mp3,7,SEEK_CUR);

    //reading the frames
    for(int i=0;i<6;i++){
        fread(frame,1,4,edit->fptr_mp3);
        frame[4]='\0';
        if(readeditsize(edit)==e_failure){
            return e_failure;
        }

        fseek(edit->fptr_mp3,2,SEEK_CUR);    //skip the flag
        if(strcmp(frame,edit->tag)==0){
            printf("Tag found : %s",frame);
            if(writenewvalue(edit)==e_failure){
                return e_failure;
            }
            printf("Tag edited \n");
            fclose(edit->fptr_mp3);
            return e_success;
        }
        fseek(edit->fptr_mp3,edit->size,SEEK_CUR);     //move to next frame
    }
    printf("Error: Tag not found\n");
    fclose(edit->fptr_mp3);
    return e_failure;
}

//read the frame size
Status readeditsize(Toeditmp3 *edit){
    unsigned char size[4];
    fread(size,1,4,edit->fptr_mp3);
    edit->size=(size[0]<<24) | (size[1]<<16) | (size[2]<<8) | (size[3]);
    return e_success;
}

Status writenewvalue(Toeditmp3 *edit){
    unsigned int newlength;
    newlength=strlen(edit->newvalue)+1;
    //check the size 
    if(newlength>edit->size){
        printf("Size of new value is not matching with the older value\n");
        return e_failure;
    }

    fputc(0,edit->fptr_mp3);      //write encoding byte
    fwrite(edit->newvalue,1,strlen(edit->newvalue),edit->fptr_mp3);
    //fill remaining byte with 0
    for(unsigned int i=newlength;i<edit->size;i++){
        fputc(0,edit->fptr_mp3);
    }
    fflush(edit->fptr_mp3);
    return e_success;
}