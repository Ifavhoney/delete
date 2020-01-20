#include "SVGParser.h"
#include <stdlib.h>
//ATTRIBUTES DONT
/** Function to create an SVG object based on the contents of an SVG file.
 *@pre File name cannot be an empty string or NULL.
       File represented by this name must exist and must be readable.
 *@post Either:
        A valid SVGimage has been created and its address was returned
        or
        An error occurred, and NULL was returned
 *@return the pointer to the new struct or NULL
 *@param fileName - a string containing the name of the SVG file
**/
int StartsWith(const char *a, const char *b);
SVGimage *print_element_names(xmlNode *a_node, SVGimage **list);
void insertPath(SVGimage *tempList, xmlNode *cur_node, Attribute *tempData, char *storeAttribute);
void insertCircle(SVGimage *tempList, xmlNode *cur_node, Attribute *tempData, char *storeAttribute);
void insertGroup(SVGimage *tempList, xmlNode *cur_node, Attribute *tempData, char *storeAttribute);

char *printFunction(void *);
char *deleteFunction(void *);
int compareFunction(const void *, const void *);

SVGimage *createSVGimage(char *fileName)
{

    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    SVGimage *list = NULL;
    List *genericList = initializeList(printFunction, deleteFunction, compareFunction);

    //initialize
    list = malloc(sizeof(SVGimage));
    strcpy(list->namespace, (char *)XML_XML_NAMESPACE);
    list->paths = genericList;
    list->circles = genericList;
    list->rectangles = genericList;
    list->groups = genericList;

    LIBXML_TEST_VERSION

    /*parse the file and get the DOM */
    doc = xmlReadFile(fileName, NULL, 0);

    if (doc == NULL)
    {
        printf("error: could not parse file %s\n", fileName);
    }
    else
    {

        /*Get the root element node */
        root_element = xmlDocGetRootElement(doc);
        //root_element = xmlDoc
        print_element_names(root_element, &list);
        printf("%s\n", list->namespace);

        //printf("\n%s", list->circles -> head -> data);
    }

    return NULL;

    //parses & check if null

    //Returns the pointer of type SVGimage containing all data
}

