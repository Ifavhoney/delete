#include "SVGParser.h"
#include <stdlib.h>
#include <math.h>

typedef struct
{
    float value;
    char * unit;

} ParsedValue;
ParsedValue *createValue(char *data);
void deleteValue(ParsedValue *data);

char *printFunction(void *data);
void deleteFunction(void *data);
int compareFunction(const void *first, const void *second);
int StartsWith(const char *a, const char *b);
SVGimage* initializeObjects(void);
SVGimage *print_element_names(xmlNode *a_node, SVGimage **list);
Path* createPathObject(char *data);
List* recursiveRect(List *list, Group *group);
List* recursiveCircle(List *list, Group *group);
List* recursivePaths(List *list, Group *group);
List* recursiveGroups(List *list, Group *group);
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
char *printFunction(void *data){
    return "";
    
}
void deleteFunction(void *data){
}
int compareFunction(const void *first, const void *second){
    return 0;
}

int numAttr(SVGimage *img){
    int i = 0;
     List *list;
    if(img == NULL){
        return 0;
    }
    else{
    void *elem;
    list = getGroups(img);
      ListIterator iter = createIterator(list);
        while((elem = nextElement(&iter)) != NULL){
            Group *group = (Group *) elem;
            i+= group -> otherAttributes -> length;
        }
        freeList(list);
    
        list =  getRects(img);
         iter = createIterator(list);
              while((elem = nextElement(&iter)) != NULL){
                  Rectangle *rect = (Rectangle *) elem;
                i+= rect -> otherAttributes -> length;
              }
        freeList(list);
        
        list = getCircles(img);

        iter = createIterator(list);
                     while((elem = nextElement(&iter)) != NULL){
                         Circle *circle = (Circle *) elem;
                       i+= circle -> otherAttributes -> length;
                     }
        freeList(list);


        list = getPaths(img);

        iter = createIterator(list);
                                  while((elem = nextElement(&iter)) != NULL){
                                      Path *path = (Path *) elem;
                                    i+= path -> otherAttributes -> length;
                                  }
        freeList(list);

        
        
        i+= img -> otherAttributes -> length;
    }
    printf("%d", getLength(img -> otherAttributes));
    printf("Attrb %d", i);
    return i;
    
        
}
//gets num of groups with length
int numGroupsWithLen(SVGimage *img, int len){
    if(img == NULL){
        return 0;
    }
    int counter = 0;
    void *elem;
    ListIterator iter = createIterator(getGroups(img));
      while((elem = nextElement(&iter)) != NULL){
          int i = 0;
        Group *group = (Group *) elem;
          i+= group -> circles -> length;
          i+= group -> rectangles -> length;
          i+= group -> paths -> length;
          i+= group -> groups -> length;
          i+= group -> otherAttributes -> length;
          
          if(i == len){
              counter++;
          }
      }

    
    return counter;
}


int numRectsWithArea(SVGimage *img, float area){
    int num = 0;
    if(img == NULL){
    return 0;
    }
    else{
        void* elem;
        ListIterator iter = createIterator(getRects(img));
        while((elem = nextElement(&iter)) != NULL){
            Rectangle *rectangle = (Rectangle *) elem;
            float calc = rectangle -> height * rectangle -> width;
            if(ceil(calc) == ceil(area)){
                num++;
            }
        }
        
    }
    return num;
}
int numCirclesWithArea(SVGimage *img, float area){
   int num = 0;
    if(img == NULL){
    return 0;
    }
    else{
        void* elem;
        ListIterator iter = createIterator(getCircles(img));
        while((elem = nextElement(&iter)) != NULL){
            Circle *circle = (Circle *) elem;
            float calc = 3.14159265358979323846 * (circle -> r * circle -> r);
            if(ceil(calc) == ceil(area)){
                num++;
            }
        }
        
    }
    return num;
}
int numPathsWithdata(SVGimage *img, char *data){
    char *str;
    str = malloc(strlen(data) * 4);
    int num = 0;
       if(img == NULL){
       return 0;
       }
       else{
        
           void* elem;
           ListIterator iter = createIterator(getPaths(img));
           while((elem = nextElement(&iter)) != NULL){
               Path *path = (Path *) elem;
               strcpy(str,  path ->data);
               if(strcmp(str, data) == 0){
                   num++;
               }
           }
           
       }

       return num;
}


