//
//  main.c
//  xcodeAssgn1
//
//  Created by Jason Eddy on 2020-01-15.
//  Copyright © 2020 Jason Eddy. All rights reserved.
//

#include "SVGParser_A2.h"

int main(int argc, const char *argv[])
{
    // insert code here...
    SVGimage *image = NULL;
   image = createValidSVGimage("Emoji_smiling.svg", "svg.xsd");

    if(validateSVGimage(image, "svg.xsd") == false){
        printf("\n\n FALSEE\n");
    }
    else{
       writeSVGimage(image, "babe.svg");
    }
    
    deleteSVGimage(image);
  

            
    return 0;
}
