/**
 * Author:  Abhishek Jhoree
 * ID       0986820
 * CIS*2750 A1
 * GEDCOM_parser
 * */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "GEDCOMparser.h"
#include "GEDCOMutilities.h"
#include "LinkedListAPI.h"
List list2;

List TrackerList;
List TrackerList2;
 

/** Function to create a GEDCOM object based on the contents of an GEDCOM file.
 *@pre File name cannot be an empty string or NULL.  File name must have the .ged extension.
 File represented by this name must exist and must be readable.
 *@post Either:
 A valid GEDCOM has been created, its address was stored in the variable obj, and OK was returned
 or
 An error occurred, the GEDCOM was not created, all temporary memory was freed, obj was set to NULL, and the
 appropriate error code was returned
 *@return the error code indicating success or the error encountered when parsing the GEDCOM
 *@param fileName - a string containing the name of the GEDCOM file
 *@param a double pointer to a GEDCOMobject struct that needs to be allocated
 **/


GEDCOMerror createGEDCOM(char* fileName, GEDCOMobject** obj){
    
    *obj = NULL;

    FILE *fptr;
    bool subFlag = false;

   printf("filename passed in is  %s", fileName);
   
    if(fileName == NULL){
        return createINV_FILE();
    }

    if(strlen(fileName) == 0){
        return createINV_FILE();
    }

    if(strstr(fileName, ".ged") == 0){
        return createINV_FILE();
    }
   
    fptr = fopen(fileName, "r");
    if(fptr == NULL){
        printf("Cannot open the file\n");
        return createINV_FILE();
    }

     GEDCOMobject *object = createObject();
     ListRef *myList = createListRef();
    
    int line_counter;
    line_counter = 0;

    char *buffer;
    buffer = malloc(sizeof(char) * 1000);

    

    while(myfgets(buffer, 300, fptr)){

        if(strlen(buffer) > 255){
            return createINV_RECORD(line_counter + 1);
        }
        
        line_counter++;
        
    }
    fclose(fptr);
    
    line_counter++;
    
    char **myArray = calloc(line_counter + 10, sizeof(char *));
    FILE *f = fopen(fileName, "r");
    
    if(f == NULL){
        printf("Invalid filename\n");
        return createINV_FILE();
    }
    int *counter = calloc(1, sizeof(int));
    
    *counter = 0;

    char *line = calloc(1000, sizeof(char));
    while(myfgets(line, 300, f)){
        
        line[strcspn(line, "\r\n")] = '\0';
        myArray[*counter] = malloc(sizeof(char) * strlen(line) + 9);
        strncpy(myArray[*counter], line, strlen(line) + 2);
        *counter = *counter + 1;
    }
    
    
        fclose(f);
    
    
    // for(int i = 0; myArray[i] != NULL; i++){
    //     //printf("%d %s\n",i, myArray[i]);
    // }
    
   // printf("%d\n", *counter);
    //fclose(f);
    /*
    if(f != NULL){
        
   
    }
*/
    
 // position that will be used for manipulating the array
    int *posn = calloc(1, sizeof(int));
    *posn = 0;

    // dummy string for the purpose of strtok;;
    char *dummy_string = calloc(1000,1);

    // check for the presence of a header;
    strncpy(dummy_string, myArray[0], strlen(myArray[0]) + 2);
   // printf("The dummy string iis %s\n", dummy_string);
    if(dummy_string[0] != '0'){
        return createINV_HEADER(-1);
    }
    else{
        char *p;
        p = strtok(dummy_string, " ");
        p = strtok(NULL, " ");
        if(p == NULL){
            return createINV_GEDCOM();
        }
        else{
            if(strstr(p, "HEAD") == 0){
                return createINV_GEDCOM();
            }
        }
    }

    /*
    if(myArray[*counter - 1] == NULL){
        *counter = *counter - 1;
    }
    */
    strncpy(dummy_string, myArray[*counter-1], strlen(myArray[*counter - 1]) + 2);
    if(dummy_string[0] != '0'){

        return createINV_GEDCOM();
    }
    else{
        if(dummy_string[1] != ' '){
            return createINV_GEDCOM();
        }
        char *p = strtok(dummy_string, " ");
        p = strtok(NULL, " ");
        if(p == NULL){
            return createINV_GEDCOM();
        }
        if(strcmp(p, "TRLR") != 0){
            return createINV_GEDCOM();
        }
    }
    
    // end of check for missing trlr

    
    Header *header = createHeader();
    for(*posn = 0; *posn < *counter; *posn = *posn + 1){
        strncpy(dummy_string, myArray[*posn], strlen(myArray[*posn]) + 2);
       
        char *p;
        p = strtok(dummy_string, " ");
        p = strtok(NULL, " ");
        
        // p should now have a tag;
        if(strcmp(p, "HEAD") == 0){
           // printf("In here\n");
            GEDCOMerror e;
            e = parseHeader(myArray, posn, header);
            if(e.type != OK){
                return e;
            }
            *posn = *posn - 1;
            object->header = header;
        }
        else if(checkForTags(p) == true){
            char *ref = calloc(100, sizeof(char));
            getRef(p, ref);

            p = strtok(NULL, " ");
            if(p == NULL){
                return createINV_GEDCOM();
            }
            if(strstr(p, "INDI") != 0){
                IndiRef *myRef = createIndiRef();
               
                setIndiRefRef(myRef, ref);
                
                Individual *individual = createIndividual();
                parseIndividual(myArray, posn, individual);
                insertBack(&object->individuals, individual);
               
                myRef->individual = individual;
                insertBack(&myList->IndiRef, myRef);
                free(ref);
                *posn = *posn - 1;
            }
            else if(strstr(p, "FAM") != 0){
               
                Family *family = createFamily();
                parseFamily(myArray, posn, family, myList);
                insertFamilyInObject(object, family);
                free(ref);
                *posn = *posn - 1;
            }
            else if(strcmp(p, "SUBM") == 0){
                Submitter *submitter = createSubmitter();
                GEDCOMerror e;
                e = parseSubmitter(myArray, posn, submitter);
                if(e.type != OK){
                    return e;
                }
                subFlag = true;
                //if(submitter->address == NULL){
                //    strcpy(submitter->address,  "");
                //}
                object->header->submitter = submitter;
                object->submitter = submitter;

                *posn = *posn - 1;
                free(ref);
            }
            
           
            
        }

        // perform a level check && call appropriate parsers
    }
    
    linker(myList);
    free(buffer);
    free(dummy_string);
    free(posn);
    free(counter);
    free(line);

    for (int i = 0; i < line_counter+10; i++){
        free(myArray[i]);
    }
    free(myArray);
    
    deleteListRef(myList);
    if(subFlag == false){
        return createINV_GEDCOM();
    }
    
    *obj  = object;
    // Node *indi = object->individuals.head;
    // printf("*************************\n");
    // while(indi != NULL){
    //     Individual *i = indi->data;
    //     printf("%s %s\n", i->givenName, i->surname);
    //     indi = indi->next;
    // }
    // printf("********************************\n");
    // printf("Family Length is %d\n", getLength(object->families));
    // printf("Individual Length is %d\n", getLength(object->individuals));
     list2 = initializeList(printIndividual, deleteIndividual, compareIndividuals);
    
    return createOK();
    
    
}


