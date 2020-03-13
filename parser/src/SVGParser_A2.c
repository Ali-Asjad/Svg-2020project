/*
Name: Ananya Thukral
Student ID: 1027782
Date: 27/01/2020
Assignment: CIS*2750 A1
Site/reference: http://www.xmlsoft.org/examples/tree1.c
Site/reference: http://www.xmlsoft.org/html/libxml-parser.html
*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <libxml/xmlschemas.h>
#include <stdio.h>
#include <math.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "SVGParser.h"
#include "LinkedListAPI.h"

#define  rectOffset     0
#define  circleOffset   1
#define  pathOffset     2
#define  groupOffset    3

#define M_PI 3.14159265358979323846

Rectangle* getRectangle(xmlNode * rect);
Circle* getCircle(xmlNode *circ);
Path* getPath(xmlNode *p);
Group* getGroup(xmlNode *gp);

const char *rectangleString = "Rectangle\n\r"
"\tX      : %8.2f %s\n"
"\tY      : %8.2f %s\n"
"\tWidth  : %8.2f %s\n"
"\tHeight : %8.2f %s\n"
"\t%s";
const char *circleString = "Circle\n"
"\tCX     : %8.2f %s\n"
"\tCY     : %8.2f %s\n"
"\tR      : %8.2f %s\n"
"\t%s";
const char *pathString = "Path\n"
"\tData   : %s\n"
"\t%s\n";

const char *groupString = "Group Start\n";

const char *imageString = "NameSpace  : %s\n"
"Title      : %s\n"
"Description: %s\n";
/**
* Function Allocates the new memory and copy the source string to new memory location and
* can be released later using free()
* input   : address of the Source String to be copied
* returns : address of the copied string
* pre     : Source String should not be a NULL
**/
char *strdup(const char *src) {
	if (src == NULL)
		return NULL;

	int length = strlen(src) + 1;
	char *dest = (char *)malloc(length);
	if (dest == NULL) {
		printf("failed to allocate the memory\n\r");
		return NULL;
	}
	strcpy(dest, src);
	return dest;
}

/**
* Function frees the resources consumed by the Attribute Structure
* input   : pointer to the structure
* returns : none
* pre     : input pointer shouldn't be NULL
**/
void deleteAttribute(void* data) {
	if (data == NULL)
		return;

	Attribute *attribute = data;
	free(attribute->name);
	free(attribute->value);
	free(attribute);
}

/**
* Functions Converts the Data Object of Attribute Structure to a String
* input   : pointer to the Structure
* returns : address of the String
* pre     : input pointer shouldn't be NULL
**/
char* attributeToString(void* data) {
	if (data == NULL)
		return NULL;

	Attribute *attribute = data;
	int length = strlen(attribute->name) + strlen(attribute->value) + 5; // 5 - newLine, space and NULL character
	char *string = (char *)malloc(length);
	//     " Attribute : Value "   
	sprintf(string, "%s : %s\n", attribute->name, attribute->value);
	return string;
}

/**
* compare functions always return ZERO
**/
int compareAttributes(const void *first, const void *second) {
	return 0;
}

/**
* Function frees the resources consumed by the Rectangle Structure
* input   : pointer to the structure
* returns : none
* pre     : input pointer shouldn't be NULL
**/
void deleteRectangle(void* data) {
	if (data == NULL)
		return;
	Rectangle *rectangle = data;
	freeList(rectangle->otherAttributes);
	free(data);
}

/**
* Functions Converts the Data Object of Rectangle Structure to a String
* input   : pointer to the Structure
* returns : address of the String
* pre     : input pointer shouldn't be NULL
**/
char* rectangleToString(void* data) {
	if (data == NULL)
		return NULL;

	Rectangle *rectangle = data;
	char *str = toString(rectangle->otherAttributes); // Attribute Object to String
	char string[1024];              // temparory Memory 
  /*  "Rectangle\n\r"
	  "\tX      : %8.2f %s\n"
	  "\tY      : %8.2f %s\n"
	  "\tWidth  : %8.2f %s\n"
	  "\tHeight : %8.2f %s\n"
	  "\t%s"; */
	sprintf(string, rectangleString, rectangle->x, rectangle->units, rectangle->y, rectangle->units,
		rectangle->width, rectangle->units, rectangle->height, rectangle->units, str);
	free(str);                      // free the memory allocated for Attribute.toString()
	return strdup(string);          // allocate a new memory and return the address of newly allocated location
}

/**
* compare functions always return ZERO
**/
int compareRectangles(const void *first, const void *second) {
	return 0;
}

/**
* Function frees the resources consumed by the Circle Structure
* input   : pointer to the structure
* returns : none
* pre     : input pointer shouldn't be NULL
**/
void deleteCircle(void* data) {
	if (data == NULL)
		return;

	Circle *circle = data;
	freeList(circle->otherAttributes);
	free(data);
}

/**
* Functions Converts the Data Object of Circle Structure to a String
* input   : pointer to the Structure
* returns : address of the String
* pre     : input pointer shouldn't be NULL
**/
char* circleToString(void* data) {
	if (data == NULL)
		return NULL;

	Circle *circle = data;
	char *str = toString(circle->otherAttributes); // Attribute Object to String
	char string[1024];              // temparory Memory
  /*    "Circle\n"
		"\tCX     : %8.2f %s\n"
		"\tCY     : %8.2f %s\n"
		"\tR      : %8.2f %s\n"
		"\t%s"; */
	sprintf(string, circleString, circle->cx, circle->units, circle->cy,
		circle->units, circle->r, circle->units, str);
	free(str);                      // free the memory allocated for Attribute.toString()
	return strdup(string);          // allocate a new memory and return the address of newly allocated location
}

/**
* compare functions always return ZERO
**/
int compareCircles(const void *first, const void *second) {
	return 0;
}

/**
* Function frees the resources consumed by the Path Structure
* input   : pointer to the structure
* returns : none
* pre     : input pointer shouldn't be NULL
**/
void deletePath(void* data) {
	if (data == NULL)
		return;

	Path *path = data;
	free(path->data);
	freeList(path->otherAttributes);
	free(path);
}

/**
* Functions Converts the Data Object of Path Structure to a String
* input   : pointer to the Structure
* returns : address of the String
* pre     : input pointer shouldn't be NULL
**/
char* pathToString(void* data) {
	if (data == NULL)
		return NULL;

	Path *path = data;
	char *str = toString(path->otherAttributes); // Attribute Object to String
	char string[1024];              // temparory Memory
  /*    "Path\n"
		"\tData   : %s\n"
		"\t%s\n";*/
	sprintf(string, pathString, path->data, str);
	free(str);                      // free the memory allocated for Attribute.toString()
	return strdup(string);          // allocate a new memory and return the address of newly allocated location
}

/**
* compare functions always return ZERO
**/
int comparePaths(const void *first, const void *second) {
	return 0;
}

/**
* Function frees the resources consumed by the Group Structure
* input   : pointer to the structure
* returns : none
* pre     : input pointer shouldn't be NULL
**/
void deleteGroup(void* data) {
	if (data == NULL)
		return;

	Group *group = data;
	freeList(group->rectangles);
	freeList(group->circles);
	freeList(group->paths);
	freeList(group->groups);
	freeList(group->otherAttributes);
	free(group);
}

