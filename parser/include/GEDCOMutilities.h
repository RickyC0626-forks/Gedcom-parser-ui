#ifndef GEDCOMUTILITIES_H
#define GEDCOMUTILITIES_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "LinkedListAPI.h"
#include "GEDCOMparser.h"

char *trim(char *string);

/**
 * Function that creates a header
 * It will malloc space for the header and return the pointer to the 
 * allocated object
 * All the fields will by default be set to null at the beginning
 * A set of setters will later set the different fields
 */
Header *createHeader();

/*
* Function that sets the source of the header
* Takes in a header pointer ;
*/
void setHeaderSource(Header * header, char *source);

/*
* function that sets the header encoding
*/
void setHeaderGEDCversion(Header * header, char *version);

/*
* Function that sets the header encoding;
*/
void setHeaderEncoding(Header * header, char *encoding);

/*
* Function that sets any other tags into other fields 
*/
void setHeaderOtherFields(Header * header, char *tag, char *value);

void deleteHeader(void *toBeDeleted);


// **************************************************************************
// Field helper functions;

//  function that creates a field
// mallocs space for it and sets  everything to null
Field *createField();

// function that sets the tag
// the tag must not be null
void setFieldTag(Field *field, char *tag);

// sets the field value
// the value must not be null
void setFieldValue(Field *field, char *value);



// **********************************************************************
// Individual helper Functions

/*
* This function creates an individual struct
* It Mallocs space for it and the lists are initialized
* the name and surname are set to NULL at first
*/
Individual *createIndividual();

/*
* Function that sets the Given names of the Individuals
*/
void setIndividualGivenName(Individual * individual, char *givenName);

/**
 * Function to set the surname
 * it mallocs space for it
 * */

void setIndividualSurname(Individual *individual, char *surname);

/**
 * Function to insert an event into the individual list
 * */

void setIndividualEvent(Individual *individual, Event *event);

/**
 * Function to set a family in the individual list
 * 
 * */

void setIndividualFamily(Individual *individual, Family *family);

/**
 * Function to  set others fields into the indiividual list
 * */

void setIndividualOtherFields(Individual *individual, char *tag, char *value);


//********************************************************************************
// Family Functions

Family *createFamily();
void setFamilyWife(Family *family, Individual *individual);
void setFamilyHusb(Family *family, Individual *individual);
void insertChildInFamily(Family *family, Individual *individual);
void insertEventInFamily(Family *family, Event *event);
void insertOtherFieldsInFamily(Family *family, Field *field);

//**********************************************************************************


//**********************************************************************************
// Submitter Functions

Submitter *createSubmitter();
void setSubmitterName(Submitter *submitter, char *name);
void setSubmitterAddress(Submitter *submitter, char *address);
void insertSubmitterOtherFields(Submitter *submitter, Field *field);
void deleteSubmitter(void *toBeDeleted);

void dummyDelete(void *toBeDeleted);

//**********************************************************************************


//***********************************************************************************
// Event Functions

Event *createEvent();
void setEventType(Event *event, char *type);
void setEventDate(Event *event, char *date);
void setEventPlace(Event *event, char *place);
void insertEventOtherFields(Event *event, Field *field);

//************************************************************************************


GEDCOMerror createINV_FILE();
GEDCOMerror createINV_RECORD(int line);
GEDCOMerror createOK();
GEDCOMerror createINV_GEDCOM();
GEDCOMerror createINV_HEADER(int line);
GEDCOMerror createOTHER_ERROR();

/**
 * function that checks for tags and returns a bool;
 * */
bool checkForTags(char *str);



/**
 * Function that creates a gedcom object
 * does all the initializations
 * sets all  to null;
 * */
GEDCOMobject *createObject();
void insertFamilyInObject(GEDCOMobject *obj, Family *family);
void insertIndividualInObject(GEDCOMobject *obj, Individual *individual);








// ****************************************************************************
// Functions to help in linking

/**
 * struct that contains the ref of the individual
 * if also has a pointer to that individual
 * */
typedef struct{
    char *ref;
    Individual * individual;
}IndiRef;

/**
 * creates an individual reference struct
 * mallocs space for it 
 * sets all the values to NULL
 * */
IndiRef *createIndiRef();

void setIndiRefRef(IndiRef *indi, char *ref);
void setIndiRefIndi(IndiRef *indi, Individual *individual);

void deleteIndiRef(void* toBeDeleted);
int compareIndiRef(const void* first,const void* second);
char* printIndiRef(void* toBePrinted);


typedef struct{
    char *ref;
    char *tag;
    Family *family;
}FamRef;

FamRef *createFamRef();

void setFamRefTag(FamRef *famref, char *tag);
void setFamRefRef(FamRef *famref, char *ref);
void setFamRefFam(FamRef *famref, Family *family);

void deleteFamRef(void *toBeDeleted);
int compareFamRef(const void *first, const void* second);
char *printFamRef(void *toBePrinted);

typedef struct{
    List IndiRef;
    List FamRef;
}ListRef;

ListRef *createListRef();
void insertFamRef(ListRef *listref, FamRef *famref);
void insertIndiRef(ListRef *listref, IndiRef *indiref);
void deleteListRef(ListRef *list);



// function that returns the ref
void getRef(char *src, char *dest);

// function that loops through a list of events and  returns true is match is found

bool checkIndiEvents(char *str);

bool checkFamEvents(char *str);






// function that will aprse the header and will return an error if necessary;
GEDCOMerror parseHeader(char **myArray, int *counter, Header *header);

GEDCOMerror parseIndividual(char **myArray, int *counter, Individual *individual);

GEDCOMerror parseFamily(char **myArray, int *counter, Family *family, ListRef *myList);

GEDCOMerror parseSubmitter(char **myArray, int *counter, Submitter *submitter);

void linker(ListRef *myList);

//myfgets(char *dst, int max, FILE *fp);

bool cmp(const void *first, const void *second);

char *
myfgets(char *dst, int max, FILE *fp);

void getFam( Individual *child, List* descList);

Individual *copy(void *toBeCopied);



//******************************************************************************************
//  ASS 2 FUNCTIONS

char *encodingToStr(CharSet encoding);
List WrappergetDescendantListN(const GEDCOMobject* familyRecord, const Individual* person, unsigned int maxGen);
List WrappergetAncestorListN(const GEDCOMobject* familyRecord, const Individual* person, int maxGen);


//*************************************************************************
// tracker functions to keep track of levels;
typedef struct{
    Individual *individual;
    int level;
}Tracker;
void deleteTracker(void *toBeDeleted);
Tracker *createTracker();
char *printTracker(void *toBePrinted);
int compareTracker(const void *first, const void *second);
//*********************************************************************************
// end of tracker functions;

//****************************************************************
// custom compare function for the gedcom list;

int customCompare(const void *first, const void *second);

void deleteMotherList(void *toBeDeleted);

void parth(void *toBeDeleted);

char *random();

// need to use gListToJson;
// List gList, from the obj..

char* returnIndiList();

void createSimpleGedcom(char *source, float version, char* encoding, char *filename, char *subName, char *subAddr);

void addAnIndividual(char *firstName, char *surname, char *filename);

char *returnFileLogData(char *filename);
char *returnDescList(char *filename, char *firstName, char *surname, char *maxGen);
char *returnAncList(char *filename, char *firstName, char *surname, char *maxGen);

char *ObjectToJSON(char *filename);

#endif 