/** Function to create a string representation of a GEDCOMobject.
 *@pre GEDCOMobject object exists, is not null, and is valid
 *@post GEDCOMobject has not been modified in any way, and a string representing the GEDCOM contents has been created
 *@return a string contaning a humanly readable representation of a GEDCOMobject
 *@param obj - a pointer to a GEDCOMobject struct
 **/
char* printGEDCOM(const GEDCOMobject* obj){
    if(obj == NULL){
        return NULL;
    }
    char *str;
    str = malloc(sizeof(char) * 1000);
    strcpy(str, "     ");
    return str;

}


/** Function to delete all GEDCOM object content and free all the memory.
 *@pre GEDCOM object exists, is not null, and has not been freed
 *@post GEDCOM object had been freed
 *@return none
 *@param obj - a pointer to a GEDCOMobject struct
 **/
void deleteGEDCOM(GEDCOMobject* obj){

    if(obj == NULL){
        return;
    }
    deleteHeader(obj->header);
    clearList(&obj->families);
    clearList(&obj->individuals);
    deleteSubmitter(obj->submitter);
    free(obj);

}



/** Function to "convert" the GEDCOMerror into a humanly redabale string.
 *@return a string contaning a humanly readable representation of the error code
 *@param err - an error struct
 **/
char* printError(GEDCOMerror err){
    char *str;
    str = malloc(100);
    if(err.type == OK){
        sprintf(str, "OK\n");
    }
    if(err.type == INV_FILE){
        sprintf(str, "This is an invalid File\n");
    }
    else if(err.type == INV_GEDCOM){
        sprintf(str, "Invalid GEDCOM\n");
    }
    else if(err.type == INV_HEADER){
        sprintf(str, "Invalid Header\n");
    }

    else if(err.type == INV_RECORD){
        sprintf(str, "Invalid Record\n");
    }
    else if(err.type == OTHER_ERROR){
        sprintf(str, "Other Error\n");
    }
    
    return str;
}




/** Function that searches for an individual in the list using a comparator function.
 * If an individual is found, a pointer to the Individual record
 * Returns NULL if the individual is not found.
 *@pre GEDCOM object exists,is not NULL, and is valid.  Comparator function has been provided.
 *@post GEDCOM object remains unchanged.
 *@return The Individual record associated with the person that matches the search criteria.  If the Individual record is not found, return NULL.
 *If multiple records match the search criteria, return the first one.
 *@param familyRecord - a pointer to a GEDCOMobject struct
 *@param compare - a pointer to comparator fuction for customizing the search
 *@param person - a pointer to search data, which contains seach criteria
 *Note: while the arguments of compare() and person are all void, it is assumed that records they point to are
 *      all of the same type - just like arguments to the compare() function in the List struct
 **/
Individual* findPerson(const GEDCOMobject* familyRecord, 
bool (*compare)(const void* first, const void* second), const void* person){

    if(familyRecord == NULL){
        return NULL;
    }

    Node *temp = familyRecord->individuals.head;
    while(temp != NULL){
        if(compareIndividuals(temp->data, person) == 0){
            return (Individual*)temp->data;
        }
        temp = temp->next;
    }
    return NULL;

}




/** Function to return a list of all descendants of an individual in a GEDCOM
 *@pre GEDCOM object exists, is not null, and is valid
 *@post GEDCOM object has not been modified in any way, and a list of descendants has been created
 *@return a list of descendants.  The list may be empty.  All list members must be of type Individual, and can appear in any order.
 *All list members must be COPIES of the Individual records in the GEDCOM file.  If the returned list is freed, the original GEDCOM
 *must remain unaffected.
 *@param familyRecord - a pointer to a GEDCOMobject struct
 *@param person - the Individual record whose descendants we want
 **/

 
