#ifndef VIEW_H
#define VIEW_H

#include <stdio.h>
#include <string.h>
#include "type.h"

typedef struct
{
    char *srcmp3_fname;
    FILE *fptr_mp3;
    char tag[5];
    unsigned int size;
    char content[300];
}Toviewmp3;

Status readandvalidate(char *argv[],Toviewmp3 *Toview);
Status openviewfiles(Toviewmp3 *Toview);
Status toview(Toviewmp3 *Toview);
Status checkid3(Toviewmp3 *Toview);
Status checkversion(Toviewmp3 *Toview);
Status skipheader(Toviewmp3 *Toview);
Status readsize(Toviewmp3 *Toview);
Status readcontents(Toviewmp3 *Toview);
Status mp3view(Toviewmp3 *Toview);

#endif