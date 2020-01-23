#include "SVGParser.h"
#include <stdlib.h>

char *printFunction(void *);
void deleteFunction(void *);
int compareFunction(const void *, const void *);
int StartsWith(const char *a, const char *b);
SVGimage* initializeObjects(void);
SVGimage *print_element_names(xmlNode *a_node, SVGimage **list);
void insertPath(SVGimage *tempList, xmlNode *cur_node, Attribute *tempData, char *storeAttribute);
Path* createPathObject( char *data, List *otherAttribute);
Circle* createCircleObject(float cx, float cy, float r, char units[50], List *otherAttributes);
Rectangle* createRectangleObject(float x, float y, float width, float height, char units[50], List* otherAttributes);
Group* createGroupObject(Rectangle *rectangle, Circle circles, Path paths, Group groups);
int hasAttribute(List *otherAttributes);
int hasAttribute(List *otherAttributes){
if(otherAttributes->length == 0){
        return 0;
    }
    else{
        return 1;
    }
}
Group* createGroupObject(Rectangle *rectangle, Circle circles, Path paths, Group groups){

return NULL;
}
Rectangle* createRectangleObject(float x, float y, float width, float height, char units[50], List* otherAttributes){
    Rectangle *rect = malloc(sizeof(Rectangle));
    rect -> x = x;
    rect -> y = y;
    rect -> width = width;
    rect -> height = height;
    rect -> otherAttributes = NULL;
    List *dummy = initializeList(attributeToString, deleteAttribute, compareAttributes);
    char* toBeInserted = calloc(strlen(units) + 4, sizeof(char *));
    
     if(strlen(units) == 0){
         strcpy(toBeInserted, " ");
        strcpy(rect -> units, toBeInserted);
    }
    else{
        strcpy(rect -> units, units);
    }
   
     if(hasAttribute(otherAttributes) == 0 || otherAttributes == NULL){
        insertBack(rect -> otherAttributes, dummy);
    }
    else{
        
        insertBack(rect -> otherAttributes, otherAttributes);
    }
    free(toBeInserted);
    free(dummy);
    return rect;

}

Path* createPathObject(char *data, List *otherAttributes){
    
    if(data == NULL){
    strcpy(data, " ");
    }
    Path *path = malloc(sizeof(Path));
    path -> data = data;
    path -> otherAttributes = NULL;

    List *dummy = initializeList(attributeToString, deleteAttribute, compareAttributes);

     if(hasAttribute(otherAttributes) == 0 || otherAttributes == NULL){
        insertBack(path -> otherAttributes, dummy);
    }
    else{
        
        insertBack(path -> otherAttributes, otherAttributes);
    }
    free(dummy);
    
    return path;

}

/*
class Circle{
    double cx = 0;
    LinkedList

    Circle(float cx ...){
        this->cx = cx
        this->LinkedList = new LinkedList<A>
    }
}
*/


Attribute* createAttribute(char* name, char* value){
   Attribute *this = malloc(sizeof(Attribute));
    this->name = calloc(strlen(name) + 4, sizeof(char * ));
     this->value = calloc(strlen(value) + 4, sizeof(char * ));

    //TO DO
    if (name == NULL || value == NULL ){
        char* str = calloc(4, sizeof(char));
        strcpy(str, "EMPTYY");
        strcpy(this -> name, str);
        strcpy(this -> value, str);
        free(str);
        return this;
    }
    

   
     strcpy(this -> name, name);
     strcpy(this -> value, value);
    
     return this;

}

///Create circle doesnt break code when otherAttributes is null
Circle* createCircleObject(float cx, float cy, float r, char units[50], List *otherAttributes){
    Circle *circle = malloc(sizeof(Circle));

    circle -> cx = cx;
    circle -> cy = cy;
    circle -> r = r;
    //added
    Attribute *dummyValue = createAttribute(" ", " ");
    
    circle -> otherAttributes = NULL;
    char* toBeInserted = calloc(strlen(units) + 4, sizeof(char *));


    if(strlen(units) == 0 || units == NULL){
        printf("empty");
        strcpy(toBeInserted, " ");
        strcpy(circle -> units, toBeInserted);
    }
    else{

        strcpy(toBeInserted, units);
        strcpy(circle -> units, toBeInserted);
    }
    if(hasAttribute(otherAttributes) == 0 || otherAttributes == NULL){
        insertBack(circle -> otherAttributes, dummyValue);
    }
    else{
        Attribute *realValue = malloc(sizeof(Attribute));
        realValue = (Attribute *) otherAttributes;
        //After insert
        Attribute *ok = malloc(sizeof(realValue));
        ok = (Attribute *) otherAttributes -> head ->  data;
        
        while(otherAttributes -> head != NULL){
            ok = (Attribute *) otherAttributes -> head ->  data;
            otherAttributes -> head = otherAttributes -> head -> next;
            insertBack(circle -> otherAttributes, ok);

            printf("%s", ok -> name);
        }
     
    }
    free(toBeInserted);
    //might cause a double free?
    free(dummyValue);
    return circle;
    
    
}


SVGimage *createSVGimage(char *fileName)
{

   //Initialize Our tempData - we'll be reusing this memory, modifying the data
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
      SVGimage* list = initializeObjects();

    //Added 1 more insert & attribute
   Attribute *value =  createAttribute("OKKK", "SEE");
    Attribute *value1 =  createAttribute("hello", "why");

     List * otherAttributes= initializeList(attributeToString,deleteAttribute, compareAttributes);
    //valgrind MISTAKE
         insertBack(otherAttributes, value);
    insertBack(otherAttributes, value1);

    
    Circle *circle = NULL;
    circle = createCircleObject(1,2,3, "dsd", otherAttributes);
    Attribute *otherValue = NULL;
       otherValue = malloc(sizeof(Attribute));
   // char *convertedCircle = circleToString(circle);

    free(circle);
    
    freeList(otherAttributes);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return list;

    //Returns the pointer of type SVGimage containing all data
}

