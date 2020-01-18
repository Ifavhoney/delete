#include "SVGParser.h"

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

SVGimage *createSVGimage(char *fileName)
{

    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    LIBXML_TEST_VERSION

    return NULL;

    //parses & check if null

    //Returns the pointer of type SVGimage containing all data
}
