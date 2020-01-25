#include "SVGParser.h"
#include <stdlib.h>

char *printFunction(void *);
void deleteFunction(void *);
int compareFunction(const void *, const void *);
int StartsWith(const char *a, const char *b);
SVGimage* initializeObjects(void);
SVGimage *print_element_names(xmlNode *a_node, SVGimage **list);
Path* createPathObject(char *data);
void insertPath(SVGimage *tempList, xmlNode *cur_node);
void insertRect(SVGimage *tempList, xmlNode *cur_node);
void insertCircle(SVGimage *tempList, xmlNode *cur_node);

Attribute* createAttribute(char* name, char* value);
Circle* createCircleObject(float cx, float cy, float r, char units[50]);
Rectangle* createRectangleObject(float x, float y, float width, float height, char units[50]);
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
Rectangle* createRectangleObject(float x, float y, float width, float height, char units[50]){
    Rectangle *rect = malloc(sizeof(Rectangle));
    rect -> x = x;
    rect -> y = y;
    rect -> width = width;
    rect -> height = height;
    rect -> otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    char* toBeInserted = calloc(strlen(units) + 4, sizeof(char *));
    
     if(strlen(units) == 0){
         strcpy(toBeInserted, " ");
        strcpy(rect -> units, toBeInserted);
    }
    else{
        strcpy(rect -> units, units);
    }
   free(toBeInserted);
    return rect;

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


SVGimage *createSVGimage(char *fileName)
{

   //Initialize Our tempData - we'll be reusing this memory, modifying the data
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
      SVGimage* list = initializeObjects();

        LIBXML_TEST_VERSION
        doc = xmlReadFile(fileName, NULL, 0);
        if(doc == NULL){
            printf("error: could not parse file %s\n", fileName);

        }
        else{
         /*Get the root element node */
        root_element = xmlDocGetRootElement(doc);
        //root_element = xmlDoc
        print_element_names(root_element, &list);
       
        //printf("\n %s \n", path -> otherAttributes -> head );
         }

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

        char *parent = (char *)cur_node -> parent ->name;

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

        
        if(parent != NULL){

        if(strcmp(parent, "g") != 0){

       
        if(strcmp((char *)cur_node->name, "circle") == 0){
            
            insertCircle(tempList, cur_node);

                        
        }
         if (strcmp((char *)cur_node->name, "path") == 0)
        {

            insertPath(tempList, cur_node);

        }
            if(strcmp((char *)cur_node -> name, "rect") == 0){

                insertRect(tempList, cur_node);
            }

        /*Uncomment the code below if you want to see the content of every node.
        

  
        */

        
       
       /*
         if (strcmp((char *)cur_node->name, "g") == 0)
                     {

                         printf("i: %d node type: Element, name: %s\n", i, cur_node->name);

                         insertGroup(tempList, cur_node, tempData, storeAttribute);
                     }
         
         
       
   */

      
        //increments by children
         }
         else{
             //G
         }
          }
          else{
            printf("%d", i);
          }
        print_element_names(cur_node->children, &tempList);
    }


    return tempList;
       
}

void insertRect(SVGimage *tempList, xmlNode *cur_node){
 int i = 0;
    //printf("%s");
    xmlAttr *attr;
    float x, y, width, height;
    char *units = NULL;
    for (attr = cur_node->properties; attr != NULL; attr = attr->next)
      {
           i++;
        // printf("%d\n",i);
         xmlNode *snapshot = attr->children;
         char *getAttrValue = (char *)snapshot->content;
         char *getAttrName = (char *)attr->name;

         if(getAttrValue != NULL && getAttrName != NULL  ){
            
         //seperate them
        if(strcmp("x", getAttrName) == 0 ){
            x = atof(getAttrValue);
        }
         if(strcmp("y", getAttrName) == 0 ){
            y = atof(getAttrValue);
        }
        if(strcmp("width", getAttrName) == 0 ){
            width = atof(getAttrValue);
        }
        if(strcmp("height", getAttrName) == 0 ){
            height = atof(getAttrValue);
        }
         if(attr->next == NULL){
            //check for null
        if(units == NULL){
         units = calloc(1, strlen(getAttrValue) + 1);
        strcpy(units, "");
        }
        

        Rectangle *rectangle = createRectangleObject(x, y, width, height, units);
             
             
        xmlAttr* _attr;

    for (_attr = cur_node->properties; _attr != NULL; _attr = _attr->next){
            xmlNode *snapshot = _attr->children;
            char *getAttrValue = (char *)snapshot->content;
            char *getAttrName = (char *)_attr->name;


                if(strcmp("x", getAttrName) != 0 && strcmp("y", getAttrName) != 0 && strcmp("width", getAttrName) != 0 && strcmp("height", getAttrName) != 0
                ){
                    Attribute *attribute = createAttribute(getAttrName, getAttrValue);
                    insertBack(rectangle -> otherAttributes, attribute);
                    
                     }
    
    }
            
            insertBack(tempList -> rectangles, rectangle);

        free(units);
        
         

        }
      


      }
}
}
void insertPath(SVGimage *tempList, xmlNode *cur_node){
    int i = 0;
    //printf("%s");
    xmlAttr *attr;
    char *d = NULL;
    for (attr = cur_node->properties; attr != NULL; attr = attr->next)
      {
          
        i++;
        // printf("%d\n",i);
         xmlNode *snapshot = attr->children;
         char *getAttrValue = (char *)snapshot->content;
         char *getAttrName = (char *)attr->name;
         if(getAttrValue != NULL && getAttrName != NULL  ){
             if(strcmp("d", getAttrName) == 0 ){
                 d = calloc(1, strlen(getAttrValue) + 1);
                 strcpy(d, getAttrValue);
                }
          
          if(attr->next == NULL){
              
              Path *path = createPathObject(d);
              xmlAttr* _attr;


              for (_attr = cur_node->properties; _attr != NULL; _attr = _attr->next){
                      xmlNode *snapshot = _attr->children;
                      char *getAttrValue = (char *)snapshot->content;
                      char *getAttrName = (char *)_attr->name;


                          if(strcmp("d", getAttrName) != 0 )
                          {
                            Attribute *attribute = createAttribute(getAttrName, getAttrValue);
                            insertBack(path -> otherAttributes, attribute);

                          }
              }
              
              //removes data and otherAttributes from memory
                        insertBack(tempList -> paths, path);


             }
      }
      }
}

