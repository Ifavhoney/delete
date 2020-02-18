//
//  main.c
//  xcodeAssgn1
//
//  Created by Jason Eddy on 2020-01-15.
//  Copyright © 2020 Jason Eddy. All rights reserved.
//

#include "SVGParser.h"


int main(int argc, const char *argv[])
{
    // Everts's microsystem.
     SVGimage *image = NULL;
    image = createSVGimage("quad01_A2.svg");
    
    validateSVGimage(image, "svg.xsd");
    
    deleteSVGimage(image);
    
            
    return 0;
}

