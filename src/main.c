//
//  main.c
//  xcodeAssgn1
//
//  Created by Jason Eddy on 2020-01-15.
//  Copyright © 2020 Jason Eddy. All rights reserved.
//

#include "../include/SVGParser_A2.h"

int main(int argc, const char *argv[])
{
    // insert code here...
    SVGimage *image = NULL;
   image = createSVGimage("quad01.svg");
    if (image == NULL)
    {
        printf("NULL!!");
    }
    else
    {

    }
            deleteSVGimage(image);    

    return 0;
}