void insertCircle(SVGimage *tempList, xmlNode *cur_node){
    int i = 0;
    //printf("%s");
    xmlAttr *attr;
    float cx = 0, cy, r;
    char *units = NULL;
    
    for (attr = cur_node->properties; attr != NULL; attr = attr->next)
    {
         
                i++;
       // printf("%d\n",i);
        xmlNode *snapshot = attr->children;
        char *getAttrValue = (char *)snapshot->content;
        char *getAttrName = (char *)attr->name;
        if(getAttrValue != NULL && getAttrName != NULL  ){
            
         //seperate them
        
      
        if(strcmp("cx", getAttrName) == 0 ){
            cx = atof(getAttrValue);
        }

        else if(strcmp("cy", getAttrName) == 0 ){
            cy = atof(getAttrValue);

        }
       else if(strcmp("r", getAttrName) == 0 ){
             r = atof(getAttrValue);

            }
          else if(strcmp("units", getAttrName) == 0 ){
            units = calloc(1, strlen(getAttrValue) + 1);
            strcpy(units, getAttrValue);
        }
        
        if(attr->next == NULL){
            //check for null
        if(units == NULL){
         units = calloc(1, strlen(getAttrValue) + 1);
        strcpy(units, " ");
        }
        

        Circle *circle = createCircleObject(cx, cy, r, units);
        xmlAttr* _attr;

    for (_attr = cur_node->properties; _attr != NULL; _attr = _attr->next){
            xmlNode *snapshot = _attr->children;
            char *getAttrValue = (char *)snapshot->content;
            char *getAttrName = (char *)_attr->name;


                if(strcmp("cx", getAttrName) != 0 && strcmp("cy", getAttrName) != 0 && strcmp("r", getAttrName) != 0 && strcmp("units", getAttrName) != 0
                ){
                    Attribute *attribute = createAttribute(getAttrName, getAttrValue);
                    insertBack(circle -> otherAttributes, attribute);
                }
    
    }
            insertBack(tempList -> circles, circle);

        free(units);
        
         

        }

       // insertBack(tempList -> circle, circle);
    }

       
    }
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
Circle* createCircleObject(float cx, float cy, float r, char units[50]){
    Circle *circle = malloc(sizeof(Circle));

    circle -> cx = cx;
    circle -> cy = cy;
    circle -> r = r;
    //added
    
    circle -> otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
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
  
    //might cause a double free?
    free(toBeInserted);
//   deleteAttribute(dummyValue);
    return circle;
    
    
}
Path* createPathObject(char *data){
    
    if(data == NULL){
    strcpy(data, " ");
    }
    Path *path = malloc(sizeof(Path));
    path -> data = data;
    path -> otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    
    return path;

}
//helper functions

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
        Attribute *cur_attribute = NULL;
        while(this -> otherAttributes -> head != NULL){
     cur_attribute = (Attribute *) this -> otherAttributes -> head ->  data;
        strcat(value, cur_attribute -> name);
        strcat(value, cur_attribute -> value);
            this -> otherAttributes -> head = this -> otherAttributes -> head -> next;

        }
        
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
