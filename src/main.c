#include "SVGParser.h"
#include <assert.h>

//Test harness
int main(){

    SVGimage* img = NULL;
    char* imageString = NULL;
    int rectCount;
    Rectangle* rect;
    List* list;
    printf("Acessing a image that doesnt exist\n");
    img = createValidSVGimage("notAFile.svg","testFiles/svg.xsd");
    assert(img == NULL);
    printf("\033[32m" "\tPASS\n" "\033[0m");


    printf("NULL as file name\n");
    img = createValidSVGimage(NULL,"testFiles/svg.xsd");
    assert(img == NULL);
    printf("\033[32m" "\tPASS\n" "\033[0m");


    printf("NULL as schema\n");
    img = createValidSVGimage(NULL,NULL);
    assert(img == NULL);
    printf("\033[32m" "\tPASS\n" "\033[0m");

    printf("NULL as schema\n");
    img = createValidSVGimage("testFiles/Emoji_poo.svg",NULL);
    assert(img == NULL);
    printf("\033[32m" "\tPASS\n" "\033[0m");

    printf("test quad\n");
    img = createValidSVGimage("testFilesA2/quad01_A2.svg","testFilesA2/svg.xsd");
    assert(img != NULL);
    printf("\033[32m" "\tPASS\n" "\033[0m");
    deleteSVGimage(img);

    printf("invalid schema and file name\n");
    img = createValidSVGimage("testFiles/Emoji_poo.svg","not a schema.xsd");
    assert(img == NULL);
    printf("\033[32m" "\tPASS\n" "\033[0m");

    printf("Acessing a image that is not valid xml\n");
    img = createValidSVGimage("testFiles/broken.svg","testFiles/svg.xsd");
    assert(img == NULL);
    printf("\033[32m" "\tPASS\n" "\033[0m");

    printf("Opening valid svg file rect width Units\n");
    img = createValidSVGimage("testFiles/rec_with_units.svg","testFiles/svg.xsd");
    assert(img != NULL);
    printf("\033[32m" "\tPASS\n" "\033[0m");

    printf("Testing nameSpace\n");
    assert(strcmp(img->namespace,"http://www.w3.org/2000/svg") == 0);
    printf("\033[32m" "\tPASS\n" "\033[0m");

    printf("Testing title\n");
    assert(strcmp(img->title,"Example rect - simple revctange wiuth units") == 0);
    printf("\033[32m" "\tPASS\n" "\033[0m");

    printf("Testing description\n");
    assert(strcmp(img->description," ") == 0);
    printf("\033[32m" "\tPASS\n" "\033[0m");

    /*
    printf("Converting image to string\n");
    imageString = SVGimageToString(img);
    assert(imageString != NULL);
    if(imageString){
      printf("%s\n",imageString);

    }
    if(imageString){
      free(imageString);
    }
    */
    printf("\033[32m" "\tPASS\n" "\033[0m");



    printf("Testing rect count... with 285\n");
    rectCount = numRectsWithArea(img,285);
    assert(rectCount == 1);
    printf("\033[32m" "\tPASS: 1 rect found\n" "\033[0m");

    printf("Testing rect count... with 284\n");
    rectCount = numRectsWithArea(img,284);
    assert(rectCount == 0);
    printf("\033[32m" "\tPASS: 0 rect found\n""\033[0m");

    //tests first rect to see units
    printf("testing rect\n");
    list = getRects(img);
    rect = (Rectangle*)getFromFront(list);
    assert(strcmp(rect->units,"cm") == 0);
    printf("\033[32m" "\tPass: UNITS\n""\033[0m");
    assert(rect->x == 1 && rect->y == 1);
    printf("\033[32m" "\tPass: x and y\n""\033[0m");
    assert(getLength(rect->otherAttributes) == 3);
    printf("\033[32m" "\tPass: otherAttributes length\n""\033[0m");
    freeList(list);

    printf("num of attributes %d\n",numAttr(img));
    assert(numAttr(img) == 7);
    printf("\033[32m" "\tPASS: attribute length\n""\033[0m");

    //testing that all other lists exist but are empty
    printf("Checking that other list exist but are empty\n");
    printf("Circles\n");
    list = getCircles(img);
    assert(getLength(list) == 0);
    freeList(list);
    printf("\033[32m" "\tPASS:Circles\n""\033[0m");

    printf("paths\n");
    list = getPaths(img);
    assert(getLength(list) == 0);
    freeList(list);
    printf("\033[32m" "\tPASS:paths\n""\033[0m");

    printf("Groups\n");
    list = getGroups(img);
    assert(getLength(list) == 0);
    freeList(list);
    printf("\033[32m" "\tPASS:groups\n""\033[0m");


    deleteSVGimage(img);


    printf("Opening valid svg file Emoji_poo\n");
    img = createValidSVGimage("testFiles/Emoji_poo.svg","testFiles/svg.xsd");
    assert(img != NULL);
    printf("\033[32m" "\tPASS\n" "\033[0m");
        writeSVGimage(img,"test.svg");

        assert(validateSVGimage(img,"testFiles/svg.xsd"));

    printf("Passing NULL to all functions\n");
    printf("if it doesnt crash it passes\n");
    char *s = SVGimageToString(NULL);
    free(s);
    deleteSVGimage(NULL);
    getRects(NULL);
    getCircles(NULL);
    getGroups(NULL);
    getPaths(NULL);
    assert(numRectsWithArea(NULL,0) == 0);
    assert(numCirclesWithArea(NULL,0) == 0);
    assert(numPathsWithdata(NULL,NULL) == 0);
    assert(numGroupsWithLen(NULL,0) == 0);
    assert(numAttr(NULL) == 0);
    deleteAttribute(NULL);
    s = attributeToString(NULL);
    free(s);
    compareAttributes(NULL,NULL);
    deleteGroup(NULL);
        s = groupToString(NULL);
    free(s);
    compareGroups(NULL,NULL);
    deleteRectangle(NULL);
    s =    rectangleToString(NULL);
free(s);
    compareRectangles(NULL,NULL);
    deleteCircle(NULL);
   s =  circleToString(NULL);
    free(s);
    compareCircles(NULL,NULL);
    deletePath(NULL);
   s =  pathToString(NULL);
   free(s);
    comparePaths(NULL,NULL);

    printf("\033[32m" "\tPASS\n""\033[0m");

    printf("passing invalid values to getwidth function\n");
    assert(!numRectsWithArea(img,-1));
    assert(!numCirclesWithArea(img,-1));
    assert(!numPathsWithdata(img,""));
    assert(!numGroupsWithLen(img,-1));

    printf("\033[32m" "\tPASS\n""\033[0m");


    printf("Rects count\n");
    list = getRects(img);
    assert(!getLength(list));
    freeList(list);

    printf("paths count\n");
    list = getPaths(img);
    assert(getLength(list) == 6);
    freeList(list);

    printf("Circles count\n");
    list = getCircles(img);
    assert(getLength(list) == 2);
    freeList(list);

    printf("groups count\n");
    list = getGroups(img);
    assert(getLength(list) == 1);
    freeList(list);

    printf("DELETE svg\n");
    deleteSVGimage(img);




    printf("Opening valid svg .svg\n");
    img = createValidSVGimage("testFiles/test.svg","testFiles/svg.xsd");
    assert(img != NULL);
    printf("\033[32m" "\tPASS\n" "\033[0m");
    writeSVGimage(img,"test.svg");
    assert(validateSVGimage(img,"testFiles/svg.xsd") == false);

    deleteSVGimage(img);
    

    printf("2. Opening valid svg .svg\n");
    img = createValidSVGimage("testFilesA2/Emoji_grinning.svg","testFiles/svg.xsd");
    assert(img != NULL);
    printf("\033[32m" "\tPASS\n" "\033[0m");
    Attribute* attribute = malloc(sizeof(Attribute));
    attribute->name = malloc(sizeof(char)*(strlen("fillmka")+1));
    strcpy(attribute->name,"fill");
    attribute->value = malloc(sizeof(char)*(strlen("#111111")+1));
    strcpy(attribute->value,"#111111");
    setAttribute(img,PATH,3,attribute);

    Circle* circle = malloc(sizeof(Circle));
    circle->cx = 10;
    circle->cy = 100;
    circle->r = 30;
    strcpy(circle->units,"cm");
    circle->otherAttributes = initializeList(attributeToString,deleteAttribute,compareAttributes);
    addComponent(img,CIRC, circle);

    Rectangle* rect2= malloc(sizeof(Rectangle));
    rect2->x = 123;
    rect2->y = 50;
    rect2->width = 30;
    rect2->height = 3;
    strcpy(rect2->units,"cm");
    rect2->otherAttributes = initializeList(attributeToString,deleteAttribute,compareAttributes);
    addComponent(img,RECT,rect2);
    Attribute* attribute2 = malloc(sizeof(Attribute));
    attribute2->name = malloc(sizeof(char)*(strlen("width")+1));
    strcpy(attribute2->name,"width");
    attribute2->value = malloc(sizeof(char)*(strlen("#111111")+1));
    strcpy(attribute2->value,"420");
    setAttribute(img,RECT,0,attribute2);

    Attribute* attribute3 = malloc(sizeof(Attribute));
    attribute3->name = malloc(sizeof(char)*(strlen("stroke")+1));
    strcpy(attribute3->name,"stroke");
    attribute3->value = malloc(sizeof(char)*(strlen("#111111")+1));
    strcpy(attribute3->value,"#696969");
    setAttribute(img,GROUP,20,attribute3);
    deleteAttribute(attribute3);
        writeSVGimage(img,"test.svg");
    assert(validateSVGimage(img,"testFiles/svg.xsd")  == true);


    deleteSVGimage(img);


    printf("Opening valid svg .svg\n");
    img = createValidSVGimage("testFilesA2/Emoji_grinning.svg","testFiles/svg.xsd");
    assert(img != NULL);
    char* svgString = SVGtoJSON(img);
    printf("%s\n",svgString);
    free(svgString);

   
    deleteSVGimage(img);

     Path* path = malloc(sizeof(Path));
    path->data = malloc(sizeof(char));
    deletePath(path);
    
    printf("Freeing memory use valgrind to check for leaks\n");
    return 0;
}