SVGimage *print_element_names(xmlNode *a_node, SVGimage **list)
{
    Attribute *tempData = NULL;
    SVGimage *tempList = *list;
    char *storeAttribute = " ";
    char *storePrevname = NULL;
    xmlNode *cur_node = NULL;
    int i = 0;

    //Iterates through xmlNode given by root_element, as you can see by its name
    for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next)
    {
        i++;
        if (cur_node->type == XML_ELEMENT_NODE)
        {

            // printf("i: %d node type: Element, name: %s\n", i, cur_node->name);
            if (strcmp((char *)cur_node->name, "title") == 0)
            {
                storePrevname = (char *)cur_node->name;
                //  printf("i:%d content: %s \n", i, cur_node -> content);
            }
        }
        if (cur_node->content != NULL)
        {
            //  printf("i:%d content: %s \n", i, cur_node -> content);
        }

        /*Uncomment the code below if you want to see the content of every node.
        
    
        */

        /*
           
        if(strcmp((char *)cur_node->name, "circle") == 0){

        insertCircle(tempList, cur_node, tempData, storeAttribute);
        }
         */

        if (strcmp((char *)cur_node->name, "g") == 0)
        {

            printf("i: %d node type: Element, name: %s\n", i, cur_node->name);

            insertGroup(tempList, cur_node, tempData, storeAttribute);
        }

        /*
        
        if (strcmp((char *)cur_node->name, "path") == 0)
        {
            insertPath(tempList, cur_node, tempData, storeAttribute);
        }
   */

        //increments by children
        print_element_names(cur_node->children, &tempList);
    }

    return tempList;
}
void insertGroup(SVGimage *tempList, xmlNode *cur_node, Attribute *tempData, char *storeAttribute)
{
    //Initialize Our tempData - we'll be reusing this memory, modifying the data
    tempData = malloc(sizeof(Attribute));
    char *attribute;

    /*
         1. Counter is for debugging purpose,
         2. Verifying when the next value is null
         3. Storing data
         */
    int i = 0;

    List *otherAttributes = initializeList(printFunction, deleteFunction, compareFunction);
    //Gets the attributes (e.g (atrbName) fill = (atrbContent)"#fff")
    xmlAttr *attr;
    for (attr = cur_node->properties; attr != NULL; attr = attr->next)
    {
        i++;

        //Ask about this
        xmlNode *snapshot = attr->children;

        char *getAttrValue = (char *)snapshot->content;
        char *getAttrName = (char *)attr->name;
        if (strcmp((char *)cur_node->name, "g") == 0)
        {

            //Initialize Our tempData
            tempData->name = getAttrName;
            tempData->value = getAttrValue;
            //Properties with 1 attributeName must be returned immediately

            if (attr->next == NULL & i == 1)
            {

                Group *group = malloc(sizeof(Group));
                insertBack(otherAttributes, attributeToString(tempData));
                group->otherAttributes = otherAttributes;
                //    printf("%s", xmlNextElementSibling(cur_node -> children) -> name);
                long nodeCounter = xmlChildElementCount(cur_node);
                xmlNode *temp_cur_node = cur_node->children;

                while (nodeCounter != 0)
                {
                    nodeCounter--;
                    const char *validateName = (const char *)xmlNextElementSibling(temp_cur_node)->name;
                    if (strcmp(validateName, "circle"))
                    {
                        for (xmlAttr *val = temp_cur_node->properties; val != NULL; val = attr->next)
                        {
                            printf("??");
                        }
                    }
                    else if (strcmp(validateName, "path"))
                    {
                    }
                    else if (strcmp(validateName, "rect"))
                    {
                    }
                    else
                    {
                        //oa
                    }
                }
                continue;
            }
            //Proceed by storing the first Attribute
            if (i == 1)
            {
                //MALLOC ERROR
                storeAttribute = attributeToString(tempData);

                continue;
            }
            //Concatenate attributes two, three, four etc
            attribute = attributeToString(tempData);
            storeAttribute = strcat((char *)storeAttribute, (char *)attribute);
        }
        //When completed insert Paths sequentially

        /*
            if (attr->next == NULL && strcmp((char *)cur_node->name, "svg") != 0)
            {
                printf("End at %d\n", i);
                i = 0;
                printf("%s\n", storeAttribute);
                Circle *circle = malloc(sizeof(circle));
                char *pch = strtok(storeAttribute, ">");
                //Consider putting it above
                while (pch != NULL)
                {
                    pch = strtok(NULL, ">");
                }
           
             //   insertBack(tempList->circles, circle);

            }
             */
    }
    free(tempData);
}

