#include <stdio.h>
#include <string.h>
#include "header.h"
#include "edit.h"
#include <stdlib.h>

int main(int argc,char *argv[])
{
    Toviewmp3 view;
    Toeditmp3 edit;

if(argc==2 && strcmp(argv[1],"-help")==0){
        printf("\nTo view:\n");
        printf("./a.out songname.mp3\n");

        printf("\nTo edit:\n");
        printf("./a.out -e -t song.mp3 \"New Title\"\n");
        printf("./a.out -e -a song.mp3 \"New Artist\"\n");
        printf("./a.out -e -A song.mp3 \"New Album\"\n");
        printf("./a.out -e -y song.mp3 \"2026\"\n");
        printf("./a.out -e -g song.mp3 \"Rock\"\n");
        printf("./a.out -e -c song.mp3 \"New Comment\"\n");
    }
//for view operation
else if (argc == 3){
        if (strcmp(argv[1], "-v") == 0){
            if (readandvalidate(argv, &view) == e_success){
                if (openviewfiles(&view) == e_success){
                    if (toview(&view) == e_failure){
                        printf("Unable to edit the tag\n");
                    }
                }else{
                    printf("Unable to open the mp3 file\n");
                }
            }else{
                printf("Invalid edit argument\n");
                printf("Enter ./a.out -help to take reference\n");
            }
        }else{
            printf("Invalid option\n");
        }
    }

 //for edit operation
else if(argc==5){
    if(strcmp(argv[1],"-e")==0){
        if(readandvalidateedit(argv,&edit)==e_success){
            if(openeditfile(&edit)==e_success){
                if(toedit(&edit)==e_failure){
                    printf("Unable to edit the tag\n");
                }
            }else{
                printf("Unable to open the mp3 file\n");
            }
        }else{
            printf("Invalid edit argument\nEnter ./a.out -help to take reference\n");
        }
    }else{
        printf("Invalid option\n");
    }
}
else{
    printf("Invalid argument\nEnter ./a.out -help to take reference\n");
}

return 0;
}