List getDescendants(const GEDCOMobject* familyRecord, const Individual* person){

    if(person == NULL || familyRecord == NULL){
        return  list2;
    }
    Node *temp = person->families.head;
    while(temp != NULL){
        if( ((Family*)(temp->data))->wife == person || ((Family*)(temp->data))->husband  == person ){
            Node *temp2  = ((Family*)temp->data)->children.head;
            while(temp2 != NULL){
               // Individual *i = temp2->data;
               // printf("%s %s\n", i->givenName, i->surname);
                insertBack(&list2, ((Individual*)(temp2->data)) );
                
                getDescendants(familyRecord, ((Individual*)temp2->data) );
                temp2 = temp2->next;
            
            }
            
        }
        temp = temp->next;

        
    }
    // Node *n = list2.head;
    // while(n != NULL){
    //     Individual *in = n->data;
    //    // printf(".........%s %s\n", in->givenName, in->surname);
    //     n = n->next;
    // }
    return list2;
    
}


/**
 * 
 * Functions for events!
 * ------------------------------------------------------------------------
 */
void deleteEvent(void* toBeDeleted){

    if(toBeDeleted == NULL){
        return;
    }

        Event *event;
        event = (Event*)toBeDeleted;

        free(event->date);
        free(event->place);
        clearList(&event->otherFields);
        free(event);
    
}

int compareEvents(const void* first,const void* second){

    // fix ;
    return 0;
}

char* printEvent(void* toBePrinted){

    if(toBePrinted == NULL){
        return NULL;
    }
    char *str;
    Event *event = (Event*)toBePrinted;
    str = calloc(5 + strlen(event->date) + strlen(event->place) + 9, sizeof(char));
    sprintf(str, "%s %s %s", event->type, event->date, event->date);
    return str;

}


/**
 * End of Event Functions!
 * -------------------------------------------------------------------------------
 */



// start of Individual Functions
// -----------------------------------------------------------------------------
void deleteIndividual(void* toBeDeleted){
    if(toBeDeleted == NULL){
        return;
    }
    Individual *indi = (Individual*)toBeDeleted;
    if(indi->givenName != NULL){
        free(indi->givenName);
    }
    if(indi->surname != NULL){
        free(indi->surname);
    }
    clearList(&indi->events);
    clearList(&indi->otherFields);
    clearList(&indi->families);

    free(indi);
}

int compareIndividuals(const void* first,const void* second){
    if(first == NULL || second == NULL){
        return -2;
    }
    Individual *a;
    Individual *b;
    a = (Individual*)first;
    b = (Individual*)second;

    char *str1;
    char *str2;

    str1 = calloc(strlen(a->givenName) + strlen(a->surname) + 9, sizeof(char));
    str2 = calloc(strlen(b->givenName) + strlen(b->surname) + 9, sizeof(char));

    strncpy(str1, "", 2);
    strncpy(str2, "", 2);

    strncat(str1, a->givenName, strlen(a->givenName)+ 2);
    strncat(str1, " ", 2);
    strncat(str1, a->surname, strlen(a->surname) + 2);

    strncat(str2, b->givenName, strlen(b->givenName) + 2);
    strncat(str2, " ", 2);
    strncat(str2, b->surname, strlen(b->surname) + 2);

    if(strcmp(str1, str2) == 0){
        free(str1);
        free(str2);
        return 0;
    }
    else if(strcmp(str1, str2) > 0){
        free(str1);
        free(str2);
        return 1;
    }
    else{
        free(str1);
        free(str2);
        return -1;
    }
    

}

char* printIndividual(void* toBePrinted){
    Individual *Indi;
	if(toBePrinted == NULL){
		return NULL;

	}
	Indi = (Individual*)toBePrinted;

	//int length;

	//length = strlen(Indi->givenName) + strlen(Indi->surname);
	
		//length +=  strlen(toString(Indi->events));
	
	
		//length += strlen(toString(Indi->families));
	
	
		//length += strlen(toString(Indi->otherFields));
	

	char *str;
	str = malloc(sizeof(char) *1000 );
    if(Indi->givenName != NULL){
        strcat(str, Indi->givenName);
    }
    if(Indi->surname != NULL){
        strcat(str, Indi->surname);
    }
	//sprintf("str, %s\t%s\n", Indi->givenName, Indi->surname);

	return str;

}

// End of Individual Functions !!
// -----------------------------------------------------------------------



// Start of Family Functions!
// -----------------------------------------------------------------------------
void deleteFamily(void* toBeDeleted){

    if(toBeDeleted == NULL){
        return;
    }
    Family *family = (Family*)toBeDeleted;
    //deleteIndividual(family->wife);
    //deleteIndividual(family->husband);
    clearList(&family->children);
    clearList(&family->events);
    clearList(&family->otherFields);

    free(family);

}


int compareFamilies(const void* first,const void* second){

    return 0;

}


char* printFamily(void* toBePrinted){

    if(toBePrinted == NULL){
        return NULL;
    }
    Family *f = (Family*)toBePrinted;
    char *str;
    str = calloc(strlen(f->wife->givenName) + strlen(f->wife->surname) 
    + strlen(f->husband->givenName) + strlen(f->husband->surname) +
    strlen(toString(f->children)) + 500, sizeof(char));

    strncat(str, "WIFE\n", 5);
    strncat(str, f->wife->givenName, (strlen(f->wife->givenName) + 2));
    strncat(str, " ", 2);
    strncat(str, f->wife->surname, 100 );

    strncat(str, "\n", 1);
    strncat(str, "HUSB",  5);
    strncat(str, f->husband->givenName, 100);
    strncat(str, " ", 2);
    strncat(str, f->husband->surname, 100);

    strncat(str, "\nCHIL\n", 6);
    strncat(str, toString(f->children), strlen(toString(f->children)));

    

    return str;

}

// End of the family functions !!
// -------------------------------------------------------------------------------




// start of the Field Functions !
// *************************************************************************************