SVGimage *print_element_names(xmlNode *a_node, SVGimage **list)
{
     Attribute *tempData = NULL;
    SVGimage *tempList = *list;
    char *storeAttribute = " ";
    xmlNode *cur_node = NULL;
    int i = 0;

    //Iterates through xmlNode given by root_element, as you can see by its name
    for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next)
    {
        i++;
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            
           // printf("i: %d node type: Element, name: %s\n", i, cur_node->name);
            if(strcmp((char *)cur_node -> name, "title") == 0){

            strcpy(tempList -> title, (char *) cur_node -> children -> content );

            }
             else if(strcmp((char *)cur_node -> name, "desc") == 0){
           strcpy(tempList -> description,  (char *)cur_node -> children -> content );

            }
            
        }
           if (cur_node->content != NULL)
           {
             // =printf("i:%d content: %s \n", i, cur_node -> content);
             
               }

        /*Uncomment the code below if you want to see the content of every node.
        

    if(strcmp((char *)cur_node->name, "circle") == 0){

           insertCircle(tempList, cur_node, tempData, storeAttribute);
        printf("%s", tempList -> circles -> head -> next -> data);
        }
        */

        
       
       /*
         if (strcmp((char *)cur_node->name, "g") == 0)
                     {

                         printf("i: %d node type: Element, name: %s\n", i, cur_node->name);

                         insertGroup(tempList, cur_node, tempData, storeAttribute);
                     }
         
         
       
   */

 
        if (strcmp((char *)cur_node->name, "path") == 0)
        {
            insertPath(tempList, cur_node, tempData, storeAttribute);
        }
        //increments by children
        print_element_names(cur_node->children, &tempList);
    }

    return tempList;
       
}


SVGimage* initializeObjects(){
    SVGimage *list = NULL;
         list = malloc(sizeof(SVGimage));

    list->paths = initializeList(pathToString, deletePath, comparePaths);
    list->circles =initializeList(circleToString, deleteCircle, compareCircles);
    list->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);;
    list->groups = initializeList(groupToString, deleteGroup, compareGroups);
        list->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);


    //initialize
    return list;
}



//helper functions

void deleteSVGimage(SVGimage *img){
  freeList(img -> paths);
    freeList(img -> circles);
    freeList(img -> rectangles);
    freeList(img -> groups);
    freeList(img -> otherAttributes);
    free(img);
}


void deleteAttribute(void *data){
    
    if(data == NULL){
        return;
    }
    Attribute *this = data;
     free(this -> name);
     free(this -> value);
     free(this);
}
char *attributeToString(void *data){
    
    Attribute *newData = (Attribute *)data;
    char *newBuffer = (char *)calloc(2,sizeof(Attribute) + 10);
    strcat(newBuffer, newData->name);
    strcat(newBuffer, "=");
    strcat(newBuffer, newData->value);
    strcat(newBuffer, "\0");
    //ATTRIBUTE HAS A LIST???

    return newBuffer;
    
}
int compareAttributes(const void *first, const void *second){
    return 0;
}
void deleteCircle(void *data){
    if(data == NULL){
        return;
    }
  Circle *this = data;
  freeList(this -> otherAttributes);
  free(this);
}
char *circleToString(void *data){

    if(data == NULL){
        return " ";
    }
    
        Circle *this = data;
        char *value = calloc(6, sizeof(data) *  2);
    char *dummy = malloc(sizeof(data));
    sprintf(dummy, "%.2f",  this -> cx);
    
        strcat(value, dummy);
    sprintf(dummy, "%.2f",  this -> cy);
        strcat(value, dummy);
    
        sprintf(dummy, "%.2f",  this -> r);
        strcat(value, dummy);
        strcpy(dummy, this -> units);
        strcat(value, dummy);
            printf("%s", value);
            
    if(this -> otherAttributes != NULL){
        printf("i got here");
        Attribute *otherValue = NULL;
         otherValue = malloc(sizeof(Attribute));
         
        otherValue = (Attribute *)this -> otherAttributes -> head -> data;
        printf("%s", otherValue -> name) ;
    }
    
    
    
    free(dummy);
    return value;
}
int compareCircles(const void *first, const void *second){

    return 0;
}

void deletePath(void *data){
    if(data == NULL){
        return;
    }
    Path *this = data;
    free(this -> data);
    freeList(this -> otherAttributes);
    free(this);
}
char *pathToString(void *data){
    
    return " ";
}
int comparePaths(const void *first, const void *second){return 0;}

void deleteRectangle(void *data){
    if(data == NULL){
        return;
    }
  Rectangle *this = data;
  freeList(this -> otherAttributes);
  free(this);
}
char *rectangleToString(void *data){return " ";}
int compareRectangles(const void *first, const void *second){return 0;}

void deleteGroup(void *data){
     if(data == NULL){
        return;
    }
    Group *this = data;

    //TO DO

}
char *groupToString(void *data){
    return " ";
}
int compareGroups(const void *first, const void *second){
    return 0;
};

int StartsWith(const char *a, const char *b)
{
    if (strncmp(a, b, strlen(b)) == 0)
        return 1;
    return 0;
}
//Need to implenet other atributes (e.g fill)
void insertPath(SVGimage *tempList, xmlNode *cur_node, Attribute *tempData, char *storeAttribute)
{

}