/**
* Functions Converts the Data Object of Group Structure to a String
* input   : pointer to the Structure
* returns : address of the String
* pre     : input pointer shouldn't be NULL
**/
char* groupToString(void* data) {
	if (data == NULL)
		return NULL;

	Group *group = data;
	char *rectangles = toString(group->rectangles); // Rectangle Object to String
	char *circles = toString(group->circles);    // Circle Object to String
	char *paths = toString(group->paths);      // Path Object to String
	char *groups = toString(group->groups);     // Group Object to String
	char *attr = toString(group->otherAttributes);// Attribute Object to String

	int length = strlen(groupString) + strlen(rectangles) + strlen(circles) +
		strlen(paths) + strlen(groups) + strlen(attr) + 16;
	char *string = (char *)malloc(length);      // new buffer for string

	strcpy(string, groupString);                    // copy all the strings
	strcat(string, rectangles);
	strcat(string, circles);
	strcat(string, paths);
	strcat(string, groups);
	strcat(string, attr);
	strcat(string, "Group End\n");

	free(rectangles);                               // free the allocated Strings
	free(circles);
	free(paths);
	free(groups);
	free(attr);

	return string;                                  // address of the new copied String buffer
}

/**
* compare functions always return ZERO
**/
int compareGroups(const void *first, const void *second) {
	return 0;
}

/**
* Function that returns List of the Groups and their Children, it uses Recursive function to go into the hierachy
* input  : pointer to the xmlNode usually the element with <g> .... </g>
* return : return the pointer to List of the Group
*/
Group* getGroup(xmlNode *gp)
{
	Group *group;
	// Allocating the memory
	group = (Group *)malloc(sizeof(Group));
	if (group == NULL) {
		printf("Error in Allocating Group memory\n\r");
		return NULL;
	}
	memset(group, 0, sizeof(Group));

	// initialize the List required for Group Structure
	group->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
	group->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
	group->paths = initializeList(&pathToString, &deletePath, &comparePaths);
	group->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);
	group->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

	// loop through the Children Element and add then to the respective function
	xmlNode *cur_node = NULL;

	for (cur_node = gp->children; cur_node != NULL; cur_node = cur_node->next) {
		if (!xmlStrcmp(cur_node->name, (const xmlChar *)"rect")) {
			insertBack(group->rectangles, getRectangle(cur_node));
		}
		else if (!xmlStrcmp(cur_node->name, (const xmlChar *)"circle")) {
			insertBack(group->circles, getCircle(cur_node));
		}
		else if (!xmlStrcmp(cur_node->name, (const xmlChar *)"path")) {
			insertBack(group->paths, getPath(cur_node));
		}
		else if (!xmlStrcmp(cur_node->name, (const xmlChar *)"g")) {
			insertBack(group->groups, getGroup(cur_node));  // Recursive Call
		}
	}

	// loop through all the Attributes and add to the otherAttributes List
	xmlAttr *attr;
	for (attr = gp->properties; attr != NULL; attr = attr->next) {
		xmlNode *value = attr->children;
		Attribute *attribute = (Attribute *)malloc(sizeof(Attribute));
		attribute->name = strdup((const char *)attr->name);
		attribute->value = strdup((const char *)value->content);
		insertBack(group->otherAttributes, attribute);
	}

	return group;
}

/**
* Function that returns List of the Rectangle
* input  : pointer to the xmlNode usually the element with <rect> .... </rect>
* return : return the pointer to List of the Rectangle
*/
Rectangle* getRectangle(xmlNode *rect)
{
	Rectangle *rects;
	// Allocating the memory
	rects = (Rectangle *)malloc(sizeof(Rectangle));
	if (rects == NULL) {
		printf("Error in Allocating Rectangle memory\n\r");
		return NULL;
	}
	memset(rects, 0, sizeof(Rectangle));
	// initialize the List required for otherAttributes Structure
	rects->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

	// loop through all the Attributes and add to the otherAttributes List
	xmlAttr *attr;
	for (attr = rect->properties; attr != NULL; attr = attr->next) {
		xmlNode *value = attr->children;
		char *attrName = (char *)attr->name;
		char *ptr = NULL;
		if (!xmlStrcmp((const xmlChar *)attrName, (const xmlChar *)"x")) {
			rects->x = strtof((const char *)(value->content), &ptr);
			strncpy(rects->units, ptr, sizeof(rects->units) - 1);
		}
		else if (!xmlStrcmp((const xmlChar *)attrName, (const xmlChar *)"y")) {
			rects->y = strtof((const char *)(value->content), NULL);
		}
		else if (!xmlStrcmp((const xmlChar *)attrName, (const xmlChar *)"width")) {
			rects->width = strtof((const char *)(value->content), NULL);
		}
		else if (!xmlStrcmp((const xmlChar *)attrName, (const xmlChar *)"height")) {
			rects->height = strtof((const char *)(value->content), NULL);
		}
		else {
			Attribute *attribute = (Attribute *)malloc(sizeof(Attribute));
			attribute->name = strdup((const char *)attr->name);
			attribute->value = strdup((const char *)value->content);
			insertBack(rects->otherAttributes, attribute);
		}
	}
	return rects;
}

/**
* Function that returns List of the Circle
* input  : pointer to the xmlNode usually the element with <circle> .... </circle>
* return : return the pointer to List of the Circle
*/
Circle* getCircle(xmlNode *circ)
{
	Circle *circle;
	// Allocating the memory
	circle = (Circle *)malloc(sizeof(Circle));
	if (circle == NULL) {
		printf("Error in Allocating Circle memory\n\r");
		return NULL;
	}
	memset(circle, 0, sizeof(Circle));
	// initialize the List required for otherAttributes Structure
	circle->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
	// loop through all the Attributes and add to the otherAttributes List
	xmlAttr *attr;
	for (attr = circ->properties; attr != NULL; attr = attr->next) {
		xmlNode *value = attr->children;
		char *attrName = (char *)attr->name;
		char *ptr = NULL;
		if (!xmlStrcmp((const xmlChar *)attrName, (const xmlChar *)"cx")) {
			circle->cx = strtof((const char *)(value->content), &ptr);
			strncpy(circle->units, ptr, sizeof(circle->units) - 1);
		}
		else if (!xmlStrcmp((const xmlChar *)attrName, (const xmlChar *)"cy")) {
			circle->cy = strtof((const char *)(value->content), NULL);
		}
		else if (!xmlStrcmp((const xmlChar *)attrName, (const xmlChar *)"r")) {
			circle->r = strtof((const char *)(value->content), NULL);
		}
		else {
			Attribute *attribute = (Attribute *)malloc(sizeof(Attribute));
			attribute->name = strdup((const char *)attr->name);
			attribute->value = strdup((const char *)value->content);
			insertBack(circle->otherAttributes, attribute);
		}
	}
	return circle;
}