char* SVGimageToString(SVGimage* img){
    
    /*
    List* circleList = img -> circles;
    List* rectangleList = img -> rectangles;
    List* pathList = img -> paths;
    List* groupList = img -> groups;
    List* otherList = img ->otherAttributes;
    printf("Namespace: %s\n", img -> namespace  );
    printf("Title: %s\n", img -> title  );
    printf("Description: %s\n", img -> description  );
    printf("\nCircle: \n");
    char *circle = NULL;
    char *rectangle = NULL;
    char *toStrings = toString(img -> circles);
    circle = malloc(strlen(toStrings) + 2);
    strcpy(circle, toStrings );
    printf("%s", circle);
    free(circle);
    free(toStrings);
    rectangle = malloc(strlen(toStrings) + 2);
    toStrings = toString(img -> rectangles);
    strcpy(rectangle, toStrings);
    printf("%s", rectangle);
     */

    


    //freeing the value
    //freeing the memory
    /*
    char *value;
    while(rectangleList -> head != NULL){
      value = rectangleToString((Circle *)circleList -> head -> data);
      printf("%s", value);
      circleList -> head = circleList -> head -> next;
          free(value);
          free(circleList -> head);
      }
     */

    return " ";
    
}

List *getGroups(SVGimage *img){
     if(img == NULL){
       return 0;
   }

    List *list = initializeList(printFunction, deleteFunction, compareFunction);
    ListIterator iter2 = createIterator(img -> groups);
    void *elem2;
    while((elem2 = nextElement(&iter2)) != NULL){
        Group *group = (Group *) elem2;
                   
        //looks for groups that have rectangles
        insertBack(list, group);
        recursiveGroups(list, group);
        }

    return list;
    
}

List* recursiveGroups(List *list, Group *group){
    
  void * elem3;

    if(group -> groups != NULL){
           
           ListIterator iter3 = createIterator(group -> groups);
           //Our incrementer
           while((elem3 = nextElement(&iter3)) != NULL){
               Group *grp = (Group *) elem3;
               //Calls function & restarts
               insertBack(list, grp);
               list = recursiveGroups(list, grp);
           }
       }
    
    return list;
}


List *getPaths(SVGimage *img){
   
   if(img == NULL){
       return 0;
   }
    List *list = initializeList(printFunction, deleteFunction, compareFunction);
       ListIterator iter = createIterator(img -> paths);
       ListIterator iter2 = createIterator(img -> groups);

       
       void* elem;
       void* elem2;
       void* elem3;


       
       while((elem = nextElement(&iter)) != NULL){
           Path *paths = (Path *) elem;
           insertBack(list, paths);
       }
        
       
       while((elem2 = nextElement(&iter2)) != NULL){
           Group *group = (Group *) elem2;
       
           //if when we loop that group -> rectangles -> head != null
           if(group -> paths -> head != NULL){
               ListIterator iter3 = createIterator(group -> paths );
               //loop through that whole inner loop
               while((elem3 = nextElement(&iter3)) != NULL){
                   Path *path = (Path *) elem3;
                   insertBack(list, path);
                   
               
               }
           }
       
          while((elem2 = nextElement(&iter2)) != NULL){
                        Group *group = (Group *) elem2;
                        
                        //looks for groups that have rectangles
                       recursivePaths(list, group );
                    }
           
       }
      
     // printf("%s", )'
  //  printf("\nGetPaths length: %d\n", getLength(list));
       return list;
}

List* recursivePaths(List *list, Group *group){
    void * elem3;
    
    
    if(group -> rectangles != NULL){
          
        ListIterator iter3 = createIterator(group -> paths);
        
              while((elem3 = nextElement(&iter3)) != NULL){
              Path *rect = (Path *) elem3;
                  insertBack(list, rect);

          }
        }
    
    if(group -> groups != NULL){
        
        ListIterator iter3 = createIterator(group -> groups);
        //Our incrementer
        while((elem3 = nextElement(&iter3)) != NULL){
            Group *grp = (Group *) elem3;
            //Calls function & restarts
            list = recursivePaths(list, grp);
        }
    }
    
    return list;
  
}