//helper functions
//helper functions
void insertCircle(SVGimage *tempList, xmlNode *cur_node, Attribute *tempData, char *storeAttribute)
{
    //Initialize Our tempData - we'll be reusing this memory, modifying the data
    tempData = malloc(sizeof(Attribute));
    char *attribute;

    /*
     1. Counter is for debugging purpose,
     2. Verifying when the next value is null
     3. Storing data
     */
    int i = 0;

    List *otherAttributes = initializeList(printFunction, deleteFunction, compareFunction);
    printf("%d", otherAttributes->length);
    //Gets the attributes (e.g (atrbName) fill = (atrbContent)"#fff")
    xmlAttr *attr;
    for (attr = cur_node->properties; attr != NULL; attr = attr->next)
    {
        i++;

        //Ask about this
        xmlNode *snapshot = attr->children;

        char *getAttrValue = (char *)snapshot->content;
        char *getAttrName = (char *)attr->name;

        if (strcmp((char *)cur_node->name, "circle") == 0)
        {
            //Initialize Our tempData
            tempData->name = getAttrName;
            tempData->value = getAttrValue;
            //Properties with 1 attributeName must be returned immediately
            if (attr->next == NULL & i == 1)
            {

                printf("zaza");
                //changed from tempCircles to tempList - consider changing tempData to type circles
                insertBack(tempList->circles, tempData);
                continue;
            }
            //Proceed by storing the first Attribute
            if (i == 1)
            {
                //MALLOC ERROR
                storeAttribute = attributeToString(tempData);

                continue;
            }
            //Concatenate attributes two, three, four etc
            attribute = attributeToString(tempData);
            storeAttribute = strcat((char *)storeAttribute, (char *)attribute);
        }
        //When completed insert Paths sequentially

        if (attr->next == NULL && strcmp((char *)cur_node->name, "svg") != 0)
        {

            printf("End at %d\n", i);
            i = 0;
            printf("%s\n", storeAttribute);
            Circle *circle = malloc(sizeof(circle));
            char *pch = strtok(storeAttribute, ">");
            //Consider putting it above
            while (pch != NULL)
            {
                if (strstr("cx", pch) == 0)
                {

                    if (StartsWith(pch, "cx"))
                    {
                        pch++;
                        pch++;
                        pch++;

                        circle->cx = atof((char *)pch);
                    }
                    else if (StartsWith(pch, "cy"))
                    {
                        pch++;
                        pch++;
                        pch++;

                        circle->cy = atof((char *)pch);
                    }
                    else if (StartsWith(pch, "r"))
                    {
                        pch++;
                        pch++;

                        circle->r = atof((char *)pch);
                    }
                    else
                    {
                        insertBack(otherAttributes, pch);
                        //other attribute
                    }

                    //RX, RY, R=
                }

                pch = strtok(NULL, ">");
            }

            if (otherAttributes->length == 0)
            {
                insertBack(otherAttributes, " ");
            }
            circle->otherAttributes = otherAttributes;

            printf("cx: %f, cy: %f, r: %f, oa: %s ", circle->cx, circle->cy, circle->r, circle->otherAttributes->head->data);

            insertBack(tempList->circles, circle);

            /*

*/
        }
    }

    free(tempData);
}
//Need to implenet other atributes (e.g fill)
void insertPath(SVGimage *tempList, xmlNode *cur_node, Attribute *tempData, char *storeAttribute)
{

    List *otherAttributes = initializeList(printFunction, deleteFunction, compareFunction);
    char *attribute = NULL;
    //Initialize Our tempData - we'll be reusing this memory, modifying the data
    tempData = malloc(sizeof(Attribute));
    /*
     1. Counter is for debugging purpose,
     2. Verifying when the next value is null
     3. Storing data
     */

    int i = 0;
    //Gets the attributes (e.g (atrbName) fill = (atrbContent)"#fff")
    xmlAttr *attr;
    for (attr = cur_node->properties; attr != NULL; attr = attr->next)
    {
        i++;

        //Ask about this
        xmlNode *snapshot = attr->children;

        char *getAttrValue = (char *)snapshot->content;
        char *getAttrName = (char *)attr->name;

        if (strcmp((char *)cur_node->name, "path") == 0)
        {
            //Initialize Our tempData
            tempData->name = getAttrName;
            tempData->value = getAttrValue;
            //Properties with 1 attributeName must be returned immediately
            if (attr->next == NULL & i == 1)
            {
                printf("!!zaza!!");
                insertBack(tempList->paths, tempData);
                continue;
            }
            //Proceed by storing the first Attribute
            if (i == 1)
            {
                //MALLOC ERROR
                storeAttribute = attributeToString(tempData);

                continue;
            }
            //Concatenate attributes two, three, four etc
            attribute = attributeToString(tempData);
            storeAttribute = strcat(storeAttribute, attribute);
        }
        //When completed insert Paths sequentially
        if (attr->next == NULL && strcmp((char *)cur_node->name, "svg") != 0)
        {

            //Seperate attributes
            Path *path = malloc(sizeof(path));
            char *pch = strtok(storeAttribute, ">");
            while (pch != NULL)
            {
                if (!StartsWith(pch, "d"))
                {
                    insertBack(otherAttributes, pch);
                }
                else
                {
                    path->data = pch;
                }

                pch = strtok(NULL, ">");
            }
            if (otherAttributes->length == 0)
            {
                insertBack(otherAttributes, " ");
            }
            i = 0;
            path->otherAttributes = otherAttributes;
            insertBack(tempList->paths, path);

            printf("End at %d\n", i);
        }
    }
    free(attribute);

    free(tempData);
}

int StartsWith(const char *a, const char *b)
{
    if (strncmp(a, b, strlen(b)) == 0)
        return 1;
    return 0;
}

char *attributeToString(void *data)
{

    Attribute *newData = (Attribute *)data;
    char *newBuffer = (char *)malloc(sizeof(data));
    strcat(newBuffer, newData->name);
    strcat(newBuffer, "=");

    strcat(newBuffer, newData->value);
    strcat(newBuffer, ">");

    return newBuffer;
}

int compareFunction(const void *fun1, const void *fun2)
{
    return 0;
}
char *printFunction(void *fun)
{
    return " ";
}
char *deleteFunction(void *fun1)
{
    return "";
}