void deleteField(void* toBeDeleted){

    if(toBeDeleted == NULL){
        return;
    }
    else{
        Field * field = (Field*)toBeDeleted;
        free(field->tag);
        free(field->value);
        free(field);
    }

}


int compareFields(const void* first,const void* second){

    if(first == NULL || second == NULL){
        return -2;
    }

    Field *a = (Field*)first;
    Field *b = (Field*)second;

    char *str1 = calloc(strlen(a->tag) + strlen(a->value) + 9, sizeof(char));
    char *str2 = calloc(strlen(a->tag) + strlen(a->value) + 9, sizeof(char));

    strncpy(str1, a->tag, strlen(a->tag) + 2);
    strncat(str1, " ", 2);
    strncpy(str1, a->value, strlen(a->value) + 2);

    strncpy(str2, b->tag, strlen(b->tag) + 2);
    strncpy(str2, b->value, strlen(b->tag) + 2);

    if(strcmp(str1, str2) == 0){
        return 0;
    }
    else if(strcmp(str1, str2) > 0){
        return 1;
    }
    else if(strcmp(str1, str2) < 0){
        return -1;
    }

    return -2;

}


char* printField(void* toBePrinted){

    if(toBePrinted == NULL){
        return NULL;
    }
    Field *field = (Field*)toBePrinted;
    char *str;
    str = calloc(strlen(field->tag) + strlen(field->value) + 9, sizeof(char));

    sprintf(str, "%s %s", field->tag, field->value);

    return str;
 
}

// End of the Field Functions !!
// *******************************************************************************



char *myfgets(char *dst, int max, FILE *fp){
	int c;
	char *p;
    int temp;

    long int x = 0;
    

	/* get max bytes or upto a newline */

	for (p = dst, max--; max > 0; max--) {
		if ((c = fgetc (fp)) == EOF )
			break;
		*p++ = c;
       
        x = ftell(fp);
        temp = fgetc(fp); 

        if(temp == EOF){
            break;
        }
        
		if (c == '\n' && temp != '\r'){
            fseek(fp, x, SEEK_SET);
            break;
            
        }
        if(c  == '\r' && temp != '\n'){
            fseek(fp, x, SEEK_SET);
            break;
           
        }
       

        fseek(fp, x, SEEK_SET);
            
	}
	*p = 0;
	if (p == dst || c == EOF)
		return NULL;
	return (p);
}


// ****************************** A2 functions ******************************

/** Function to writing a GEDCOMobject into a file in GEDCOM format.
 *@pre GEDCOMobject object exists, is not null, and is valid
 *@post GEDCOMobject has not been modified in any way, and a file representing the
 GEDCOMobject contents in GEDCOM format has been created
 *@return the error code indicating success or the error encountered when parsing the calendar
 *@param obj - a pointer to a GEDCOMobject struct
 **/
