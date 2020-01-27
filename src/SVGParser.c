#include "SVGParser.h"
#include <stdlib.h>

typedef struct
{
    float value;
    char * unit;

} ParsedValue;
ParsedValue *createValue(char *data);
void deleteValue(ParsedValue *data);

char *printFunction(void *);
void deleteFunction(void *);
int compareFunction(const void *, const void *);
int StartsWith(const char *a, const char *b);
SVGimage* initializeObjects(void);
SVGimage *print_element_names(xmlNode *a_node, SVGimage **list);
Path* createPathObject(char *data);

void insertPath(void *data, xmlNode *cur_node, int version);
void insertRect(void *data, xmlNode *cur_node, int version);
void insertCircle(void *data, xmlNode *cur_node, int version);
void insertGroup(void *data, xmlNode *cur_node, int version);
void insertOtherAttribute(void *data, xmlNode *cur_node);
void insertNSUnits(SVGimage *tempList, xmlNode *cur_node);
Attribute* createAttribute(char* name, char* value);
Circle* createCircleObject(float cx, float cy, float r, char units[50]);
Rectangle* createRectangleObject(float x, float y, float width, float height, char units[50]);
Group* createGroupObject(void);
void validateNameSpace(char *namespace, SVGimage **list);
void validateTitle(char *title, SVGimage **list);
void validateDescription(char *description, SVGimage **list);

int hasAttribute(List *otherAttributes);

int hasAttribute(List *otherAttributes){
if(otherAttributes->length == 0){
        return 0;
    }
    else{
        return 1;
    }
}

char* SVGimageToString(SVGimage* img){
    
    List* circleList = img -> circles;
    List* rectangleList = img -> rectangles;
    List* pathList = img -> paths;
    List* groupList = img -> groups;
    List* otherList = img ->otherAttributes;
    printf("Namespace: %s\n", img -> namespace  );
    printf("Title: %s\n", img -> title  );
    printf("Description: %s\n", img -> description  );
    printf("\nCircle: \n");
    Circle *circle = circleList -> head -> data;
    /*
    char *value;
    while(circleList != NULL){
    value = circleToString(circleList -> head -> data);
    //printf(value);
        circleList = circleList -> head -> next;
        free(value);
    }
    freeList(circleList);
    */
    return NULL;
    
}

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
        print_element_names(root_element, &list);
            validateNameSpace((char *)root_element -> ns -> href, &list);
            /*
    if(strlen(list -> title) < 1){
        strcpy(list -> title, " ");
        }
            if(strlen(list -> description) < 1){
                  strcpy(list -> description, " ");
                  }
             */
            
    
        
        
        /*
        Circle *circle = createCircleObject(1,2,3,"cm");
        Attribute *attribute = createAttribute("HELLO", "JHSDSD");
        insertBack(circle -> otherAttributes, attribute );
        deleteCircle(circle);
*/


         


         //  printf("%s", attribute -> value);
           
            
                    
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
            
                validateTitle((char *)cur_node -> children -> content, &tempList);            }
             else if(strcmp((char *)cur_node -> name, "desc") == 0){
                 validateDescription((char *)cur_node -> children -> content , &tempList);
                 
             }
        }
        if(strcmp((char *)cur_node->name, "svg") == 0){
        }


           if (cur_node->content != NULL)
           {
             // =printf("i:%d content: %s \n", i, cur_node -> content);
             
               }

        
        if(parent != NULL){
        if(strcmp(parent, "g") != 0){

       
        if(strcmp((char *)cur_node->name, "circle") == 0){

            insertCircle(tempList, cur_node, 0);

                        
        }
         if (strcmp((char *)cur_node->name, "path") == 0)
        {


            insertPath(tempList, cur_node, 0);

        }
            if(strcmp((char *)cur_node -> name, "rect") == 0){

                insertRect(tempList, cur_node, 0);
            }
              if(strcmp((char *)cur_node -> name, "g") == 0){

                insertGroup(tempList, cur_node, 0);
            }
            if(strcmp((char *)cur_node -> name, "g") != 0 && strcmp((char *)cur_node -> name, "circle") != 0 && strcmp((char *)cur_node -> name, "path") != 0 && strcmp((char *)cur_node -> name, "rect") != 0  && strcmp((char *)cur_node -> name, "text") != 0  && strcmp((char *)cur_node -> name, "comment") != 0 && strcmp((char *)cur_node -> name, "desc") != 0 && strcmp((char *)cur_node -> name, "title") != 0     ){
                insertOtherAttribute(tempList, cur_node);
                Attribute *attribute = tempList -> otherAttributes -> head -> data;
                printf("%s", attribute ->name);
            }
        /*Uncomment the code below if you want to see the content of every node.
        
        */

      
        //increments by children
         }
         else{

             //G
         }
          }
          else{
          }
        print_element_names(cur_node->children, &tempList);
    }


    return tempList;
       
}

