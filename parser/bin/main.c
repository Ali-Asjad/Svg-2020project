#include <stdio.h>
#include <string.h> 
#include "SVGParser.h"


/**
 * Simple example to parse a file called "file.xml", 
 * walk down the DOM, and print the name of the 
 * xml elements nodes.
 */
int
main(int argc, char **argv)
{
    SVGimage* img;	

    img = createSVGimage(argv[1]); 

    // char *imageToString = SVGimageToString(img);
    // printf("%s", imageToString);
    // free(imageToString);
    
	//  img = createValidSVGimage(argv[1], "svg.xsd");
	//  if (img == NULL)
	//  	return -1;
	//  if (validateSVGimage(img, "svg.xsd"))
	//     writeSVGimage(img, "new.svg");


    // Attribute *newAttr = (Attribute*)malloc(sizeof(Attribute));
    // newAttr->name = strdup("stroke-width");
    // newAttr->value = strdup("7");

    // setAttribute(img, PATH, 0, newAttr);
	// writeSVGimage(img, "new.svg");

    // Rectangle* rectangle = (Rectangle*)malloc(sizeof(Rectangle));
	//  memset(rectangle, 0, sizeof(Rectangle));
    // rectangle->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

	// rectangle->width = 100;
	// rectangle->height = 100;
    // rectangle->x = rectangle->y = 100;

    Rectangle* imgtest;
	//Circle* imgtest;
	char* testjson;
	testjson = "{\"x\":1,\"y\":2,\"w\":19,\"h\":15,\"units\":\"cm\"}";
    printf(testjson);
	imgtest = JSONtoRect(testjson);

    addComponent(img, RECT, imgtest);
	writeSVGimage(img, "test.svg");

    List *Rectangles 	= getRects(img);
    List *Circles 	= getCircles(img);
    List *Paths 	= getPaths(img);
    List *Groups 	= getGroups(img);		

    // printf("\nStats\n");

    // printf("Total Number of Rectangles : %d\n", getLength(Rectangles));
    // printf("Total Number of Circles    : %d\n", getLength(Circles));	
    // printf("Total Number of Paths      : %d\n", getLength(Paths)); 
    // printf("Total Number of Groups     : %d\n\n", getLength(Groups));
    
    // printf("Number of Rectangles with Area 315 : %d\n", numRectsWithArea(img, 315));
    // printf("Number of Circles with Area 315    : %d\n", numCirclesWithArea(img, 315.0));
    // numPathsWithdata(img, "");
    // printf("Number of Group with 5 elements    : %d\n", numGroupsWithLen(img, 5));
    // printf("Number of Attributes               : %d\n", numAttr(img));
	
	

   
    
    // Rectangle* rectangle = (Rectangle*)malloc(sizeof(Rectangle));
	//  memset(rectangle, 0, sizeof(Rectangle));
    // rectangle->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

	// rectangle->width = 100;
	// rectangle->height = 100;
    // rectangle->x = rectangle->y = 100;

    // addComponent(img, RECT, rectangle);
	// writeSVGimage(img, "test1.svg");

    // char *getJson;
	// Attribute *newAttr = (Attribute*)malloc(sizeof(Attribute));
	// newAttr->name = strdup("stroke-width");
	// newAttr->value = strdup("8");
	// getJson = attrToJSON(newAttr);
	// printf(getJson);
	// free(getJson);

	
	freeList(Rectangles);
	freeList(Circles);
	freeList(Paths);
	freeList(Groups);
    // free(newAttr->name);
    // free(newAttr->value);
    // free(newAttr);

    deleteSVGimage(img);


    return 0;
}