GEDCOMerror writeGEDCOM(char* fileName, const GEDCOMobject* obj){

    //
    
    if(fileName == NULL){
        GEDCOMerror a;
        a.type = WRITE_ERROR;
        a.line = -1;
        return a;
        //return createINV_FILE();
    }
    if(obj == NULL){
        GEDCOMerror a;
        a.type = WRITE_ERROR;
        a.line = -1;
        return a;
        //return createINV_GEDCOM();
    }
    if(strlen(fileName) == 0){
        return createINV_FILE();
    }
    if(strstr(fileName, ".ged") == 0){
        return createINV_FILE();
    }

    validateGEDCOM(obj);
    FILE *fptr;
    fptr = fopen(fileName, "w+");
    Header *header = obj->header;

    fprintf(fptr, "0 HEAD\n");
    fprintf(fptr, "1 SOUR %s\n", header->source);
    /*
    Node *n = header->otherFields.head;
    while(n != NULL){
        Field *f = n->data;
        fprintf(fptr, "2 %s %s\n",  f->tag, f->value);
        n = n->next;
    }
    */
    fprintf(fptr, "1 GEDC\n");
    fprintf(fptr, "2 VERS %.1lf\n", header->gedcVersion);
    char *encoding = encodingToStr(header->encoding);
    fprintf(fptr, "1 CHAR %s\n", encoding);
    free(encoding);
    fprintf(fptr, "1 SUBM @SUB1@\n");

    fprintf(fptr, "0 @SUB1@ SUBM\n");
    Submitter *submitter = obj->submitter;
    fprintf(fptr, "1 NAME %s\n", submitter->submitterName);
   // if(submitter->address != NULL && strlen(submitter->address) != 0){
        fprintf(fptr, "1 ADDR %s\n", submitter->address);
   // }

    // individuals;
    ListRef *myRefs = createListRef();

    // get the famrefs;
    int fam_counter = 1;
    Node *fam_node1 = obj->families.head;
    while(fam_node1 != NULL){
        Family *fam = fam_node1->data;
        FamRef *famref = createFamRef();
        char *str = calloc(1000, sizeof(char));
        sprintf(str, "@F%d@", fam_counter);
        setFamRefRef(famref, str);
        famref->family = fam;
        insertBack(&myRefs->FamRef, famref);
        fam_node1 = fam_node1->next;
        fam_counter++;
        free(str);
    }

    int counter = 1;
    Node *indi_node = obj->individuals.head;
    while(indi_node != NULL){
        Individual *indi = indi_node->data;
        IndiRef  *indiref = createIndiRef();
        indiref->ref = calloc(250000, sizeof(char));
        indiref->individual = indi;
        sprintf(indiref->ref, "@I%d@", counter);

        fprintf(fptr, "0 %s INDI\n", indiref->ref);
        fprintf(fptr, "1 NAME %s /%s/\n",indi->givenName, indi->surname );
        fprintf(fptr, "2 GIVN %s\n", indi->givenName);
        fprintf(fptr, "2 SURN %s\n", indi->surname);
        
        Node *otherFields_node = indi->otherFields.head;
        while(otherFields_node != NULL){
            Field *field = otherFields_node->data;
            if(strcmp(field->tag,  "FAMC") != 0 && strcmp(field->tag, "FAMS") != 0){
                fprintf(fptr, "1 %s %s\n", field->tag, field->value);
            }
            otherFields_node = otherFields_node->next;
        }
        
        
        Node *events_node = indi->events.head;
        while(events_node != NULL){
            Event *event = events_node->data;
            
            fprintf(fptr, "1 %s\n",event->type);
        
               if(event->place != NULL){
                    fprintf(fptr, "2 PLAC %s\n", event->place);
               }
           
                if(event->date != NULL){
                    fprintf(fptr, "2 DATE %s\n", event->date);
                }
           

            events_node = events_node->next;
        }

        // famc fams tags now
        Node *famref_node = myRefs->FamRef.head;
        while(famref_node != NULL){
            FamRef *famref = famref_node->data;
            Family *family = famref->family;
            if(family->husband == indi ){
                fprintf(fptr, "1 FAMS %s\n", famref->ref);
                //break;
            }
            else if(family->wife == indi){
                fprintf(fptr, "1 FAMS %s\n", famref->ref);
                //break;
            }

            Node *chil_node = family->children.head;
                while(chil_node != NULL){
                    Individual *child_indi = chil_node->data;
                    if(child_indi == indi){
                        fprintf(fptr, "1 FAMC %s\n", famref->ref);
                    }
                    
                    chil_node = chil_node->next;
                }
           
            famref_node = famref_node->next;
        }
        

        indi_node = indi_node->next;
        counter++;
        insertBack(&myRefs->IndiRef, indiref);
    }

    // families now;
    Node *fam_node = obj->families.head;
    Node *temp = myRefs->FamRef.head;
    while(fam_node != NULL){
        FamRef *famref = temp->data;
        fprintf(fptr, "0 %s FAM\n", famref->ref);
        Family *family  = fam_node->data;
        // write the wife;, loop through the list and find her LOL;
        Node *node = myRefs->IndiRef.head;
        while(node != NULL){
            //Individual *refIndi = node->data;
            IndiRef *indi_ref = node->data;
            if(compareIndividuals(indi_ref->individual, family->wife) == 0){
                fprintf(fptr, "1 WIFE %s\n", indi_ref->ref);
                break;
            }
            node = node->next;
            
        }

        node = myRefs->IndiRef.head;
        // write the husband now
        while(node != NULL){
            IndiRef *indi_ref = node->data;
            if(compareIndividuals(indi_ref->individual, family->husband) == 0){
                fprintf(fptr, "1 HUSB %s\n", indi_ref->ref);
                break;
            }
            node = node->next;
        }
        // events now;
        Node *event_node = family->events.head;
        while(event_node != NULL){
            Event *event = event_node->data;
            fprintf(fptr, "1 %s\n", event->type);
            if(event->date != NULL){
                if(strlen(event->date) > 2)
                fprintf(fptr, "2 %s\n", event->date);
            }
            if(event->place != NULL){
                if(strlen(event->place) > 2)
                fprintf(fptr, "2 %s\n", event->place);
            }
            event_node = event_node->next;
        }
        Node *other_fields_node = family->otherFields.head;
        while(other_fields_node != NULL){
            Field *f = other_fields_node->data;
            fprintf(fptr, "2 %s %s\n", f->tag, f->value);

            other_fields_node = other_fields_node->next;
        }
        node = myRefs->IndiRef.head;
        Node *child_node = family->children.head;
        while(child_node != NULL){
            Individual *child = child_node->data;
            while(node != NULL){
                IndiRef *indi_ref = node->data;
                if(compareIndividuals(indi_ref->individual, child) == 0){
                    fprintf(fptr, "1 CHIL %s\n",  indi_ref->ref);
                    break;
                }
                node = node->next;
            }
            child_node = child_node->next;
            node = myRefs->IndiRef.head;
        }



        fam_node = fam_node->next;
        temp = temp->next;
    }
    fprintf(fptr, "0 TRLR");
    deleteListRef(myRefs);
    fclose(fptr);
    return createOK();

}

/** Function for validating an existing GEDCOM object
 *@pre GEDCOM object exists and is not null
 *@post GEDCOM object has not been modified in any way
 *@return the error code indicating success or the error encountered when validating the GEDCOM
 *@param obj - a pointer to a GEDCOMobject struct
 **/