void insertOtherAttribute(void *data, xmlNode *cur_node){
    SVGimage *list = data;

    xmlAttr *attr;
           // printf("%d\n",i);
    
    for (attr = cur_node->properties; attr != NULL; attr = attr->next)
    {
        xmlNode *snapshot = attr->children;
        char *getAttrValue = (char *)snapshot->content;
        char *getAttrName = (char *)attr->name;
        char *name = calloc(1, strlen(getAttrName) + strlen((char *)cur_node -> name)  + 10);
        strcat(name, (char *)cur_node -> name );
        strcat(name, " >");
        strcat(name, getAttrName);
        Attribute *attribute = createAttribute(name, getAttrValue);
        insertBack(list -> otherAttributes, attribute);
      
        free(name);
    }
            
            
    
    
    
    printf("\n");
    
    
}


void insertGroup(void *data, xmlNode *cur_node, int version){
     int i = 0;
    //printf("%s");
    xmlAttr *attr;
    float x = 0, y = 0, width = 0, height = 0;
    ParsedValue *parsedValue = NULL;
      Group *group;
    if(i == 0){
        group = createGroupObject();
               }
     
      attr =  cur_node->properties;
        // printf("%d\n",i);
         xmlNode *snapshot = attr->children;
         char *getAttrValue = (char *)snapshot->content;
         char *getAttrName = (char *)attr->name;
                  if(getAttrValue != NULL && getAttrName != NULL  ){
                    
                    long nodeCounter = xmlChildElementCount(cur_node);
                   xmlNode *temp_cur_children = cur_node -> children;
                      xmlNode * temp_cur_node;
                                     //Looks at the sibling of the current children
                  // printf("\n%ld\n", nodeCounter);
                      const char *validateName;

                    while(nodeCounter != 0){
                    temp_cur_node = xmlNextElementSibling(temp_cur_children);
                        if(temp_cur_node != NULL){
                            validateName = (const char *) temp_cur_node -> name;
                           // printf("%s\n", validateName);
                            if(strcmp(validateName, "rect") == 0){
                               
                                insertRect(group, temp_cur_node, 1);
                                //we'll be storing the other attributes here as well;xxw
                            }
                            
                            if(strcmp(validateName, "circle") == 0){
                                insertCircle(group, temp_cur_node, 1);

                            }
                            if(strcmp(validateName, "path") == 0){
                                insertPath(group, temp_cur_node, 1);

                            }
                            if(strcmp(validateName, "g") == 0){
                               insertGroup(group, temp_cur_node, 1);
                            }
                            
                            
                        }
                    
                        temp_cur_children = temp_cur_children -> next -> next;
                    
                        nodeCounter--;
          
                }
                      if(nodeCounter == 0){
                          if(version == 0){
                              SVGimage *list = data;
                            insertBack( list -> groups, group);

                          }
                          if(version == 1){
                              Group *list = data;
                            insertBack(list -> groups, group);

                          }
                      }

                }

               


      
}


