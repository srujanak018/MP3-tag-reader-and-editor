#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"

Status readandvalidate(char *argv[],Toviewmp3 *Toview){
    if(strcmp(argv[1],"-v")!=0){
        return e_failure;
    }
    if(strstr(argv[2],".mp3")==NULL){
        return e_failure;
    }
    Toview->srcmp3_fname=argv[2];
    return e_success;
}

Status openviewfiles(Toviewmp3 *Toview){
    Toview->fptr_mp3=fopen(Toview->srcmp3_fname,"r");

    if(Toview->fptr_mp3==NULL){
        return e_failure;
    }
    return e_success;
}

Status checkid3(Toviewmp3 *Toview){
    char id[4];
    rewind(Toview->fptr_mp3);       //move file pointer to beginung
    fread(id,1,3,Toview->fptr_mp3);  //read first 3 bytes
    id[3]='\0';                      //add null character

    //check its present
    if(strcmp(id,"ID3")==0){
        return e_success;
    }

    return e_failure;
}

Status checkversion(Toviewmp3 *Toview){
    unsigned char version[2];
    fread(version,1,2,Toview->fptr_mp3);
    printf("ID3 Version : 2.%d%d\n",version[0],version[1]);
    return e_success;
}

Status skipheader(Toviewmp3 *Toview){
    fseek(Toview->fptr_mp3,10,SEEK_SET);
    return e_success;
}

Status readsize(Toviewmp3 *Toview){
    unsigned char size[4];
    fread(size,1,4,Toview->fptr_mp3);    //read 4 byte data
    Toview->size = (size[0] << 24) | (size[1] << 16) | (size[2] << 8)  | size[3];
    return e_success;
}

Status readcontents(Toviewmp3 *Toview){
    fseek(Toview->fptr_mp3,1,SEEK_CUR);             //skip encoding byte
    fread(Toview->content,1,Toview->size-1,Toview->fptr_mp3);       //reading the content from the file 
    Toview->content[Toview->size-1]='\0';               //add null character
}

Status mp3view(Toviewmp3 *Toview){
    char frame[5];
    //to read 6 frame
    for(int i=0;i<6;i++){
        fread(frame,1,4,Toview->fptr_mp3);      //read the frame id
        frame[4]='\0';
        
        readsize(Toview);                     //read frame size
        fseek(Toview->fptr_mp3,2,SEEK_CUR);   //skip 2 byte flag
        fread(Toview->content,1,Toview->size,Toview->fptr_mp3);    //read frame content
        Toview->content[Toview->size]='\0';     //add null character

        //title
        if(strcmp(frame,"TIT2")==0){
            printf("Title : %s\n",Toview->content+1);
        }
        //artist
        else if(strcmp(frame,"TPE1")==0){
            printf("Artist : %s\n",Toview->content+1);
        }
        //album
        else if(strcmp(frame,"TALB")==0){
            printf("Album : %s\n",Toview->content+1);
        }
        //year
        else if(strcmp(frame,"TYER")==0){
            printf("Year : %s\n",Toview->content+1);
        }
        //genre
        else if(strcmp(frame,"TCON")==0){
            printf("Genre : %s\n",Toview->content+1);
        }
        //comment
        else if(strcmp(frame,"COMM")==0){
            printf("Comment : %s\n",Toview->content+1);
        }

    }
    return e_success;
}

Status toview(Toviewmp3 *Toview){
    printf("-------------------------------------------\n");
    printf("         mp3 tag reader -view              \n");
    printf("-------------------------------------------\n");
    
    //check ID3 tag
    if(checkid3(Toview)==e_failure){
        printf("Error:ID3 tag not found\n");
        return e_failure;
    }
    
    //check ID3 version
    if(checkversion(Toview)==e_failure){
        return e_failure;
    }

    //to skip header
    if(skipheader(Toview)==e_failure){
        return e_failure;
    }

    //display tags
    if(mp3view(Toview)==e_failure){
        return e_failure;
    }

    printf("-------------------------------------------\n");

    fclose(Toview->fptr_mp3);        //close the file
    return e_success;
}