/**
* Function that returns List of the Path
* input  : pointer to the xmlNode usually the element with <path> .... </path>
* return : return the pointer to List of the Path
*/
Path* getPath(xmlNode *p)
{
	Path *path;
	// Allocating the memory
	path = (Path *)malloc(sizeof(Path));
	if (path == NULL) {
		printf("Error in Allocating Path memory\n\r");
		return NULL;
	}
	memset(path, 0, sizeof(Path));
	// initialize the List required for otherAttributes Structure
	path->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
	// loop through all the Attributes and add to the otherAttributes List
	xmlAttr *attr;
	for (attr = p->properties; attr != NULL; attr = attr->next) {
		xmlNode *value = attr->children;
		char *attrName = (char *)attr->name;

		if (!xmlStrcmp((const xmlChar *)attrName, (const xmlChar *)"d")) {
			path->data = strdup((const char *)value->content);
		}
		else {
			Attribute *attribute = (Attribute *)malloc(sizeof(Attribute));
			attribute->name = strdup((const char *)attr->name);
			attribute->value = strdup((const char *)value->content);
			insertBack(path->otherAttributes, attribute);
		}
	}
	return path;
}

/**
* Dummy Function Since the getRects, getCircles, getPaths, getGroups function create a list for all the
* primitives in a seperate list for each primitives. This List contains only the pointer to the respective primitives
* not a copy, so when clearing List with respective primitive deleteFunc it release the memory in those pointer
* which is still in use , so this delete function will be used to initialize the LIST, which do nothing when deleteFunc
* is called from the LinkedListAPI.c, but the freeList wont deallocate the primitives but only the LIST structure
* initialized at the starting
**/
void delete(void *data) {
	(void)data;
	return;
}

/**
* Function search through the group LIST and add the primitive selected by offset to the new LIST
* this is a recursive function which call itself which loops through all the group hierachy
* in Group Structure each member is a LIST pointer offset from each other by sizeof(List*)
* input  srcList : pointer to Group List to be searched
		 destList: pointer to primitive List to add the primitive
		 offset  : selected primitive Rectangle, circle, path, group
* returns        : none
**/
void getGroupElement(List *srcList, List *destList, int offset) {
	if (getFromFront(srcList) == NULL)
		return;         // end of the Group

	ListIterator iter = createIterator(srcList);
	Group *gp = NULL;

	while ((gp = nextElement(&iter)) != NULL) {
		// gp address of the group, cast it to integer then add the offset,
		// 0 * sizeof(List *) for Rectangle, 1 * sizeof(List *) for Circle and so on.
		// Now result will be address of the pointer of the primitive LIST, which is double pointer
		// so the result is cast back into LIST**, then dereference the double pointer by one we get the address of the 
		// respective primitive LIST  
		//ListIterator circleIter = createIterator(*(List **)((uintptr_t)gp + offset * sizeof(List *)));
		ListIterator circleIter = createIterator(*(List **)((void*)gp + offset * sizeof(List *)));
		void *element = NULL;
		while ((element = nextElement(&circleIter)) != NULL) {
			insertBack(destList, element);
		}
		getGroupElement(gp->groups, destList, offset);  // recursive call
	}
}

/**
* Function that returns a list of all rectangles in the image.
* input  : pointer to SVGimage object
* return : pointer to LIST of all rectangles
**/
List* getRects(SVGimage* img) {
	if (img == NULL)
		return NULL;

	List *rectangles = initializeList(&rectangleToString, &delete, &compareRectangles);
	ListIterator iter = createIterator(img->rectangles);
	Rectangle *rect = NULL;
	while ((rect = nextElement(&iter)) != NULL) {
		insertBack(rectangles, rect);
	}
	getGroupElement(img->groups, rectangles, rectOffset);
	return rectangles;
}

/**
* Function that returns a list of all circles in the image.
* input  : pointer to SVGimage object
* return : pointer to LIST of all circles
**/
List* getCircles(SVGimage* img) {
	if (img == NULL)
		return NULL;

	List *circles = initializeList(&circleToString, &delete, &compareCircles);
	ListIterator iter = createIterator(img->circles);
	Circle *circle = NULL;
	while ((circle = nextElement(&iter)) != NULL) {
		insertBack(circles, circle);
	}
	getGroupElement(img->groups, circles, circleOffset);
	return circles;
}

/**
* Function that returns a list of all groups in the image.
* input  : pointer to SVGimage object
* return : pointer to LIST of all groups
**/
List* getGroups(SVGimage* img) {
	if (img == NULL)
		return NULL;

	List *groups = initializeList(&groupToString, &delete, &compareGroups);
	ListIterator iter = createIterator(img->groups);
	Group *group = NULL;
	while ((group = nextElement(&iter)) != NULL) {
		insertBack(groups, group);
	}
	getGroupElement(img->groups, groups, groupOffset);
	return groups;
}

/**
* Function that returns a list of all paths in the image.
* input  : pointer to SVGimage object
* return : pointer to LIST of all paths
**/
List* getPaths(SVGimage* img) {
	if (img == NULL)
		return NULL;

	List *paths = initializeList(&pathToString, &delete, &comparePaths);
	ListIterator iter = createIterator(img->paths);
	Path *path = NULL;
	while ((path = nextElement(&iter)) != NULL) {
		insertBack(paths, path);
	}
	getGroupElement(img->groups, paths, pathOffset);
	return paths;
}

/**
* Function which compare the area of the Rectangle
* input   first : pointer Rectangle struct
*         second: pointer the value to be compared
* return        : true if equal else false
**/
bool compareRectangleArea(const void *first, const void *second) {
	Rectangle *rectangle = (Rectangle *)first;
	float area = *(float *)second;
	float deltaArea = rectangle->width * rectangle->height;  // width * height
	if (ceil(area) == ceil(deltaArea))
		return true;
	return false;
}

/**
* Function which compare the area of the Circle
* input   first : pointer Circle struct
*         second: pointer the value to be compared
* return        : true if equal else false
**/
bool compareCircleArea(const void *first, const void *second) {
	Circle *circle = (Circle *)first;
	float area = *(float *)second;
	float deltaArea = M_PI * circle->r * circle->r;   // PI*R^2 
	if (ceil(area) == ceil(deltaArea))
		return true;
	return false;
}

/**
* Function that returns the number of all rectangles with the specified area
* input  : pointer to SVGimage object
* return : number of all rectangles with the specified area
**/
int numRectsWithArea(SVGimage* img, float area) {
	if (img == NULL)
		return 0;

	List *rectangles = getRects(img);
	int number = 0;
	ListIterator itr = createIterator(rectangles);

	void* data = nextElement(&itr);
	while (data != NULL) {
		if (compareRectangleArea(data, (const void *)&area)) {
			number++;
		}
		data = nextElement(&itr);
	}
	freeList(rectangles);
	return number;
}

/**
* Function that returns the number of all circles with the specified area
* input  : pointer to SVGimage object
* return : number of all circles with the specified area
**/
int numCirclesWithArea(SVGimage* img, float area) {
	if (img == NULL)
		return 0;

	List *circles = getCircles(img);
	int number = 0;
	ListIterator itr = createIterator(circles);

	void* data = nextElement(&itr);
	while (data != NULL) {
		if (compareCircleArea(data, (const void *)&area)) {
			number++;
		}
		data = nextElement(&itr);
	}
	freeList(circles);
	return number;
}