///Version is so that we could reuse this with different types of list
void insertRect(void* data, xmlNode *cur_node, int version){
 int i = 0;
    //printf("%s");
    xmlAttr *attr;
    float x = 0, y = 0, width = 0, height = 0;
    ParsedValue *parsedValue = NULL;

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
          
            if(parsedValue == NULL){
            parsedValue = createValue(getAttrValue);
            x = parsedValue -> value;
            }
            else{
                x = atof(getAttrValue);
            }

        }
         if(strcmp("y", getAttrName) == 0 ){
            if(parsedValue == NULL){
             parsedValue = createValue(getAttrValue);
            y = parsedValue -> value;
            }
            else{
               y = atof(getAttrValue);
            }
        }
        if(strcmp("width", getAttrName) == 0 ){
            
             if(parsedValue == NULL){
             parsedValue = createValue(getAttrValue);
            width = parsedValue -> value;
            }
            else{
            width = atof(getAttrValue);

            }
        }
        if(strcmp("height", getAttrName) == 0 ){
            if(parsedValue == NULL){
             parsedValue = createValue(getAttrValue);
            height = parsedValue -> value;
            }
            else{
            height = atof(getAttrValue);
            }
        }
         if(attr->next == NULL){
            //check for null
    
        
        Rectangle *rectangle = createRectangleObject(x, y, width, height, parsedValue -> unit);
                    deleteValue(parsedValue);
      //  printf("\n%s", rectangle -> units);
        xmlAttr* _attr;
    Group *grpList = data;
    SVGimage *list = data;

    for (_attr = cur_node->properties; _attr != NULL; _attr = _attr->next){
            xmlNode *snapshot = _attr->children;
            char *getAttrValue = (char *)snapshot->content;
            char *getAttrName = (char *)_attr->name;


                if(strcmp("x", getAttrName) != 0 && strcmp("y", getAttrName) != 0 && strcmp("width", getAttrName) != 0 && strcmp("height", getAttrName) != 0
                ){
                  if(version == 1){
                        Attribute *attribute = createAttribute(getAttrName, getAttrValue);
                        insertBack(grpList -> otherAttributes , attribute);
                        }
                    else{
                        Attribute *attribute1 = createAttribute(getAttrName, getAttrValue);
                        insertBack(rectangle -> otherAttributes, attribute1);
                    }
                  
                   
                     }
    
    }
            if(version == 1){
                 insertBack(grpList -> rectangles, rectangle);

             }
             else{

                insertBack(list -> rectangles, rectangle);

             }
         

        }
      


      }
}
}
void insertPath(void *data, xmlNode *cur_node, int version){
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
              Group *grpList = data;
              SVGimage *list = data;

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
              
              if(version == 1){
                            insertBack(grpList -> paths, path);

                                     }
                                     else{

                                        insertBack(list -> paths, path);

                                     }

             }
      }
      }
}