ErrorCode validateGEDCOM(const GEDCOMobject* obj){
    if(obj == NULL){
            return INV_GEDCOM;
    }
    if(obj->header == NULL){
        return INV_GEDCOM;
    }
    if(obj->submitter == NULL){
        return INV_GEDCOM;
    }

    //  check for the header now;
    Header *header = obj->header;
    if(header->submitter == NULL){
        return INV_HEADER;
    }
    if(header->source  == NULL || strlen(header->source) == 0){
        return INV_HEADER;
    }
    if(strlen(header->submitter->submitterName) == 0){
        return INV_RECORD;
    }
    if(header->gedcVersion < 0){
        return INV_HEADER;
    }
    // if(header->gedcVersion != 5.5 && header->gedcVersion != 5.51){
    //     return INV_HEADER;
    // }

    // check the encoding;
    if(header->encoding != ANSEL && header->encoding != ASCII && header->encoding != UTF8
    && header->encoding != UNICODE){
        return INV_HEADER;
    }

    if(header->submitter == NULL){
        return INV_HEADER;
    }

    // check the submitter record now;
    Submitter *submitter = obj->submitter;
    if(submitter->submitterName == NULL){
        return INV_RECORD;
    }
    if(strlen(submitter->submitterName) == 0){
        return INV_RECORD;
    }
    if(strlen(submitter->submitterName) > 61){
        return INV_RECORD;
    }
    if(submitter->address != NULL){
        if(strlen(submitter->address) > 200){
            return INV_RECORD;
        }
    }


    // check the indiviiduals now;
    Node *indi_node = obj->individuals.head;
    while(indi_node != NULL){
        Individual *indi = indi_node->data;
        if(indi->givenName != NULL){
            if(strlen(indi->givenName) > 200){

                return INV_RECORD;
            }
        }
        if(indi->surname != NULL){
            if(strlen(indi->surname) > 200 ){

                return INV_RECORD;
            }
        }
        Node *event_node = indi->events.head;
        while(event_node != NULL){
            Event *event = event_node->data;
            if(event->type == NULL){
                return INV_RECORD;
            }
            if(strlen(event->type) > 5){
                return INV_RECORD;
            }
            if(event->date != NULL){
                if(strlen(event->date)  > 200){
                    return INV_RECORD;
                }
            }
            if(event->place != NULL){
                if(strlen(event->place) > 200){
                    return INV_RECORD;
                }
            }
            event_node = event_node->next;
        }
        Node *otherFields_node = indi->otherFields.head;
        while(otherFields_node != NULL){

            Field *field = otherFields_node->data;
            if(field->tag != NULL){
                if(strlen(field->tag) > 200){

                    return INV_RECORD;
                }
            }
            if(field->value != NULL){
                if(strlen(field->value) > 200){

                    return INV_RECORD;
                }
            }
            otherFields_node = otherFields_node->next;
        }
        Node *family_node = indi->families.head;
        while(family_node != NULL){
            Family *family = family_node->data;
            if(family == NULL && family_node != NULL){

                return INV_RECORD;
            }

            family_node = family_node->next;
        }

        indi_node = indi_node->next;
    }

    // check the families now;
    Node *family_node = obj->families.head;
    while(family_node !=  NULL){

        Family *family = family_node->data;
        Node *child_node = family->children.head;
        while(child_node != NULL){
            Individual *indi = child_node->data;
            if(indi == NULL && child_node != NULL){

                return INV_RECORD;
            }
            child_node = child_node->next;
        }

        // family events;
        Node *event_node = family->events.head;
        while(event_node != NULL){

            Event *event = event_node->data;
            if(event->type != NULL){
                if(strlen(event->type) >  5){
                    return INV_RECORD;
                }
            }
            if(event->place != NULL){
                if(strlen(event->place) > 200){

                    return INV_RECORD;
                }
            }
            if(event->date != NULL){
                if(strlen(event->date) > 200){

                    return INV_RECORD;
                }
            }
            event_node = event_node->next;
        }

        family_node = family_node->next;
    }

    return OK;

}

/** Function to return a list of up to N generations of descendants of an individual in a GEDCOM
 *@pre GEDCOM object exists, is not null, and is valid
 *@post GEDCOM object has not been modified in any way, and a list of descendants has been created
 *@return a list of descendants.  The list may be empty.  All list members must be of type List.    *@param familyRecord - a pointer to a GEDCOMobject struct
 *@param person - the Individual record whose descendants we want
 *@param maxGen - maximum number of generations to examine (must be >= 1)
 **/
List getDescendantListN(const GEDCOMobject* familyRecord, const Individual* person, unsigned int maxGen){
TrackerList = initializeList(printTracker, deleteTracker, compareTracker);
if(maxGen == 0){
    maxGen = 999;
}
if(maxGen < 0){
    maxGen = 999;
}
List L = WrappergetDescendantListN(familyRecord, person, maxGen);


printf("***** %d", getLength(TrackerList));
printf("***** %d\n", getLength(L));

 Node *node = L.head;

	 int curr_max = -1;
	
	List motherList = initializeList(printTracker, deleteMotherList, compareTracker);

	while(node != NULL){
		Tracker *tr = node->data;
		curr_max = tr->level;
		List *indi = calloc(1, sizeof(List));
        *indi = initializeList(printIndividual, parth, customCompare);
        Individual * i = NULL;
		while(curr_max == tr->level){
			i  = copy(tr->individual);
			insertSorted(indi, i);
            node = node->next;
			if(node != NULL){
                tr = node->data;
                i = tr->individual;
			}
            else{
                break;
            }
            if(curr_max !=  tr->level){
                node = node->previous;
                break;
                
            }
		}
            insertBack(&motherList, indi);
            if(node == NULL){
                break;
            }
			node = node->next;
	}
    clearList(&TrackerList);
    return motherList;

}

List WrappergetDescendantListN(const GEDCOMobject* familyRecord, const Individual* person, unsigned int maxGen){

    if(person == NULL || familyRecord == NULL){
        return  TrackerList;
    }


    Node *temp = person->families.head;
    while(temp != NULL){
        if( ((Family*)(temp->data))->wife == person || ((Family*)(temp->data))->husband  == person ){
            Node *temp2  = ((Family*)temp->data)->children.head;
            while(temp2 != NULL){
                if (maxGen > 0) {
                   Tracker *tracker = createTracker();
                   tracker->individual = temp2->data;
                   tracker->level = maxGen;
                   WrappergetDescendantListN(familyRecord, ((Individual*)temp2->data), maxGen - 1 );
                   insertSorted(&TrackerList, tracker);
                   
                }     
                temp2 = temp2->next;
    
            }
            
        }
        temp = temp->next;     
    }
    
    return TrackerList;

}

