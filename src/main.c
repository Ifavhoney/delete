//
//  main.c
//  xcodeAssgn1
//
//  Created by Jason Eddy on 2020-01-15.
//  Copyright © 2020 Jason Eddy. All rights reserved.
//

#include "../include/SVGParser.h"

int main(int argc, const char *argv[])
{
    // insert code here...
    SVGimage *image = createSVGimage("Emoji_poo.svg");
    if (image == NULL)
    {
        printf("NULL!!");
    }
    else
    {
        printf("FOUND");
    }

    return 0;
}