/**
* Function that returns the number of all paths with the specified data - i.e. Path.data field
* input  : pointer to SVGimage object
* return : number of all Path with the specified data
**/
int numPathsWithdata(SVGimage* img, char* data) {
	if (img == NULL)
		return 0;

	List *paths = getPaths(img);
	int number = 0;
	ListIterator itr = createIterator(paths);

	Path *element = nextElement(&itr);
	while (element != NULL) {
		if (!strcmp(data, element->data)) {
			number++;
		}
		element = nextElement(&itr);
	}
	freeList(paths);
	return number;
}

/**
* Function that returns the number of all groups with the specified length
* input  : pointer to SVGimage object
* return : number of all groups with the specified length
**/
int numGroupsWithLen(SVGimage* img, int len) {
	if (img == NULL)
		return 0;

	List *groups = getGroups(img);
	int number = 0;
	ListIterator itr = createIterator(groups);

	Group *element = nextElement(&itr);
	while (element != NULL) {
		int total_number = getLength(element->rectangles) +
			getLength(element->circles) +
			getLength(element->paths) +
			getLength(element->groups);
		if (total_number == len) {
			number++;
		}
		element = nextElement(&itr);
	}
	freeList(groups);
	return number;
}

/**
* Function that returns the number of all otherAttributes in Rectangle struct
* input  : pointer to SVGimage object
* return : number of all otherAttributes in Rectangle struct
**/
int countRectAttr(SVGimage* img) {
	if (img == NULL)
		return 0;

	List *rectangles = getRects(img);
	int number = 0;
	ListIterator itr = createIterator(rectangles);

	Rectangle* element;
	while ((element = nextElement(&itr)) != NULL) {
		number += getLength(element->otherAttributes);
	}
	freeList(rectangles);
	return number;
}

/**
* Function that returns the number of all otherAttributes in Circle struct
* input  : pointer to SVGimage object
* return : number of all otherAttributes in Circle struct
**/
int countCircleAttr(SVGimage* img) {
	if (img == NULL)
		return 0;

	List *circles = getCircles(img);
	int number = 0;
	ListIterator itr = createIterator(circles);

	Circle* element;
	while ((element = nextElement(&itr)) != NULL) {
		number += getLength(element->otherAttributes);
	}
	freeList(circles);
	return number;
}

/**
* Function that returns the number of all otherAttributes in Path struct
* input  : pointer to SVGimage object
* return : number of all otherAttributes in Path struct
**/
int countPathAttr(SVGimage* img) {
	if (img == NULL)
		return 0;

	List *paths = getPaths(img);
	int number = 0;
	ListIterator itr = createIterator(paths);

	Path* element;
	while ((element = nextElement(&itr)) != NULL) {
		number += getLength(element->otherAttributes);
	}
	freeList(paths);
	return number;
}

/**
* Function that returns the number of all otherAttributes in Group struct
* input  : pointer to SVGimage object
* return : number of all otherAttributes in Group struct
**/
int countGroupAttr(SVGimage* img) {
	if (img == NULL)
		return 0;

	List *groups = getGroups(img);
	int number = 0;
	ListIterator itr = createIterator(groups);

	Group* element;
	while ((element = nextElement(&itr)) != NULL) {
		number += getLength(element->otherAttributes);
	}
	freeList(groups);
	return number;
}

/**
* Function that returns the total number of Attribute structs in the SVGimage
* input  : pointer to SVGimage Object
* return : total number of Attribute structs in the SVGimage
**/
int numAttr(SVGimage* img) {
	if (img == NULL)
		return 0;

	int number = getLength(img->otherAttributes);
	number += countRectAttr(img);
	number += countCircleAttr(img);
	number += countPathAttr(img);
	number += countGroupAttr(img);
	return number;
}

/**
* Function Creates the SVGimage from the SVG image file
* input  : filename String with path
* return : pointer to the SVGimage Object
**/

SVGimage* createSVGimage(char* fileName)
{
	fprintf(stderr, "create image %s\n", fileName);
	if (fileName == NULL)
		return NULL;

	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;

	/*
	* this initialize the library and check potential ABI mismatches
	* between the version it was compiled for and the actual shared
	* library used.
	*/
	LIBXML_TEST_VERSION

		/*parse the file and get the DOM */
		doc = xmlReadFile(fileName, NULL, 0);

	if (doc == NULL) {
		printf("error: could not parse file %s\n", fileName);
		return NULL;
	}

	/*Get the root element node */
	root_element = xmlDocGetRootElement(doc);
	// Allocates memory for the SVGimage object
	SVGimage *image;
	image = (SVGimage *)malloc(sizeof(SVGimage));
	if (image == NULL) {
		printf("Error in Allocating SVGimage memory\n\r");
		return NULL;
	}
	memset(image, 0, sizeof(SVGimage));
	// initialize LIST for the SVGimage Object members
	image->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
	image->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
	image->paths = initializeList(&pathToString, &deletePath, &comparePaths);
	image->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);
	image->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
	// Read the NameSpace in the SVG image
	xmlNsPtr *nsList;
	xmlDoc *doc1 = NULL;
	nsList = xmlGetNsList(doc1, root_element->children); // get the list of all the NameSpace inside the SVG image
	// Since only one NameSpace is needed so the first NameSpace is copied
	if (*nsList != NULL) {
		strncpy(image->namespace, (const char *)nsList[0]->href, sizeof(image->namespace) - 1);
	}
	xmlFree(nsList);  // Free the NameSpace resource 

	xmlNode *cur_node = NULL; // Current Node usually children of the root_element 

	for (cur_node = root_element->children; cur_node != NULL; cur_node = cur_node->next) {
		// if the Element is title copied to the members
		if (!xmlStrcmp(cur_node->name, (const xmlChar *)"title")) {
			strncpy(image->title, (const char *)cur_node->children->content, sizeof(image->title) - 1);
		}
		// if the Element is desc copied to the member
		else if (!xmlStrcmp(cur_node->name, (const xmlChar *)"desc")) {
			strncpy(image->description, (const char *)cur_node->children->content, sizeof(image->description) - 1);
		}
		// insert the rectangle to Rectangle List
		else if (!xmlStrcmp(cur_node->name, (const xmlChar *)"rect")) {
			insertBack(image->rectangles, getRectangle(cur_node));
		}
		// insert the Circle to Circle List
		else if (!xmlStrcmp(cur_node->name, (const xmlChar *)"circle")) {
			insertBack(image->circles, getCircle(cur_node));
		}
		// insert the Path to Path List
		else if (!xmlStrcmp(cur_node->name, (const xmlChar *)"path")) {
			insertBack(image->paths, getPath(cur_node));
		}
		// insert the Group to Group List
		else if (!xmlStrcmp(cur_node->name, (const xmlChar *)"g")) {
			insertBack(image->groups, getGroup(cur_node));
		}
	}
	// Copy the Attributes of Root_element to otherAttributes List 
	xmlAttr *attr;
	for (attr = root_element->properties; attr != NULL; attr = attr->next) {
		xmlNode *value = attr->children;
		Attribute *attribute = (Attribute *)malloc(sizeof(Attribute));
		attribute->name = strdup((const char *)attr->name);
		attribute->value = strdup((const char *)value->content);
		insertBack(image->otherAttributes, attribute);
	}

	/*free the document */
	xmlFreeDoc(doc);

	/*
	*Free the global variables that may
	*have been allocated by the parser.
	*/
	xmlCleanupParser();
	fprintf(stderr, "return image\n");
	return image;
}
/**
* Function converts the SVGimage Object into a String representation.
* input  : pointer to SVGimage Object
* return : address of the String
**/
char* SVGimageToString(SVGimage* img) {
	if (img == NULL)
		return NULL;

	// members in the SVGimage object converted with their respective conversion function
	char *rectangles = toString(img->rectangles);
	char *circles = toString(img->circles);
	char *paths = toString(img->paths);
	char *groups = toString(img->groups);
	char *attr = toString(img->otherAttributes);
	// total number of memory byte required for the String
	int length = strlen(rectangles) + strlen(circles) + strlen(paths) + strlen(groups) + strlen(attr) +
		strlen(img->namespace) + strlen(img->title) + strlen(img->description) + strlen(imageString) + 1;
	// Allocates the memory 
	char *string = (char *)malloc(length);
	// combine All Strings
	sprintf(string, imageString, img->namespace, img->title, img->description);
	strcat(string, rectangles);
	strcat(string, circles);
	strcat(string, paths);
	strcat(string, groups);
	strcat(string, attr);
	// free the resources
	free(rectangles);
	free(circles);
	free(paths);
	free(groups);
	free(attr);

	return string;
}