List *getCircles(SVGimage *img){

        if(img == NULL){
               return 0 ;
           }
           List *list = initializeList(printFunction, deleteFunction, compareFunction);
           ListIterator iter = createIterator(img -> circles);
           ListIterator iter2 = createIterator(img -> groups);

           
           void* elem;
           void* elem2;
    
    
           //outside
           while((elem = nextElement(&iter)) != NULL){
               Circle *circle = (Circle *) elem;
               insertBack(list, circle);
           }
           //inner group
           
           while((elem2 = nextElement(&iter2)) != NULL){
               Group *group = (Group *) elem2;
               
               //looks for groups that have rectangles
              recursiveCircle(list, group );
           }
           
           
          // printf("\nGetCircle length: %d\n", getLength(list));
           return list;
    }

    

List* recursiveCircle(List *list, Group *group){
    void * elem3;
    
    
    if(group -> rectangles != NULL){
          
        ListIterator iter3 = createIterator(group -> circles);
        
              while((elem3 = nextElement(&iter3)) != NULL){
              Circle *rect = (Circle *) elem3;
                  insertBack(list, rect);

          }
        }
    
    if(group -> groups != NULL){
        
        ListIterator iter3 = createIterator(group -> groups);
        //Our incrementer
        while((elem3 = nextElement(&iter3)) != NULL){
            Group *grp = (Group *) elem3;
            //Calls function & restarts
            list = recursiveCircle(list, grp);
        }
    }
    
    return list;
  
}
 
 


List *getRects(SVGimage *img){
    if(img == NULL){
        return 0 ;
    }
    List *list = initializeList(printFunction, deleteFunction, compareFunction);
    ListIterator iter = createIterator(img -> rectangles);
    ListIterator iter2 = createIterator(img -> groups);

    
    void* elem;
    void* elem2;

    
    //outside
    while((elem = nextElement(&iter)) != NULL){
        Rectangle *rect = (Rectangle *) elem;
        insertBack(list, rect);
    }
    //inner group
    
    while((elem2 = nextElement(&iter2)) != NULL){
        Group *group = (Group *) elem2;
        //looks for groups that have rectangles
        recursiveRect(list, group );
    }
    return list;
}

List* recursiveRect(List *list, Group *group){
    void * elem3;
    
    
    if(group -> rectangles != NULL){
          
        ListIterator iter3 = createIterator(group -> rectangles);
        
              while((elem3 = nextElement(&iter3)) != NULL){
              Rectangle *rect = (Rectangle *) elem3;
                  insertBack(list, rect);
                  printf("hi\t");

          }
        }
    
    if(group -> groups != NULL){
        
        ListIterator iter3 = createIterator(group -> groups);
        //Our incrementer
        while((elem3 = nextElement(&iter3)) != NULL){
            Group *grp = (Group *) elem3;
            //Calls function & restarts
            list = recursiveRect(list, grp);
        }
    }
    //Base case
    return list;
  
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
            return NULL;
        }
        else{
            
            
         /*Get the root element node */
        root_element = xmlDocGetRootElement(doc);
        print_element_names(root_element, &list);
            validateNameSpace((char *)root_element -> ns -> href, &list);
         
  List *listc = getCircles(list);
            freeList(listc);
            /*
        
        printf("GetRects %d\n", getLength(listr));

            List *listc = getCircles(list);
            
            List *listp = getPaths(list);
            List *listg = getGroups(list);
            
            printf("\n\n%d", numGroupsWithLen(list, 2));
        freeList(listr);
            freeList(listc);
            freeList(listp);
            freeList(listg);
             */
            
            /*
            
            Group *group = list -> groups -> head -> data;
            Group *grp2 = group -> groups -> head -> data;
            List *grp3 = grp2 -> groups;
           printf("%d", getLength(grp3));
        */
          
            /*
            
            Group *group = list -> groups -> head -> data;
            Group *grp2 = group -> groups -> head -> data;
            List *grp3 = grp2 -> groups;

            printf("%d", getLength(grp3));

          */
            
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
            insertOtherAttribute(tempList, cur_node);
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
            /*
            if(strcmp((char *)cur_node -> name, "g") != 0 && strcmp((char *)cur_node -> name, "circle") != 0 && strcmp((char *)cur_node -> name, "path") != 0 && strcmp((char *)cur_node -> name, "rect") != 0  && strcmp((char *)cur_node -> name, "text") != 0  && strcmp((char *)cur_node -> name, "comment") != 0 && strcmp((char *)cur_node -> name, "desc") != 0 && strcmp((char *)cur_node -> name, "title") != 0     ){
                insertOtherAttribute(tempList, cur_node);               // printf("%s", attribute ->value);
            }
             */
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

    for (attr = cur_node->properties; attr != NULL; attr = attr->next)
      {
          xmlNode *snapshot = attr->children;
          char *getAttrValue = (char *)snapshot->content;
          char *getAttrName = (char *)attr->name;

          if(getAttrValue != NULL && getAttrName != NULL  ){

              Attribute *attribute = createAttribute(getAttrName, getAttrValue);
              //printf("attrn: %s", getAttrName);
              insertBack(list -> otherAttributes , attribute);
          }
            
      }
}
/*
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
 }
 */
            
            
    
    
    
    
    