/** Function to return a list of up to N generations of ancestors of an individual in a GEDCOM
 *@pre GEDCOM object exists, is not null, and is valid
 *@post GEDCOM object has not been modified in any way, and a list of ancestors has been created
 *@return a list of ancestors.  The list may be empty.
 *@param familyRecord - a pointer to a GEDCOMobject struct
 *@param person - the Individual record whose descendants we want
 *@param maxGen - maximum number of generations to examine (must be >= 1)
 **/
List getAncestorListN(const GEDCOMobject* familyRecord, const Individual* person, int maxGen){
    TrackerList2 = initializeList(printTracker, deleteTracker, compareTracker);

    //
    

    if(maxGen == 0){
        maxGen = 99;
    }
    if(maxGen < 0){
        maxGen = 99;
    }
    List l = WrappergetAncestorListN(familyRecord, person, maxGen);

    Node *t = TrackerList2.head;
    while(t != NULL){
        Tracker *tra = t->data;
        printf("####%s %d\n", tra->individual->givenName, tra->level);
        t = t->next;
    }

    Node *node = l.head;
    

    List motherList = initializeList(printIndividual, deleteMotherList, compareTracker);
    int curr_max = -1;
    while(node != NULL){
		Tracker *tr = node->data;
		curr_max = tr->level;
		List *indi = calloc(1, sizeof(List));
        *indi = initializeList(printIndividual, parth, customCompare);
        Individual * i = NULL;
		while(curr_max == tr->level){
			i  = copy(tr->individual);
			//  check for duplicates;
            bool flag = true;
            if(indi != NULL){
                Node *n = indi->head;
                while(n != NULL){
                    Individual *ind = n->data;
                    if(strcmp(ind->givenName, i->givenName) == 0 && strcmp(ind->surname, i->surname) == 0){
                        flag = false;
                        break;
                    }
                    n = n->next;
                }
            }
            if(flag == true){
                if(strcmp(i->givenName, "Unknown") == 0){
                    strcpy(i->givenName, "");
                }
			 insertSorted(indi, i);
            }
            
            
            node = node->next;
			if(node != NULL){
                tr = node->data;
                i = tr->individual;
			}
            else{
                break;
            }
            if(curr_max !=  tr->level){
                node = node->previous;
                break;
                
            }

		}
            insertBack(&motherList, indi);
       
            if(node == NULL){
                break;
            }
			node = node->next;

	}
    

    clearList(&TrackerList2);
    return motherList;

}
List WrappergetAncestorListN(const GEDCOMobject* familyRecord, const Individual* person, int maxGen){

    if(person == NULL || familyRecord == NULL){
        return  TrackerList2;
    }

   // bool found = false;

    Node *temp = person->families.head;
    while(temp != NULL){
        Family *family = temp->data;
        Node *child_node = family->children.head;

        while(child_node != NULL){
            Individual *indi = child_node->data;
            if(person == indi){
               if(maxGen > 0){
                if(family->husband != NULL){
                     Tracker *tracker = createTracker();
                     tracker->individual = family->husband;
                     tracker->level = maxGen;
                     WrappergetAncestorListN(familyRecord, family->husband, maxGen -1);
                     insertSorted(&TrackerList2, tracker);

                }
                if(family->wife != NULL){
                    Tracker *tracker = createTracker();
                    tracker->individual = family->wife;
                    tracker->level = maxGen;
                    WrappergetAncestorListN(familyRecord, family->wife, maxGen -1);
                    insertSorted(&TrackerList2, tracker);

                }
                
               }

            }
            child_node = child_node->next;
        }
        temp = temp->next;
    }
    
    return TrackerList2;

}

/** Function for converting an Individual struct into a JSON string
 *@pre Individual exists, is not null, and is valid
 *@post Individual has not been modified in any way, and a JSON string has been created
 *@return newly allocated JSON string.  May be NULL.
 *@param ind - a pointer to an Individual struct
 **/
char* indToJSON(const Individual* ind){
    if(ind == NULL){
        char *str;
        str = malloc(100);
        strcpy(str, "");
        return str;
    }
    // check for the validity now;
    char *givenName = calloc(300, sizeof(char));
    char *surname = calloc(300, sizeof(char));
    strncpy(givenName, ind->givenName, strlen(ind->givenName) + 2);
    strncpy(surname, ind->surname, strlen(ind->surname) + 2);

    char *JSONstr = calloc(strlen(surname) + strlen(givenName) + 200, sizeof(char));

    char quote = '"';
    
    sprintf(JSONstr, "{%cgivenName%c:%c%s%c,%csurname%c:%c%s%c}", 
    quote,quote,  quote, givenName,quote, quote, quote, quote, surname, quote);

    //printf("The string is : %s\n",  JSONstr);
    free(surname);
    free(givenName);
    return JSONstr;

}

/** Function for creating an Individual struct from an JSON string
 *@pre String is not null, and is valid
 *@post String has not been modified in any way, and an Individual struct has been created
 *@return a newly allocated Individual struct.  May be NULL.
 *@param str - a pointer to a JSON string
 **/