/**
* Function frees the resource Allocated for the SVGimage
* input  : pointer to SVGimage Object
* return : none
**/
void deleteSVGimage(SVGimage* img) {
	if (img == NULL)
		return;
	freeList(img->rectangles);
	freeList(img->circles);
	freeList(img->paths);
	freeList(img->groups);
	freeList(img->otherAttributes);
	free(img);
}


//
///** Function to create an SVG object based on the contents of an SVG file.
// * This function must validate the XML tree generated by libxml against a SVG schema file
// * before attempting to traverse the tree and create an SVGimage struct
// *@pre File name cannot be an empty string or NULL.
//       File represented by this name must exist and must be readable.
//       Schema file name is not NULL/empty, and represents a valid schema file
// *@post Either:
//        A valid SVGimage has been created and its address was returned
//                or 
//                An error occurred, or SVG file was invalid, and NULL was returned
// *@return the pinter to the new struct or NULL
// *@param fileName - a string containing the name of the SVG file
// **/

SVGimage* createValidSVGimage(char* fileName, char* schemaFile) {
    /**
     * Validate filename
     */
    if (fileName == NULL || *fileName == '\0') {
        return NULL;
    }

    /**
     * Validate schema file name
     */

    if (schemaFile == NULL || *schemaFile == '\0') {
        return NULL;
    }

	bool validate = false;
    /**
     * Validate schema file
     */
	xmlDocPtr doc;
	xmlSchemaPtr schema = NULL;
	xmlSchemaParserCtxtPtr ctxt;

	ctxt = xmlSchemaNewParserCtxt(schemaFile);

	schema = xmlSchemaParse(ctxt);
	xmlSchemaFreeParserCtxt(ctxt);

	doc = xmlReadFile(fileName, NULL, 0);

	if (doc == NULL)
	{
		fprintf(stderr, "Could not parse %s\n", fileName);
		return NULL;
	}
	else
	{
		xmlSchemaValidCtxtPtr ctxt;
		int ret;

		ctxt = xmlSchemaNewValidCtxt(schema);
		ret = xmlSchemaValidateDoc(ctxt, doc);
		if (ret == 0)
		{
			printf("%s validates\n", fileName);

			validate = true;
		}
		xmlSchemaFreeValidCtxt(ctxt);

	}

	xmlFreeDoc(doc);
	// free the resource
	if (schema != NULL)
		xmlSchemaFree(schema);

	xmlSchemaCleanupTypes();
	xmlCleanupParser();

	if (validate)
		return createSVGimage(fileName);
	else
		return NULL;
}


bool addAttributesToNode(xmlNodePtr node, List* attributes)
{

    if (node == NULL || attributes == NULL)
        return false;
    ListIterator iter = createIterator(attributes);
    Attribute* attr = NULL;

    while ((attr = nextElement(&iter)) != NULL)
    {
		if (attr->name == NULL)
			return false;
        xmlNewProp(node, BAD_CAST attr->name, BAD_CAST attr->value);

    }
	return true;
}

bool addGroupChildsToNode(xmlNodePtr parent, Group* group)
{
    if (parent == NULL || group == NULL)
        return false;

	ListIterator iter;
	char buf[80];
	memset(buf, 0, 80);
    xmlNodePtr node = NULL;

	if (group->rectangles) {
		iter = createIterator(group->rectangles);
		Rectangle* rect = NULL;
		while ((rect = nextElement(&iter)) != NULL)
		{
			node = xmlNewChild(parent, NULL, BAD_CAST "rect", NULL);
            sprintf(buf, "%.2f%s", rect->x, rect->units);
			xmlNewProp(node, BAD_CAST "x", BAD_CAST buf);
            sprintf(buf, "%.2f%s", rect->y, rect->units);
			xmlNewProp(node, BAD_CAST "y", BAD_CAST buf);
            sprintf(buf, "%.2f%s", rect->width, rect->units);
			xmlNewProp(node, BAD_CAST "width", BAD_CAST buf);
            sprintf(buf, "%.2f%s", rect->height, rect->units);
			xmlNewProp(node, BAD_CAST "height", BAD_CAST buf);

			if (!addAttributesToNode(node, rect->otherAttributes))
				return false;
		}
	}

	if (group->circles) {
		iter = createIterator(group->circles);
		Circle* circle = NULL;
		while ((circle = nextElement(&iter)) != NULL)
		{
			node = xmlNewChild(parent, NULL, BAD_CAST "circle", NULL);
            sprintf(buf, "%.2f%s", circle->cx, circle->units);
			xmlNewProp(node, BAD_CAST "cx", BAD_CAST buf);
            sprintf(buf, "%.2f%s", circle->cy, circle->units);
			xmlNewProp(node, BAD_CAST "cy", BAD_CAST buf);
            sprintf(buf, "%.2f%s", circle->r, circle->units);
			xmlNewProp(node, BAD_CAST "r", BAD_CAST buf);

			if (!addAttributesToNode(node, circle->otherAttributes))
				return false;
		}
	}

	if (group->paths) {
		iter = createIterator(group->paths);
		Path* path = NULL;
		while ((path = nextElement(&iter)) != NULL)
		{
			node = xmlNewChild(parent, NULL, BAD_CAST "path", NULL);
			xmlNewProp(node, BAD_CAST "d", BAD_CAST path->data);
			if (!addAttributesToNode(node, path->otherAttributes))
				return false;
		}
	}

	if (group->groups) {
		iter = createIterator(group->groups);
		Group* group = NULL;
		while ((group = nextElement(&iter)) != NULL)
		{
			node = xmlNewChild(parent, NULL, BAD_CAST "g", NULL);
			if (!addAttributesToNode(node, group->otherAttributes))
				return false;
			if (!addGroupChildsToNode(node, group))
				return false;
		}
	}

	return true;
}