void insertCircle(void *data, xmlNode *cur_node, int version){
    int i = 0;
    //printf("%s");
    xmlAttr *attr;
    float cx = 0, cy = 0, r = 0;
    ParsedValue *parsedValue = NULL;

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
            if(parsedValue == NULL){
             parsedValue = createValue(getAttrValue);
            cx = parsedValue -> value;
            }
            else{
            cx = atof(getAttrValue);

            }
            }

        else if(strcmp("cy", getAttrName) == 0 ){
            if(parsedValue == NULL){
             parsedValue = createValue(getAttrValue);
            cy = parsedValue -> value;
            }
            else{
            cy = atof(getAttrValue);

            }

        }
       else if(strcmp("r", getAttrName) == 0 ){
            if(parsedValue == NULL){
             parsedValue = createValue(getAttrValue);
            r = parsedValue -> value;
            }
            else{
            r = atof(getAttrValue);

            }
            }
         
        
        if(attr->next == NULL){
            //check for null
       
        
        Circle *circle = createCircleObject(cx, cy, r, parsedValue -> unit);
        deleteValue(parsedValue);

        xmlAttr* _attr;
            Group *grpList = data;
            SVGimage *list = data;
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
            
            if(version == 1){
                            insertBack(grpList -> circles, circle);

                        }
                        else{

                           insertBack(list -> circles, circle);

                        }
            


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





void deleteSVGimage(SVGimage *img){
  freeList(img -> paths);
    freeList(img -> circles);
    freeList(img -> rectangles);
    freeList(img -> groups);
    freeList(img -> otherAttributes);
    free(img);
}
//helper functions


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


Group* createGroupObject(){
    Group *group = malloc(sizeof(Group));
     
    group -> otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
      
group -> groups = initializeList(attributeToString, deleteGroup, compareAttributes);
    group -> rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
        
                group -> circles = initializeList(circleToString, deleteCircle, compareCircles);
                group -> paths = initializeList(pathToString, deletePath, comparePaths);

    return group;
}
void deleteGroup(void *data){
     if(data == NULL){
        return;
    }
    Group *group = (Group *) data;
    
   freeList(group -> otherAttributes);
    freeList(group -> groups);

    freeList(group -> rectangles);
    freeList(group -> circles);
    freeList(group -> paths);
    free(group);

}

ParsedValue *createValue(char *data){
    ParsedValue *value = malloc(sizeof(ParsedValue));
    char *temp;
    char *end;

        temp = calloc(1, strlen(data) + 1);
                strcpy(temp, data);
                value -> value = strtof(temp, &end);
                value -> unit = calloc(1, strlen(data) + 1);
                strcpy(value -> unit, end);
    
                free(temp);
    return value;
    
}

void deleteValue(ParsedValue *data){
    free(data ->unit);
    free(data);
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
    
    /*
    Attribute *newData = (Attribute *)data;
    char *newBuffer = (char *)calloc(2,sizeof(Attribute) + 10);
    strcat(newBuffer, newData->name);
    strcat(newBuffer, "=");
    strcat(newBuffer, newData->value);
    strcat(newBuffer, "\0");
    */

    return " ";
    
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
        char * value = NULL;
        value = calloc(1, strlen(this -> units) + 500);
    char *dummy = calloc(1, strlen(this -> units) + sizeof(data));

    sprintf(dummy, "%.1f",  this -> cx);
    strcat(value, "cx:\t");
    strcat(value, dummy);
   
    sprintf(dummy, "%.1f",  this -> cy);
    strcat(value, "\t\tcy:\t");
    strcat(value, dummy);

        sprintf(dummy, "%.1f",  this -> r);
        strcat(value, "\t\tr:\t");
        strcat(value, dummy);
    strcat(value, "\t\tunit:\t");

    strcpy(dummy, this -> units);
    strcat(value, "\notherAttributes:\t");

        strcat(value, dummy);
   
    

    
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

char *groupToString(void *data){
    return " ";
}
int compareGroups(const void *first, const void *second){
    return 0;
};



void validateNameSpace(char *namespace, SVGimage **list){
    SVGimage *temp_list = *list;
    if(namespace != NULL){
               
               if(strlen((char *)namespace) < 256){
                   strcpy(temp_list -> namespace, (char *) namespace);
               }
               else{
                   
                   long length =  strlen ((char *)namespace) - 256;
                   strncpy(temp_list -> namespace, (char *) namespace, 256 - length);
               
               }
           }
           else{
               strcpy(temp_list -> namespace, " ");

           }
}

void validateTitle(char *title, SVGimage **list){
    SVGimage *temp_list = *list;
    if(title != NULL){
               
               if(strlen((char *)title) < 256){
                   strcpy(temp_list -> title, (char *) title);
               }
               else{
                   
                   long length =  strlen ((char *)title) - 256;
                   strncpy(temp_list -> title, (char *) title, 256 - length);
               
               }
           }
           else{
               strcpy(temp_list -> title, " ");

           }
}

void validateDescription(char *description, SVGimage **list){
    SVGimage *temp_list = *list;
    if(description != NULL){
               
               if(strlen((char *)description) < 256){
                   strcpy(temp_list -> description, (char *) description);
               }
               else{
                   
                   long length =  strlen ((char *)description) - 256;
                   strncpy(temp_list -> description, (char *) description, 256 - length);
               
               }
           }
           else{
               strcpy(temp_list -> description, " ");

           }
}

int StartsWith(const char *a, const char *b)
{
    if (strncmp(a, b, strlen(b)) == 0)
        return 1;
    return 0;
}