Individual* JSONtoInd(const char* str){

    Individual *individual = NULL;
    if(str == NULL){

        return individual;
    }
    if(strlen(str) == 0){

        return individual;
    }

    // // check for the validity of the string;

    if(strstr(str, "givenName")  == 0){

        return individual;
    }
    if(strstr(str, "surname") == 0){

        return individual;
    }

    if(str[0] != '{'){

        return individual;
    }
    if(str[1] !=  '"'){

        return individual;
    }

    
    char *copy = calloc(strlen(str) + 9, sizeof(char));
    strncpy(copy, str, strlen(str) + 2);
    individual = createIndividual();
    char quote[3];
	 quote[0] = '"';
	 quote[1] = '{';
     quote[2] = '\0';
	 
	 char *a = strtok(copy, quote);
	 printf("p is now %s\n", a);
	 a = strtok(NULL, ":\"\"");
	 printf("It is  now %s\n", a);
     char *givenName; 
     char *surname; 
     if(a[0] == ','){
         givenName = NULL;
     }
     else{
         givenName =  calloc(999, sizeof(char));
         strncpy(givenName,  a, strlen(a)  + 2);
         setIndividualGivenName(individual, givenName);
         free(givenName);
     }
    

    a = strtok(NULL, ":\"\"");
    a = strtok(NULL, ":\"\"");
    a = strtok(NULL, ":\"\"");
    if(a != NULL){

        printf("a is now %s\n", a);
        if(a[0]  == '}'){
            surname = NULL;
        }
        else{
            surname =  calloc(1000, sizeof(char));
            strncpy(surname, a, strlen(a) + 2);
            setIndividualSurname(individual, surname);
            free(surname);
        }

    }
    free(copy);
    
    return individual;
    
}

/** Function for creating a GEDCOMobject struct from an JSON string
 *@pre String is not null, and is valid
 *@post String has not been modified in any way, and a GEDCOMobject struct has been created
 *@return a newly allocated GEDCOMobject struct.  May be NULL.
 *@param str - a pointer to a JSON string
 **/
GEDCOMobject* JSONtoGEDCOM(const char* str){
// check the string;
GEDCOMobject *object = NULL;

if(str == NULL){
    return object;
}

if(strlen(str) == 0){
    return object;
}

if(str[0] != '{'){
    return object;
}

char quote[3];
	 quote[0] = '"';
	 quote[1] = '{';
     quote[2] = '\0';
    Header *header = createHeader();
     char *copy_string  = calloc(1000, sizeof(char));
     strncpy(copy_string, str, strlen(str) + 2);

     char *a = strtok(copy_string, quote);
     a = strtok(NULL, ":\"\"");
     //a = strtok(NULL, ":\"\"");
     printf("a is  %s\n", a);
     setHeaderSource(header, a);

     a = strtok(NULL, ":\"\"");
     a = strtok(NULL, ":\"\"");
     a = strtok(NULL, ":\"\"");
     a = strtok(NULL, ":\"\"");
    setHeaderGEDCversion(header, a);
   // header->gedcVersion = atof(a);

     a = strtok(NULL, ":\"\"");
     a = strtok(NULL, ":\"\"");
     a = strtok(NULL, ":\"\"");
     setHeaderEncoding(header, a);

    // a = strtok(NULL, ":\"\"");
     
     a = strtok(NULL, ":\"\"");
     a = strtok(NULL, ":\"\"");
    a = strtok(NULL, "\"\"");
    a = strtok(NULL, "\"\"");

    char *name = malloc(100);
    strcpy(name, a);
    a = strtok(NULL, " ");
    strcat(name, " ");
    strcat(name, a);

Submitter *submitter = createSubmitter();
setSubmitterName(submitter, name);

    a = strtok(NULL, ":\"\"");
    a = strtok(NULL, "\"\"");
    a = strtok(NULL, "\"\"");

setSubmitterAddress(submitter, a);



object  = createObject();
object->header = header;
object->submitter = submitter;

    return object;
}   





/** Function for adding an Individual to a GEDCCOMobject
 *@pre both arguments are not NULL and valid
 *@post Individual has not been modified in any way, and its address had been added to GEDCOMobject's individuals list
 *@return void
 *@param obj - a pointer to a GEDCOMobject struct
 *@param toBeAdded - a pointer to an Individual struct
**/
void addIndividual(GEDCOMobject* obj, const Individual* toBeAdded){

    if(toBeAdded == NULL ||  obj == NULL){
        return;
    }
    Individual *individual = (Individual*)toBeAdded;
    insertBack(&obj->individuals, individual);

}

/** Function for converting a list of Individual structs into a JSON string
 *@pre List exists, is not null, and has been initialized
 *@post List has not been modified in any way, and a JSON string has been created
 *@return newly allocated JSON string.  May be NULL.
 *@param iList - a pointer to a list of Individual structs
 **/
char* iListToJSON(List iList){

    if(iList.head == NULL){
        return "[]";
    }

    Node *node = iList.head;
    char *str = calloc(500000, sizeof(char));
    strcpy(str, "");
    strcat(str, "[");
    while(node != NULL){
        Individual *individual = node->data;
        strcat(str, indToJSON(individual));
        strcat(str, ",");
        node = node->next;
    }
    str[strlen(str)- 1] = '\0';
    strcat(str, "]");
    return str;
}

/** Function for converting a list of lists of Individual structs into a JSON string
 *@pre List exists, is not null, and has been initialized
 *@post List has not been modified in any way, and a JSON string has been created
 *@return newly allocated JSON string.  May be NULL.
 *@param gList - a pointer to a list of lists of Individual structs
 **/
char* gListToJSON(List gList){

    if(gList.length == 0){
        return "[]";
    }
    char *str = calloc(500000, sizeof(char));
    Node *node = gList.head;
    strcpy(str, "[");
    while(node != NULL){
        List *Tchalla = node->data;
        strcat(str, iListToJSON(*Tchalla));
        strcat(str, ",");
        node = node->next;
    }
    str[strlen(str)- 1] = '\0';
    strcat(str, "]");
    return str;

}


//****************************************** List helper functions added for A2 *******************************************
void deleteGeneration(void* toBeDeleted){
    if(toBeDeleted == NULL){
        return;
    }

}
int compareGenerations(const void* first,const void* second){

    if(first == NULL || second == NULL){

        return -2;
    }

    return 1;

}
char* printGeneration(void* toBePrinted){

    char *str = calloc(1000, sizeof(char));

    return str;

}