/**
 * Function convert data from SVGimage format to xmlDocPtr format
 */
xmlDocPtr convertSVGimageToDoc(SVGimage* image) {
	bool fail = false;

    xmlDocPtr doc = xmlNewDoc(NULL);
    xmlNodePtr root_node = NULL; /* node pointers */
    root_node = xmlNewNode(NULL, BAD_CAST "svg");
    xmlDocSetRootElement(doc, root_node);
    xmlNsPtr ns = xmlNewNs(root_node, BAD_CAST "http://www.w3.org/2000/svg", NULL);
    xmlSetNs(root_node, ns);

    if (!addAttributesToNode(root_node, image->otherAttributes)) {
	    xmlCleanupParser();
		xmlFreeDoc(doc);
		return NULL;
	}

	if (strlen(image->title) > 0)
		xmlNewChild(root_node, NULL, BAD_CAST "title", BAD_CAST image->title);

	if (strlen(image->description) > 0)
		xmlNewChild(root_node, NULL, BAD_CAST "desc", BAD_CAST image->description);

    xmlNodePtr node = NULL;

    ListIterator iter;
	char buf[80];
    memset(buf, 0, 80);

    if (image->rectangles) {
        iter = createIterator(image->rectangles);
        Rectangle* rect = NULL;
        while ((rect = nextElement(&iter)) != NULL)
        {	
			node = xmlNewChild(root_node, NULL, BAD_CAST "rect", NULL);
            sprintf(buf, "%.2f%s", rect->x, rect->units);
            xmlNewProp(node, BAD_CAST "x", BAD_CAST buf);
            sprintf(buf, "%.2f%s", rect->y, rect->units);
            xmlNewProp(node, BAD_CAST "y", BAD_CAST buf);
            sprintf(buf, "%.2f%s", rect->width, rect->units);
            xmlNewProp(node, BAD_CAST "width", BAD_CAST buf);
            sprintf(buf, "%.2f%s", rect->height, rect->units);
            xmlNewProp(node, BAD_CAST "height", BAD_CAST buf);

            if (!addAttributesToNode(node, rect->otherAttributes)) {
				fail = true;
				break;
			}
        }
    }


	if (image->circles) {
		iter = createIterator(image->circles);
		Circle* circle = NULL;
		while ((circle = nextElement(&iter)) != NULL)
		{
			node = xmlNewChild(root_node, NULL, BAD_CAST "circle", NULL);
            sprintf(buf, "%.2f%s", circle->cx, circle->units);
			xmlNewProp(node, BAD_CAST "cx", BAD_CAST buf);
            sprintf(buf, "%.2f%s", circle->cy, circle->units);
			xmlNewProp(node, BAD_CAST "cy", BAD_CAST buf);
            sprintf(buf, "%.2f%s", circle->r, circle->units);
			xmlNewProp(node, BAD_CAST "r", BAD_CAST buf);

            if (!addAttributesToNode(node, circle->otherAttributes)) {
				fail = true;
				break;
			}
		}
	}

	if (image->paths) {
		iter = createIterator(image->paths);
		Path* path = NULL;
		while ((path = nextElement(&iter)) != NULL)
		{
			node = xmlNewChild(root_node, NULL, BAD_CAST "path", NULL);
            xmlNewProp(node, BAD_CAST "d", BAD_CAST path->data);
            if (!addAttributesToNode(node, path->otherAttributes)) {
				fail = true;
				break;
			}
		}
	}

	if (image->groups) {
		iter = createIterator(image->groups);
		Group* group = NULL;
		while ((group = nextElement(&iter)) != NULL)
		{
			node = xmlNewChild(root_node, NULL, BAD_CAST "g", NULL);

            if (!addAttributesToNode(node, group->otherAttributes)) {
				fail = true;
				break;
			}
			if (!addGroupChildsToNode(node, group)) {
				fail = true;
				break;
			}
		}
	}

    xmlCleanupParser();

	if (fail) {
		xmlFreeDoc(doc);
		return NULL;
	}
    return doc;
}

/** Function to writing a SVGimage into a file in SVG format.
 *@pre
    SVGimage object exists, is valid, and and is not NULL.
    fileName is not NULL, has the correct extension
 *@post SVGimage has not been modified in any way, and a file representing the
    SVGimage contents in SVG format has been created
 *@return a boolean value indicating success or failure of the write
 *@param
    doc - a pointer to a SVGimage struct
        fileName - the name of the output file
 **/
bool writeSVGimage(SVGimage* image, char* fileName) {
    if (image == NULL) {
        return false;
    }
    /**
     * Validate filename
     */
    if (fileName == NULL || *fileName == '\0') {
        return NULL;
    }
    /**
     * Convert SVGImage to xmlDoc
     */
	
    xmlDocPtr doc = convertSVGimageToDoc(image);
	if (doc == NULL)
		return false;

    /*
     *      Write to file
     */

    xmlSaveFormatFileEnc(fileName, doc, "UTF-8", 1);
	
    xmlFreeDoc(doc);
	return true;
}

/** Function to validating an existing a SVGimage object against a SVG schema file
 *@pre 
    SVGimage object exists and is not NULL
    schema file name is not NULL/empty, and represents a valid schema file
 *@post SVGimage has not been modified in any way
 *@return the boolean aud indicating whether the SVGimage is valid
 *@param obj - a pointer to a GPXSVGimagedoc struct
 *@param obj - the name iof a schema file
 **/
bool validateSVGimage(SVGimage* image, char* schemaFile) {

    if (image == NULL) {
        return false;
    }
    /**
     * Validate schemaFile
     */
    if (schemaFile == NULL || *schemaFile == '\0') {
        return NULL;
    }
    /**
     * Convert SVGImage to xmlDoc
     */
    xmlDocPtr doc = convertSVGimageToDoc(image);
    if (doc == NULL) {
        return false;
    }
    /**
     * Validate schema
     */
    xmlSchemaParserCtxtPtr parserCtxt = xmlSchemaNewParserCtxt(schemaFile);
    if (parserCtxt == NULL) {
        fprintf(stderr, "Could not create XSD schema parsing context.\n");
        return false;
    }
    xmlSchemaPtr schema = xmlSchemaParse(parserCtxt);
    if (schema == NULL) {
        fprintf(stderr, "Could not parse XSD schema.\n");
        return false;
    }

    xmlSchemaValidCtxtPtr validCtxt = xmlSchemaNewValidCtxt(schema);

    if (!validCtxt) {
        fprintf(stderr, "Could not create XSD schema validation context.\n");
        return false;
    }

    int result = xmlSchemaValidateDoc(validCtxt, doc);

	xmlSchemaFreeParserCtxt(parserCtxt);
	xmlSchemaFree(schema);
	xmlSchemaFreeValidCtxt(validCtxt);
	xmlFreeDoc(doc);

    if (result == 0) {
        return true;
    } else {
        return false;
    }
}