void insertGroup(void *data, xmlNode *cur_node, int version){
     int i = 0;
    //printf("%s");
    xmlAttr *attr;
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
                    for (attr = cur_node->properties; attr != NULL; attr = attr->next)
                    {
                        if(version == 0){
                          //  printf("comes here");
                            SVGimage *list = (SVGimage *)data;
                            Attribute *attribute = createAttribute(getAttrName, getAttrValue);
                                insertBack(list -> otherAttributes , attribute);
                        }
                        if(version == 1){
                            Group *list = (Group *)data;
                            Attribute *attribute = createAttribute(getAttrName, getAttrValue);
                            insertBack(list -> otherAttributes , attribute);
                        }
                    }
                      

                     
                      
                      
                    //9
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
                                //Calls recursion - goes to the nexr group
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
    if(img == NULL || img -> paths == NULL || img -> rectangles == NULL || img -> groups == NULL || img -> otherAttributes == NULL){
        return;
    }
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
        value = calloc(1,  500);
    char *dummy = calloc(1, 500);
    sprintf(dummy, "%.1f",  this -> cx);
    strcat(value, "\ncx:\t");
    strcat(value, dummy);
   
    sprintf(dummy, "%.1f",  this -> cy);
    strcat(value, "\t\tcy:\t");
    strcat(value, dummy);

        sprintf(dummy, "%.1f",  this -> r);
        strcat(value, "\t\tr:\t");
        strcat(value, dummy);
    strcat(value, "\t\tunit:\t");

    strcpy(dummy, this -> units);
    strcat(value, "\notherAttributes:");

        strcat(value, dummy);
    while (this -> otherAttributes -> head != NULL) {
        Attribute *attribute = (Attribute *) this -> otherAttributes -> head -> data;
        strcat(value, attribute -> name);
        strcat(value, ":\t");
        strcat(value, attribute -> value);
        strcat(value, "\t");


        this -> otherAttributes -> head = this -> otherAttributes -> head -> next;
    }
    strcat(value, "\n");

    
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
char *rectangleToString(void *data){
    
    if(data == NULL){
        return " ";
    }
    
    Rectangle *this = data;
          char * value = NULL;
          value = calloc(1,  500);
      char *dummy = calloc(1, 500);
   
    sprintf(dummy, "%.1f",  this -> x);
      strcat(value, "\nx:\t");
      strcat(value, dummy);
     
      sprintf(dummy, "%.1f",  this -> y);
      strcat(value, "\t\ty:\t");
      strcat(value, dummy);
    
  

          sprintf(dummy, "%.1f",  this -> width );
          strcat(value, "\t\twidth:\t");
          strcat(value, dummy);
    
    sprintf(dummy, "%.1f",  this -> height );
    strcat(value, "\t\theight:\t");
    strcat(value, dummy);
    
    
      strcat(value, "\notherAttributes:");

          strcat(value, dummy);
      while (this -> otherAttributes -> head != NULL) {
          Attribute *attribute = (Attribute *) this -> otherAttributes -> head -> data;
          strcat(value, attribute -> name);
          strcat(value, ":\t");
          strcat(value, attribute -> value);
          strcat(value, "\t");


          this -> otherAttributes -> head = this -> otherAttributes -> head -> next;
      }
      strcat(value, "\n");

      free(dummy);
    return value;
    
    
}
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