void setAttribute(SVGimage* image, elementType elemType, int elemIndex, Attribute* newAttr)
{
    List* listAttributes = NULL;
    ListIterator iter;


    if (image == NULL || newAttr == NULL)
        return;

	if (strcmp("title", newAttr->name) == 0)
	{
		strcpy(image->title, newAttr->value);
		return;
	}
	
	if (strcmp("description", newAttr->name) == 0)
	{
		strcpy(image->description, newAttr->value);
		return;
	}

	Attribute *newAttribute = (Attribute*)malloc(sizeof(Attribute));
    newAttribute->name = strdup(newAttr->name);
    newAttribute->value = strdup(newAttr->value);


	
    if (elemType == SVG_IMAGE) {
        listAttributes = image->otherAttributes;
    }
    else {

		List* listElements = NULL;

	    if (elemType == RECT)
		    listElements = image->rectangles;
        else if (elemType == CIRC)
            listElements = image->circles;
        else if (elemType == PATH)
            listElements = image->paths;
        else if (elemType == GROUP)
            listElements = image->groups;
      
        if (listElements == NULL) return;

        iter = createIterator(listElements);

        void* element = NULL;
        char* stop = NULL;

        while ((element = nextElement(&iter)) != NULL)
        {
            if (elemIndex < 0)
                return;

            if (elemIndex == 0)
            {
                if (elemType == RECT) {
                    Rectangle* rect = element;
                    if (strcmp(newAttribute->name, "x") == 0)
                        rect->x = strtof(newAttribute->value, &stop);
                    else if (strcmp(newAttribute->name, "y") == 0)
                        rect->y = strtof(newAttribute->value, &stop);
                    else if (strcmp(newAttribute->name, "width") == 0)
                        rect->width = strtof(newAttribute->value, &stop);
                    else if (strcmp(newAttribute->name, "height") == 0)
                        rect->height = strtof(newAttribute->value, &stop);
                    else {
                        listAttributes = rect->otherAttributes;
                        break;
                    }
                } 
                else if (elemType == CIRC) {
                    Circle* circle = element;
                    if (strcmp(newAttribute->name, "cx") == 0)
                        circle->cx = strtof(newAttribute->value, &stop);
                    else if (strcmp(newAttribute->name, "cy") == 0)
                        circle->cy = strtof(newAttribute->value, &stop);
                    else if (strcmp(newAttribute->name, "r") == 0)
                        circle->r = strtof(newAttribute->value, &stop);
                    else {
                        listAttributes = ((Circle*)element)->otherAttributes;
                        break;
                    }
                }
                else if (elemType == PATH) {
                    Path* path = element;
                    if (strcmp(newAttribute->name, "d") == 0) {
                        free(path->data);
                        path->data = strdup(newAttribute->value);
                    }
                    else {
                        listAttributes = ((Path*)element)->otherAttributes;
                        break;
                    }
                }
                else if (elemType == GROUP) {
                    listAttributes = ((Group*)element)->otherAttributes;
                    break;
                }
                return;
            }

            elemIndex--;
        }

    }

	if (listAttributes == NULL)
		return;	

    iter = createIterator(listAttributes);
	Attribute* attr = NULL;
	while ((attr = nextElement(&iter)) != NULL)
	{
		if (strcmp(attr->name, newAttribute->name) == 0) {
			char* temp = attr->value;
			attr->value = newAttribute->value;
			newAttribute->value = temp;
			deleteAttribute(newAttribute);
			return;
		}
	}

    insertBack(listAttributes, newAttribute);
}

void addComponent(SVGimage* image, elementType elemType, void* newComponent)
{
    if (image == NULL || newComponent == NULL)
        return;

    if (elemType == RECT){
		
        insertBack(image->rectangles, newComponent);
	}
    else if (elemType == CIRC)
        insertBack(image->circles, newComponent);
    else if (elemType == PATH)
        insertBack(image->paths, newComponent);

}

char* attrToJSON(const Attribute* a)
{
	if (a == NULL)
		return "{}";
	char *json = malloc(5000);
	memset(json, 0, 5000);
	sprintf(json, "{\"name\":\"%s\",\"value\":\"%s\"}", a->name, a->value);
	return json;
}

char* circleToJSON(const Circle* c)
{
	if (c == NULL)
		return "{}";
	char *json = malloc(1000);
	memset(json, 0, 1000);
	char* addAttr;
	addAttr = attrListToJSON(c->otherAttributes);
	sprintf(json, "{\"cx\":%.2f,\"cy\":%.2f,\"r\":%.2f,\"numAttr\":%d,\"units\":\"%s\",\"getAttr\":%s}", c->cx, c->cy, c->r, c->otherAttributes->length, c->units, addAttr);
	return json;
}

char* rectToJSON(const Rectangle* r)
{
	if (r == NULL)
		return "{}";
	char *json = malloc(1000);
	memset(json, 0, 1000);
	char* addAttr;
	addAttr = attrListToJSON(r->otherAttributes);
	sprintf(json, "{\"x\":%.2f,\"y\":%.2f,\"w\":%.2f,\"h\":%.2f,\"numAttr\":%d,\"units\":\"%s\",\"getAttr\":%s}", r->x, r->y, r->width, r->height, r->otherAttributes->length, r->units, addAttr);
	return json;

}

char* pathToJSON(const Path* p)
{
	if (p == NULL)
		return "{}";
	char *json = malloc(50000);
	memset(json, 0, 50000);
	char* addAttr;
	addAttr = attrListToJSON(p->otherAttributes);
	sprintf(json, "{\"d\":\"%s\",\"numAttr\":%d,\"getAttr\":%s}", p->data, p->otherAttributes->length, addAttr);
	return json;

}

char* groupToJSON(const Group* g)
{
	if (g == NULL)
		return "{}";
	char *json = malloc(1000);
	memset(json, 0, 1000);
	int sum = g->rectangles->length + g->circles->length + g->paths->length + g->groups->length;
	char* addAttr;
	addAttr = attrListToJSON(g->otherAttributes);
	sprintf(json, "{\"children\":%d,\"numAttr\":%d,\"getAttr\":%s}", sum, g->otherAttributes->length, addAttr);
	return json;

}

char* attrListToJSON(const List* list)
{
	if (list == NULL)
		return "[]";
	char *json = malloc(50000);
	memset(json, 0, 50000);
	Attribute *attr = NULL;
	ListIterator iter = createIterator((List*)list);
	strcat(json, "[");
	while ((attr = nextElement(&iter)) != NULL)
	{
		strcat(json, attrToJSON(attr));
		if (attr != list->tail->data)
			strcat(json, ",");
	}
	strcat(json, "]");
	return json;
}

char* circListToJSON(const List* list)
{
	if (list == NULL)
		return "[]";
	char *json = malloc(10000);
	memset(json, 0, 10000);
	Circle* crle = NULL;
	ListIterator iter = createIterator((List*)list);
	strcat(json, "[");
	while ((crle = nextElement(&iter)) != NULL)
	{
		strcat(json, circleToJSON(crle));
		if (crle != list->tail->data)
			strcat(json, ",");
	}
	strcat(json, "]");
	return json;
}

char* rectListToJSON(const List* list)
{
	if (list == NULL)
		return "[]";
	char *json = malloc(10000);
	memset(json, 0, 10000);
	Rectangle* rect = NULL;
	ListIterator iter = createIterator((List*)list);
	strcat(json, "[");
	while ((rect = nextElement(&iter)) != NULL)
	{
		strcat(json, rectToJSON(rect));
		if (rect != list->tail->data)
			strcat(json, ",");
	}
	strcat(json, "]");
	return json;

}

char* pathListToJSON(const List* list)
{
	if (list == NULL)
		return "[]";
	char *json = malloc(5000000);
	memset(json, 0, 5000000);
	Path* p = NULL;
	ListIterator iter = createIterator((List*)list);
	strcat(json, "[");
	while ((p = nextElement(&iter)) != NULL)
	{
		strcat(json, pathToJSON(p));
		if (p != list->tail->data)
			strcat(json, ",");
	}
	strcat(json, "]");
	return json;

}

char* groupListToJSON(const List* list)
{
	if (list == NULL)
		return "[]";
	char *json = malloc(10000);
	memset(json, 0, 10000);
	Group* gp = NULL;
	ListIterator iter = createIterator((List*)list);
	strcat(json, "[");
	while ((gp = nextElement(&iter)) != NULL)
	{
		strcat(json, groupToJSON(gp));
		if (gp != list->tail->data)
			strcat(json, ",");
	}
	strcat(json, "]");
	return json;

}

char* SVGtoJSON(const SVGimage* imge)
{
	if (imge == NULL)
		return "{}";

	List *Rectangles = getRects((SVGimage*)imge);
	List *Circles = getCircles((SVGimage*)imge);
	List *Paths = getPaths((SVGimage*)imge);
	List *Groups = getGroups((SVGimage*)imge);

	char *json = malloc(100);
	memset(json, 0, 100);
	sprintf(json, "{\"numRect\":%d,\"numCirc\":%d,\"numPaths\":%d,\"numGroups\":%d}", getLength(Rectangles), getLength(Circles), getLength(Paths), getLength(Groups));
	return json;

}

char* SVGtoJSONInfo(const SVGimage* imge)
{
	if (imge == NULL)
		return "{}";

	char* jsonCircle = circListToJSON((List*)imge->circles);
	char* jsonRectangle = rectListToJSON((List*)imge->rectangles);
	char* jsonPath = pathListToJSON((List*)imge->paths);
	char* jsonGroup = groupListToJSON((List*)imge->groups);
	char* jsonAttri = attrListToJSON((List*)imge->otherAttributes);

	char *json = malloc(5000);
	memset(json, 0, 5000);
	if (strlen(imge->description) == 0)
		if (strlen(imge->title) == 0)
			sprintf(json, "{\"Title\":\"\",\"Description\":\"\",\"Circles\":%s,\"Rectangles\":%s,\"Paths\":%s,\"Groups\":%s,\"OtherAttributes\":%s}", jsonCircle, jsonRectangle, jsonPath, jsonGroup, jsonAttri);
		else
			sprintf(json, "{\"Title\":\"\",\"Description\":\"%s\",\"Circles\":%s,\"Rectangles\":%s,\"Paths\":%s,\"Groups\":%s,\"OtherAttributes\":%s}", imge->description, jsonCircle, jsonRectangle, jsonPath, jsonGroup, jsonAttri);
	else
		if (strlen(imge->title) == 0)
			sprintf(json, "{\"Title\":\"%s\",\"Description\":\"\",\"Circles\":%s,\"Rectangles\":%s,\"Paths\":%s,\"Groups\":%s,\"OtherAttributes\":%s}", imge->title, jsonCircle, jsonRectangle, jsonPath, jsonGroup, jsonAttri);
		else
			sprintf(json, "{\"Title\":\"%s\",\"Description\":\"%s\",\"Circles\":%s,\"Rectangles\":%s,\"Paths\":%s,\"Groups\":%s,\"OtherAttributes\":%s}", imge->title, imge->description, jsonCircle, jsonRectangle, jsonPath, jsonGroup, jsonAttri);

	free(jsonCircle);
	free(jsonRectangle);
	free(jsonPath);
	free(jsonGroup);
	free(jsonAttri);

	return json;
}

char* METAtoJSON(const SVGimage* imge)
{
	if (imge == NULL)
		return "{}";

	char *json = malloc(300);
	memset(json, 0, 300);

	if (strlen(imge->description) == 0)
		if (strlen(imge->title) == 0)
			strcpy(json, "\"\":\"\"");
		else
			sprintf(json, "\"%s\":\"\"",imge->title);
	else
		if (strlen(imge->title) == 0)
			sprintf(json, "\"\":\"%s\"",imge->description);
		else
			sprintf(json, "\"%s\":\"%s\"",imge->title ,imge->description);
	return json;
}


SVGimage* JSONtoSVG(const char* svgString)
{
	char title[800];
	char descr[1000];
	sscanf(svgString, "{\"title\":\"%499[^\"]\",\"descr\":\"%999[^\"]\"}", title, descr);

	SVGimage *image;
	image = (SVGimage *)malloc(sizeof(SVGimage));
	if (image == NULL) {
		printf("Error in Allocating SVGimage memory\n\r");
		return NULL;
	}
	memset(image, 0, sizeof(SVGimage));
	// initialize LIST for the SVGimage Object members
	image->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
	image->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
	image->paths = initializeList(&pathToString, &deletePath, &comparePaths);
	image->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);
	image->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
	strcpy(image->title, title);
	strcpy(image->description, descr);

	return image;

}

Rectangle* JSONtoRect(const char* svgString)
{
	
	float xVal = 0.0f;
	float yVal = 0.0f;
	float wVal = 0.0f;
	float hVal = 0.0f;
	char units[20];
	memset(units, 0, 20);

	sscanf(svgString, "{\"x\":%g,\"y\":%g,\"w\":%g,\"h\":%g,\"units\":\"%19[^\"]\"}", &xVal, &yVal, &wVal, &hVal, units);

	Rectangle *rec;
	rec = (Rectangle *)malloc(sizeof(Rectangle));
	if (rec == NULL) {
		printf("Error in Allocating Rectangle image memory\n\r");
		return NULL;
	}
	memset(rec, 0, sizeof(Rectangle));
	// initialize other member of Rectangle
	rec->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
	printf("%g %g %g %g %s\n", xVal, yVal, wVal, hVal, units);
	rec->x = xVal;
	rec->y = yVal;
	rec->width = wVal;
	rec->height = hVal;
	strcpy(rec->units, units);

	return rec;

}

Circle* JSONtoCircle(const char* svgString)
{
	float cxVal = 0.0f;
	float cyVal = 0.0f;
	float rVal = 0.0f;
	char units[20];
	memset(units, 0, 20);

	sscanf(svgString, "{\"cx\":%g,\"cy\":%g,\"r\":%g,\"units\":\"%19[^\"]\"}", &cxVal, &cyVal, &rVal, units);

	Circle *circ;
	circ = (Circle *)malloc(sizeof(Circle));
	if (circ == NULL) {
		printf("Error in Allocating Rectangle image memory\n\r");
		return NULL;
	}
	memset(circ, 0, sizeof(Circle));

	// initialize other member of Rectangle
	circ->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
	printf("%g %g %g %s\n", cxVal, cyVal, rVal, units);

	circ->cx = cxVal;
	circ->cy = cyVal;
	circ->r = rVal;
	strcpy(circ->units, units);

	return circ;

}



