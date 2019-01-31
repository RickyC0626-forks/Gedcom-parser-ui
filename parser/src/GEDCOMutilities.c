#include "GEDCOMutilities.h"
#include "LinkedListAPI.h"
#include "GEDCOMparser.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

char * eventTags[] = { "BIRT", "CHR", "ADOP", "DEAT", "BURI", "CREM",
"BAPM", "BARM", "BASM", "BLES", "CHRA", "CONF", "FCOM", "ORDN", "NATU", "EMIG",
	"EMIG", "IMMI", "CENS", "PROB", "WILL", "GRAD", "RETI", "EVEN" };


    char *famTags[] = {"ANUL","CENS","DIV","DIVF","ENGA","MARB","MARC","MARR",
					"MARL","MARS"};

                    


GEDCOMerror parseHeader(char **myArray, int *counter, Header *header){
    int *line_check = calloc(1, sizeof(int));
    *line_check = 0;
    //increase the counter by 1;
    *counter = *counter + 1;

    bool sour_flag = false;
    bool vers_flag = false;
    bool encod_flag = false;
    bool subm_flag = false;
          
   // printf("********* IN HEAD PARSER**********\n");

    while(myArray[*counter][0] != '0'){
        
        char *line;  
        line = calloc(strlen(myArray[*counter]) + 9, sizeof(char));
        strncpy(line, myArray[*counter], strlen(myArray[*counter]) + 2);
     //   printf("In parse header: %s\n", line);

        char *p;
        p = strtok(line, " ");
      //  printf("after the tok, p is %s\n", p);
      //  printf("atoi p is %d\n", atoi(p));
       
        int mod = atoi(p) - *line_check;
        if(mod < 0){
            mod = mod * -1;
        }
        if(mod > 1){
            return createINV_RECORD(*counter + 1);
        }
        *line_check = atoi(p);
        
             
        p = strtok(NULL, " ");
        if(p == NULL){
            return createINV_HEADER(*counter + 1);
        }

        if(strcmp(p, "SOUR") == 0){
            char *sour = calloc(256, sizeof(char));
            strncpy(sour, "", 1);
          //  printf("in sour\n");
            p = strtok(NULL, " ");
            strncat(sour, p, strlen(p) +  2 );
            if(p == NULL){
                // return an error;
            }
            while(p != NULL){
                p = strtok(NULL, " ");
                if(p != NULL){
                    strncat(sour, " ", 1);
                    strncat(sour, p, strlen(p) + 2);
                }
                
            }
            if(strlen(sour) > 249){
                // return an error
            }
            if(isspace(sour[strlen(sour) - 1])){
                sour[strlen(sour) - 1] = '\0';
            }
            setHeaderSource(header, sour);
           // printf("After set, the source is %s\n", header->source);
            sour_flag = true;
            free(sour);
        }
        else if(strcmp(p, "GEDC") == 0){
            char *line2 = calloc(256, sizeof(char));
            *counter = *counter + 1;
            strncpy(line2, myArray[*counter], strlen(myArray[*counter]) + 2);
           // printf("Line 2 has %s\n", line2);
            char *a = strtok(line2, " ");
            int mod2 = *line_check - atoi(a);
            if(mod2 < 0){
                mod2 =  mod2 * -1;
            }
            if(mod2 >  1){
                return createINV_RECORD(*counter + 1);
            }
            
            
            a = strtok(NULL, " ");
            if(a == NULL){
                return createINV_HEADER(*counter + 1);
            }
            if(strstr(a, "VERS") == 0){
                return createINV_HEADER(*counter + 1);
            }

            a = strtok(NULL, " ");
            if(a == NULL){
                return createINV_HEADER(*counter + 1);
            }

           // if(strstr(a, "5.5") != 0 || strstr(a, "5.5.1") != 0){
                setHeaderGEDCversion(header, a);
               // printf("afetr the set %lf\n", header->gedcVersion);
           // }
            free(line2);
            vers_flag = true;
        }

        else if(strcmp(p, "CHAR") == 0){
            p = strtok(NULL, " ");
            if(p == NULL){
                return createINV_HEADER(*counter + 1);
            }
            setHeaderEncoding(header, p);
            encod_flag = true;
        }
        else if(strcmp(p, "SUBM") == 0){
            //Field *field;
            //field = createField();
            //setFieldTag(field, p);
            //p = strtok(NULL, " ");
            //setFieldValue(field, p);
           // insertBack(&header->otherFields, field);
            subm_flag = true;
        }

        else{
            char *tempTag = calloc(strlen(p) + 9, sizeof(char));
            strncpy(tempTag, p, strlen(p) + 2);

            
            char *tempValue = calloc(260, sizeof(char));
            strncpy(tempValue, "", 2);
            p = strtok(NULL, " ");
            if(p == NULL){
                free(tempTag);
                free(tempValue);
                return createINV_HEADER(*counter + 1);
            }
            while(p != NULL){
                strncat(tempValue, p, strlen(p) + 2);
                strncat(tempValue, " ", 2);
                p = strtok(NULL, " ");
            }
            if(isspace(tempTag[strlen(tempTag) -1])){
                tempTag[strlen(tempTag) - 1] = '\0';
            }
            if(isspace(tempValue[strlen(tempValue) -1])){
                tempValue[strlen(tempValue) - 1] = '\0';
            }
            setHeaderOtherFields(header, tempTag, tempValue);
            free(tempTag);
            free(tempValue);
            
        }
        

        *counter = *counter + 1;
        free(line);
 
    }
    

    if(sour_flag == false || encod_flag == false || sour_flag == false || 
    subm_flag == false || vers_flag == false){
        return createINV_HEADER(-1);
    }
    free(line_check); //moved here;
    return createOK();
}


GEDCOMerror parseIndividual(char **myArray, int *counter, Individual *individual){

    *counter = *counter + 1;
    while(myArray[*counter][0] != '0'){
        char *str;
        str = calloc(256, sizeof(char));
        strncpy(str, myArray[*counter], strlen(myArray[*counter]) + 2);

        char *p = strtok(str, " ");
        if(p == NULL){
            return createINV_RECORD(*counter + 1);
        }
        p = strtok(NULL, " ");
        if(p == NULL){
            return createINV_RECORD(*counter + 1);
        }
        if(strcmp(p, "NAME") == 0){
            p = strtok(NULL, " /");

            if(p != NULL){
                if(isspace(p[strlen(p) - 1])){
                    p[strlen(p) - 1] = '\0';
                }
            }
           
                setIndividualGivenName(individual, p);
           
            p = strtok(NULL, " /");
            if(p != NULL){
                if(isspace(p[strlen(p) - 1])){
                    p[strlen(p) - 1] = '\0';
                }
            }
        
                setIndividualSurname(individual, p);

        }
        else if(strcmp(p, "SURN")  == 0){
            p = strtok(NULL, " ");
            if(p != NULL){
                if(isspace(p[strlen(p) - 1])){
                    p[strlen(p) - 1] = '\0';
                }
            }
            
            setIndividualSurname(individual, p);
            
        }
        else if(strcmp(p, "GIVN") == 0){
            p = strtok(NULL, " ");
            //char *temp = calloc(100, sizeof(char));
            //strcpy(temp, p);
            // while(p != NULL){
            //     strcat(temp, p);
            //     strcat(temp, " ");
            //     p = strtok(NULL, " ");
            // }
            if(p != NULL){
                if(isspace(p[strlen(p) - 1])){
                    p[strlen(p) - 1] = '\0';
                }
            }
                
                setIndividualGivenName(individual, p);
                //free(temp);
            
        }
        else if(checkIndiEvents(p) == true){
           // printf("IN AN EVENT: %s\n", p);
            Event *event = createEvent();
            if(isspace(p[strlen(p) - 1])){
                p[strlen(p) -1 ] = '\0';
            }
            strcpy(event->type, p);
           // printf("Copied %s\n",  event->type);
            //setEventType(event, p);
            *counter = *counter + 1;
            
            while(myArray[*counter][0] != '1'){

                char *temp = calloc(256, sizeof(char));
                strncpy(temp, myArray[*counter], strlen(myArray[*counter]) + 2);
                char *e = strtok(temp, " ");
                e = strtok(NULL, " ");
   
                if(strstr(e, "DATE") != 0){
                    char *date = calloc(256, sizeof(char));
                    e = strtok(NULL, " ");
                    while(e != NULL){
                        strncat(date, e, strlen(e) + 2);
                        strncat(date, " ", 2);
                        e = strtok(NULL, " ");
                    }
                    if(isspace(date[strlen(date) - 1])){
                        date[strlen(date) - 1] = '\0';
                    }
                    //setEventDate(event, date);
                    event->date = calloc(strlen(date) + 9, sizeof(char));
                    strcpy(event->date, date);
                   // printf("The date is %s\n", event->date);
                    free(date);
                }
                else if(strcmp(e, "PLAC") == 0){
                    char *plac = calloc(256, sizeof(char));
                    e = strtok(NULL, " ");
                    while(e != NULL){
                        strncat(plac, e, strlen(e) + 2);
                        strncat(plac, " ", 2);
                        e = strtok(NULL, " ");
                    }
                    if(isspace(plac[strlen(plac) - 1])){
                        plac[strlen(plac) - 1] = '\0';
                    }
                    //setEventPlace(event, plac);
                    event->place = calloc(strlen(plac) + 9, sizeof(char));
                    strcpy(event->place, plac);
                  //  printf("The Place is %s\n", event->place);
                    free(plac);
                }
                
                else{
                    if(isspace(e[strlen(e) - 1])){
                        e[strlen(e) - 1] = '\0';
                    }
                    if(strcmp(e, "FAMC") != 0 && strcmp(e, "FAMS") != 0){
                    char *value = calloc(256, sizeof(char));
                    Field *field = createField();
                    setFieldTag(field, e);
                    e = strtok(NULL, " ");
                    if(e == NULL){return createINV_RECORD(*counter + 1);}
                    while(e != NULL){
                        strncat(value, e, strlen(e) + 2);
                        strncat(value, " ", 2);
                        e = strtok(NULL, " ");
                    }
                    if(isspace(value[strlen(value) -  1])){
                        value[strlen(value) - 1] = '\0';
                    }
                    setFieldValue(field, value);
                    insertEventOtherFields(event, field);
                    free(value);
                    }
                    
                }
                
                *counter = *counter + 1;
                free(temp);
            }
            insertBack(&individual->events, event);
            //setIndividualEvent(individual, event);
            *counter = *counter - 1;
        }
        else{
            if(strcmp(p, "FAMC") != 0 && strcmp(p, "FAMS") != 0){
            // must be an other field;
            Field *field = createField();
            setFieldTag(field, p);
            p = strtok(NULL, " ");
            setFieldValue(field, p);
            insertBack(&individual->otherFields, field);
            }
            
        }
        *counter = *counter + 1;
        free(str);
    }

    return createOK();

}

GEDCOMerror parseFamily(char **myArray, int *counter, Family *family, ListRef *myList){

   // printf("@@@@@@ IN THE FAMILY PARSER @@@@@\n");
    *counter = *counter + 1;

    while(myArray[*counter][0] != '0'){

        char *line;
        line = calloc(300, sizeof(char));
        strncpy(line, myArray[*counter], strlen(myArray[*counter]) + 2);

        char *p = strtok(line, " ");
        if(p == NULL){
            return createINV_RECORD(*counter + 1);
        }
        // check for the line increment here;

        p = strtok(NULL, " ");
        if(p == NULL){
            return createINV_RECORD(*counter + 1);
        }

      

        if(strcmp(p, "HUSB") == 0){
            FamRef *famref = createFamRef();
            setFamRefTag(famref, "HUSB");
            p = strtok(NULL, " @");
            setFamRefRef(famref, p);
            setFamRefFam(famref, family);
            //insertFamRef(myList, famref);
            insertBack(&myList->FamRef, famref);
          
        }
        else if(strcmp(p, "WIFE") == 0){

            FamRef *famref = createFamRef();
            setFamRefTag(famref, "WIFE");
            p = strtok(NULL, " @");
            setFamRefRef(famref, p);
            setFamRefFam(famref, family);
            //insertFamRef(myList, famref);
            insertBack(&myList->FamRef, famref);
          
        }
        else if(strstr(p, "CHIL") != 0){

            FamRef *famref = createFamRef();
            setFamRefTag(famref, "CHIL");
            p = strtok(NULL, " @");
            setFamRefRef(famref, p);
            setFamRefFam(famref, family);
            insertFamRef(myList, famref);
            //insertBack(&myList->FamRef, famref);
            
        }
        else if(checkFamEvents(p) == true){
            Event *event;
            event = createEvent();
            setEventType(event, p);
            
            *counter = *counter + 1;
            

            while(myArray[*counter][0] != '1' || myArray[*counter][0] == '2'){
                if(myArray[*counter][0] == '0'){
                    break;
                }
                char *temp;
                temp = calloc(strlen(myArray[*counter]) + 9, sizeof(char));
                strncpy(temp, myArray[*counter], strlen(myArray[*counter]) + 2);
                char *e = strtok(temp, " ");
                e = strtok(NULL, " ");
                if(strcmp(e, "DATE") == 0){
                    char *date = calloc(256, sizeof(char));
                    while(e != NULL){
                        strncat(date, e, strlen(e) + 2);
                        strncat(date, " ", 2);
                        e = strtok(NULL, " ");
                    }
                    if(isspace(date[strlen(date) -1])){
                        date[strlen(date) - 1] = '\0';
                    }
                    setEventDate(event, date);
                    setEventPlace(event, NULL);
                    free(date);
                }
                else if(strcmp(e, "PLAC") == 0){
                    char *plac = calloc(256, sizeof(char));
                    while(e != NULL){
                        strncat(plac, e, strlen(e) + 2);
                        strncat(plac, " ", 2);
                        e = strtok(NULL, " ");
                    }
                    if(isspace(plac[strlen(plac) - 1])){
                        plac[strlen(plac)- 1] = '\0';
                    }
                    setEventPlace(event, plac);
                    setEventDate(event, NULL);
                    free(plac);
                }
                else{
                    //if(strcmp(p, "FAMC") != 0 && strcmp(p, "FAMS") != 0){
                    Field *field = createField();
                    if(isspace(e[strlen(e) - 1])){
                        e[strlen(e) - 1] = '\0';
                    }
                    setFieldTag(field, e);
                    char *value = calloc(256, sizeof(char));
                    e = strtok(NULL, " ");
                    while(e != NULL){
                        strncat(value, e, strlen(e) + 2);
                        strncat(value, " ", 2);
                        e = strtok(NULL, " ");
                    }
                    if(isspace(value[strlen(value) - 1])){
                        value[strlen(value) - 1] = '\0';
                    }
                    setFieldValue(field, value);
                    insertEventOtherFields(event, field);
                    free(value);
                    //}
                }
                *counter = *counter + 1;
                free(temp);
            }
            insertBack(&family->events, event);
            *counter = *counter - 1; // added
        }

        else{
           // if(strcmp(p , "FAMC") != 0 && strcmp(p, "FAMS") != 0){
            Field *field = createField();
            setFieldTag(field, p);
            char *value = calloc(256, sizeof(char));
            while(p != NULL){
                strncat(value, p, strlen(p) + 2);
                strncat(value, " ", 2);
                p = strtok(NULL, " ");
            }
            setFieldValue(field, value);
            insertBack(&family->otherFields, field);
            free(value);
           // }
        }


        *counter = *counter + 1;
        free(line);
    }

    return createOK();
}

GEDCOMerror parseSubmitter(char **myArray, int *counter, Submitter *submitter){

   // printf("In the SUBM PARSER\n");
    *counter = *counter + 1;

    while(myArray[*counter][0] != '0'){

        char *line = calloc(500, sizeof(char));
        strncpy(line, myArray[*counter], strlen(myArray[*counter]) + 2);
        char *p = strtok(line, " ");
        
        if(p == NULL){
          //  return createINV_GEDCOM();
        }
        // check for level increment here
        p = strtok(NULL, " ");

        if(strcmp(p, "NAME") == 0){

            char *name = calloc(256, sizeof(char));
            p = strtok(NULL, " ");
            while(p != NULL){
                strncat(name, p, strlen(p) + 2);
                strncat(name, " ", 2);
                p = strtok(NULL, " ");
            }
            if(isspace(name[strlen(name) - 1])){
                name[strlen(name) -1] = '\0';
            }
            setSubmitterName(submitter, name);
            free(name);
        }
        else if(strcmp(p, "ADDR") == 0){
            char *addr;
            addr = calloc(300, sizeof(char));
            p = strtok(NULL, " ");
            while(p != NULL){
                strncat(addr, p,  strlen(p) + 2);
                strncat(addr, " ", 2);
                p = strtok(NULL, " ");
            }
            if(isspace(addr[strlen(addr) - 1])){
                addr[strlen(addr) - 1] = '\0';
            }
            setSubmitterAddress(submitter, addr);
            free(addr);

        }
        else{
            Field *field = createField();
            setFieldTag(field, p);
            p = strtok(NULL, " ");
            char *value = calloc(300, sizeof(char));
            while(p != NULL){
                strncat(value, p, strlen(p) + 2);
                strncat(value, " ", 2);
                p = strtok(NULL, " ");
            }
            if(isspace(value[strlen(value) - 1])){
                value[strlen(value) - 1] = '\0';
            }
            setFieldValue(field, value);
            insertSubmitterOtherFields(submitter, field);
            free(value);
        }

        free(line);
        *counter = *counter + 1;
    }

    return createOK();
}


/**
 * Dummy delete
 * this will be used to avoid double frees
 * used in families and individuals
 * */
void dummyDelete(void *toBeDeleted){

    if(toBeDeleted == NULL){
        return;
    }
    return;
}

bool checkForTags(char *str){
    for(int i = 0; i < strlen(str); i++){
        if(str[i] == '@'){
            return true;
        }
    }
    return false;
}

GEDCOMerror createINV_FILE(){
    GEDCOMerror inv_file;
    inv_file.type = INV_FILE;
    inv_file.line = -1;

    return inv_file;
}

GEDCOMerror createINV_RECORD(int line){
    GEDCOMerror inv_record;
    inv_record.type = INV_RECORD;
    inv_record.line = line;

    return inv_record;
}

GEDCOMerror createOK(){
    GEDCOMerror okay;
    okay.type = OK;
    okay.line = -1;

    return okay;
}

GEDCOMerror createINV_GEDCOM(){
    GEDCOMerror inv_gedcom;
    inv_gedcom.type = INV_GEDCOM;
    inv_gedcom.line = -1;\

    return inv_gedcom;
}

GEDCOMerror createINV_HEADER(int line){
    GEDCOMerror inv_header;
    inv_header.type = INV_HEADER;
    inv_header.line = -1;

    return inv_header;
}

GEDCOMerror createOTHER_ERROR(){
    GEDCOMerror other_error;
    other_error.type = OTHER_ERROR;
    other_error.line = -1;

    return other_error;
}

char *trim(char *string){
    
    int *i = calloc(1, sizeof(int));
    int *j = calloc(1, sizeof(int));
    *i = 0;
    *j = 0;

    for(*i = 0; *i < strlen(string); *i = *i + 1){
        if(string[*i] != ' '){
            string[*j] = string[*i];
            *j = *j + 1;
        }
    }
    free(i);
    free(j);
    return string;
}


//***************************************************************************************************************************************
// start of the header functions;

/**
 * Function that creates a header and mallocs space for it
 * sets all values to defauld
 * setters will be used to set the values later;
 * */
Header *createHeader(){
    Header *header = malloc(sizeof(Header));
    strncpy(header->source, "", 1);
    header->gedcVersion = 5.5;
    header->encoding = ASCII;
    header->submitter = NULL;
    header->otherFields = initializeList(printField, deleteField, compareFields);

    return header;
}

void setHeaderSource(Header * header, char *source){
    if(source == NULL){
        return;
    }
    if(strlen(source) == 0){
        return;
    }
    
    source = trim(source);
    strncpy(header->source, source, 249);
}

void setHeaderGEDCversion(Header * header, char *version){
    version[strcspn(version, "\r\n")] = '\0';
    if(strlen(version) == 0){
        return;
    }
    version = trim(version);

    header->gedcVersion  = atof(version);

    // if(strcmp(version, "5.5") == 0){
    //     header->gedcVersion = 5.5;
    // }
    // else if(strcmp(version, "5.5.1") == 0){
    //     header->gedcVersion = 5.51;
    // }

    // TO free;
    
}

void setHeaderEncoding(Header * header, char *encoding){
    
    encoding[strcspn(encoding, "\r\n")] = '\0';
   // char *p = encoding;
    encoding = trim(encoding);
    //free(p);
    if(strlen(encoding) == 0){
        return;
    }

    if(strcmp(encoding,  "ANSEL") == 0 ){
        header->encoding = ANSEL;
    }
    else if(strcmp(encoding, "UTF8") == 0){
        header->encoding = UTF8;
    }
    else if(strcmp(encoding, "UTF-8") ==  0){
        header->encoding = UTF8;
    }
    else if(strcmp(encoding, "UNICODE") == 0){
        header->encoding = UNICODE;
    }
    else if(strcmp(encoding, "ASCII") == 0){
        header->encoding = ASCII;
    }
}

void setHeaderOtherFields(Header * header, char *tag, char *value){
    Field *field = createField();
    setFieldTag(field, tag);
    setFieldValue(field, value);
    insertBack(&header->otherFields, field);
}

void deleteHeader(void *toBeDeleted){

    if(toBeDeleted == NULL){
        return;
    }

    Header *header = (Header*)toBeDeleted;
    clearList(&header->otherFields);
    free(header);
}

//************************************************************************
//      end of the header functions;



// ***************************************************************************
// Field Functions

/**
 * Function that creates a field struct
 * sets all fields to null
 * */
Field *createField(){
    Field *field;
    field = malloc(sizeof(Field));
  // field = calloc(1, sizeof(Field));
    field->tag = NULL;
    field->value = NULL;

    return field;
}

/**
 * Function that sets the tag
 * mallocs space for it
 * */
void setFieldTag(Field *field, char *tag){

    if(tag == NULL){
        field->tag = calloc(2, sizeof(char));
        strncpy(field->tag, "", 2);
        return;
    }
    if(strlen(tag) == 0){
        field->tag = calloc(2, sizeof(char));
        strncpy(field->tag, "", 2);
        return;
    }
    field->tag = calloc(strlen(tag) + 20, sizeof(char));
    strncpy(field->tag, tag, strlen(tag) + 2);

}

/**
 * Function that sets the value of the field
 * mallocs space for the value
 * frees the temp string
 * */
void setFieldValue(Field *field, char *value){
    if(value == NULL){
        field->value = calloc(2, sizeof(char));
        strncpy(field->value, "", 2);
        return;
    }
    if(strlen(value) == 0){
        field->value = calloc(2, sizeof(char));
        strncpy(field->value, "", 2);
        return;
    }
    
    field->value = calloc(strlen(value) + 20, sizeof(char));
    strncpy(field->value, value, strlen(value)+2);

}

//***********************************************************
// end of field functions


//*************************************************************************************
// Individual Functions

/**
 * Function that creates an individual
 * mallocs the struct
 * sets all null
 * initializes the lists
 * */
Individual *createIndividual(){

    Individual *individual = malloc(sizeof(Individual) * 1);
    individual->givenName = NULL;
    individual->surname = NULL;
    individual->events = initializeList(printEvent, deleteEvent, compareEvents);
    individual->families = initializeList(printFamily, dummyDelete, compareFamilies);
    individual->otherFields = initializeList(printField, deleteField, compareFields);

    return individual;
}

void setIndividualGivenName(Individual * individual, char *givenName){
    if(individual->givenName != NULL){
        free(individual->givenName);
    }
    if(givenName == NULL){
        individual->givenName = calloc(2, sizeof(char));
        strncpy(individual->givenName, "", 2);
        return;
    }
    givenName[strcspn(givenName, "\r\n")] = '\0';
    
    if(strlen(givenName) == 0){
        individual->givenName = calloc(2, sizeof(char));
        strncpy(individual->givenName, "", 2);
        return;
    }
    if(isspace(givenName[strlen(givenName) - 1])){
        givenName[strlen(givenName) -1 ] = '\0';
    }
    individual->givenName = calloc(strlen(givenName) + 9, sizeof(char));
    strncpy(individual->givenName, givenName, strlen(givenName) + 2);
}

void setIndividualSurname(Individual *individual, char *surname){

    if(individual->surname != NULL){
        free(individual->surname);
    }

    if(surname == NULL){
        individual->surname = calloc(2, sizeof(char));
        strncpy(individual->surname, "", 2);
        return;
    }
    surname[strcspn(surname, "\r\n")]  = '\0';

    if(strlen(surname) == 0){
        individual->surname = calloc(2, sizeof(char));
        strncpy(individual->surname, "", 2);
        return;
    }
    if(isspace(surname[strlen(surname) - 1])){
        surname[strlen(surname) - 1] = '\0';
    }
    individual->surname = calloc(strlen(surname) + 9, sizeof(char));
    strncpy(individual->surname, surname, strlen(surname) + 2);
}

void setIndividualEvent(Individual *individual, Event *event){
    if(event == NULL){
        return;
    }

    insertBack(&individual->events, event);
}

void setIndividualFamily(Individual *individual, Family *family){
    if(family == NULL){
        return;
    }
    insertBack(&individual->families, family);
}

void setIndividualOtherFields(Individual *individual, char *tag, char *value){
    if(tag == NULL || value == NULL){
        return;
    }

    Field *f = createField();
    setFieldTag(f, tag);
    setFieldValue(f, value);

    insertBack(&individual->otherFields, f);

}

//**************************************************************************


// linking functions
IndiRef *createIndiRef(){
    IndiRef *ref  = malloc(sizeof(IndiRef));
    ref->ref = NULL;
    ref->individual  = NULL;

    return ref;
}


void setIndiRefRef(IndiRef *indi, char *ref){
    if(ref == NULL){
        indi->ref = calloc(2, sizeof(char));
        strncpy(indi->ref, "", 2);
        return;
    }
    indi->ref = calloc(strlen(ref) + 9, sizeof(char));
    strncpy(indi->ref, ref, strlen(ref) + 2);

}


void setIndiRefIndi(IndiRef *indi, Individual *individual){
    //if(individual == NULL){

    //    return;
   // }
    indi->individual = individual;
}

void deleteIndiRef(void* toBeDeleted){
    if(toBeDeleted == NULL){
        return;
    }
    IndiRef *ref = (IndiRef*)toBeDeleted;
    free(ref->ref);
    //deleteIndividual(ref->individual);
    ref->individual = NULL;
    free(ref);
}


int compareIndiRef(const void* first,const void* second){
    if(first == NULL || second == NULL){
        return -2;
    }
    IndiRef *a;
    IndiRef *b;
    a = (IndiRef*)first;
    b = (IndiRef*)second;

    if(strcmp(a->ref, b->ref) == 0){
        return 0;
    }
    else if(strcmp(a->ref, b->ref) > 0){
        return 1;
    }
    else {
        return -1;
    }
}


char* printIndiRef(void* toBePrinted){

    if(toBePrinted == NULL){
        return NULL;
    }
    char *str;
    IndiRef *ref = (IndiRef*)toBePrinted;
    str = calloc(strlen(ref->ref) + 9, sizeof(char));

    sprintf(str, "%s", ref->ref);
    return str;
}


FamRef *createFamRef(){

    FamRef *famref;
    famref = malloc(sizeof(FamRef));
    famref->ref = NULL;
    famref->tag = NULL;
    famref->family = NULL;

    return famref;
}

void setFamRefTag(FamRef *famref, char *tag){

    if(tag == NULL){
        famref->tag = calloc(2, sizeof(char));
        strncpy(famref->tag, "", 2);
        return;
    }

    famref->tag = calloc(strlen(tag) + 9, sizeof(char));
    strncpy(famref->tag, tag, strlen(tag) + 2);

}


void setFamRefRef(FamRef *famref, char *ref){

    if(ref == NULL){
        famref->ref = calloc(2, sizeof(char));
        strncpy(famref->ref, "", 2);
        return;
    }

    famref->ref = calloc(strlen(ref) + 9, sizeof(char));
    strncpy(famref->ref, ref, strlen(ref) + 2);

}


void setFamRefFam(FamRef *famref, Family *family){

    if(family  == NULL){
        return;
    }

    famref->family = family;
}

void deleteFamRef(void *toBeDeleted){

    if(toBeDeleted == NULL){
        return;
    }

    FamRef *famref = (FamRef*)toBeDeleted;

    free(famref->ref);
    free(famref->tag);
    famref->family = NULL;
    free(famref);

}


int compareFamRef(const void *first, const void* second){

    if(first == NULL || second == NULL){
        return -2;
    }

    FamRef *a;
    FamRef *b;
    a = (FamRef*)first;
    b = (FamRef*)second;

    char *str1;
    char *str2;

    str1 = calloc(strlen(a->tag) + strlen(a->ref) + 9, sizeof(char));
    str2 = calloc(strlen(b->ref) + strlen(b->tag) + 9, sizeof(char));

    strncpy(str1, "", 2);
    strncpy(str2, "", 2);

    strncat(str1, a->ref, strlen(a->ref) + 2);
    strncat(str1, " ", 2);
    strncat(str1, a->tag, strlen(a->tag) + 2);

    strncat(str2, b->ref, strlen(b->ref) + 2);
    strncat(str2, " ", 2);
    strncat(str2, b->tag, strlen(b->tag) + 2);

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
    else if(strcmp(str1, str2) < 0){
        free(str1);
        free(str2);
        return -1;
    }

    free(str1);
    free(str2);
    return -2;

}


char *printFamRef(void *toBePrinted){

    if(toBePrinted == NULL){
        return NULL;
    }
    
    FamRef *ref = (FamRef*)toBePrinted;

    char *str;
    str = calloc(strlen(ref->ref) + strlen(ref->tag) + 9, sizeof(char));

    sprintf(str, "%s %s", ref->ref, ref->tag);

    return str;

}

ListRef *createListRef(){
    ListRef *listref;
    listref = malloc(sizeof(ListRef));

    listref->FamRef = initializeList(printFamRef, deleteFamRef, compareFamRef);
    listref->IndiRef = initializeList(printIndiRef, deleteIndiRef, compareIndiRef);

    return listref;
}

void insertFamRef(ListRef *listref, FamRef *famref){

    if(famref == NULL){
        return;
    }

    insertBack(&listref->FamRef, famref);
}
void insertIndiRef(ListRef *listref, IndiRef *indiref){

    if(indiref == NULL){
        return;
    }

    insertBack(&listref->IndiRef, indiref);
}

void deleteListRef(ListRef *list){

    if(list == NULL){
        return;
    }
    clearList(&list->FamRef);
    clearList(&list->IndiRef);

    free(list);
}

//****************************************************************************************
// End of linking Functions




//*****************************************************************************8
// GEDCOM object Functions

GEDCOMobject *createObject(){

    GEDCOMobject *obj = malloc(sizeof(GEDCOMobject));
    obj->header = NULL;
    obj->families = initializeList(printFamily, deleteFamily, compareFamilies);
    obj->individuals = initializeList(printIndividual, deleteIndividual, compareIndividuals);
    obj->submitter = NULL;

    return obj;
}
void insertFamilyInObject(GEDCOMobject *obj, Family *family){

    if(family == NULL){
        return;
    }

    insertBack(&obj->families, family);
}
void insertIndividualInObject(GEDCOMobject *obj, Individual *individual){

    if(individual == NULL){
        return;
    }

    insertBack(&obj->individuals, individual);
}

//***************************************************************************************
// END of GEDCOM Object Functions


//*****************************************************************************************
// Family Functions

Family *createFamily(){

    Family *family = malloc(sizeof(Family));
    family->wife = NULL;
    family->husband = NULL;
    family->children = initializeList(printIndividual, dummyDelete, compareIndividuals);
    family->events = initializeList(printEvent, deleteEvent, compareEvents);
    family->otherFields = initializeList(printField, deleteField, compareFields);

    return family;
}
void setFamilyWife(Family *family, Individual *individual){

    if(individual == NULL){
        return;
    }

    family->wife = individual;
}
void setFamilyHusb(Family *family, Individual *individual){

    if(individual == NULL){
        return;
    }

    family->husband = individual;
}
void insertChildInFamily(Family *family, Individual *individual){

    if(individual == NULL){
        return;
    }

    insertBack(&family->children, individual);
}
void insertEventInFamily(Family *family, Event *event){

    if(event == NULL){
        return;
    }

    insertBack(&family->events, event);
}
void insertOtherFieldsInFamily(Family *family, Field *field){

    if(field == NULL){
        return;
    }

    insertBack(&family->otherFields, field);
}

//******************************************************************************************
// End of Family Functions


//*****************************************************************************************
// Submitter Functions

Submitter *createSubmitter(){

    Submitter *submitter = malloc(sizeof(Submitter) + sizeof(char[256]));
    submitter->submitterName[0] = '\0';
    submitter->otherFields = initializeList(printField, deleteField, compareFields);
    submitter->address[0] = '\0';

    return submitter;

}
void setSubmitterName(Submitter *submitter, char *name){
    if(name == NULL){
        strncpy(submitter->submitterName, "", 2);
        return;
    }
    name[strcspn(name, "\r\n")] = '\0';
    strncpy(submitter->submitterName, name, 61);
}
void setSubmitterAddress(Submitter *submitter, char *address){

    if(address == NULL){
        strncpy(submitter->address, "", 2);
        return;
    }
    address[strcspn(address, "\r\n")] = '\0';
    strncpy(submitter->address, address, strlen(address) + 2);

}
void insertSubmitterOtherFields(Submitter *submitter, Field *field){

    if(field == NULL){
        return;
    }

    insertBack(&submitter->otherFields, field);
}
void deleteSubmitter(void *toBeDeleted){

    if(toBeDeleted == NULL){
        return;
    }

    Submitter *submitter = (Submitter*)toBeDeleted;
    clearList(&submitter->otherFields);
    free(submitter);
}
/*
char *printSubmitter(void *toBePrinted){
    if(toBePrinted == NULL){
        return NULL;
    }
    Submitter *s = (Submitter*)toBePrinted;
    char *str = calloc(strlen(s->submitterName) + strlen(s->address) + 
    strlen(toString(s->otherFields)) + 9,  sizeof(char) );
    sprintf(str, "%s %s %s", s->submitterName, s->address, toString(s->otherFields));

    return str;
}
*/


//*************************************************************************************
// end of submitter functions



//***********************************************************************************
// Event Functions

Event *createEvent(){

    Event *event = malloc(sizeof(Event));
    event->type[0] = '\0';
    event->date = NULL;
    event->place = NULL;
    event->otherFields = initializeList(printField, deleteField, compareFields);

    return event;
}
void setEventType(Event *event, char *type){

    if(type == NULL){
        strncpy(event->type, "", 2);
        return;
    }

    strncpy(event->type, type, 5);

}
void setEventDate(Event *event, char *date){

    if(event->date != NULL){
        free(event->date);
    }

    if(date == NULL){
        event->date = calloc(2, sizeof(char));
        strncpy(event->date, "", 2);
        return;
    }
    
    event->date = calloc(strlen(date) + 9, sizeof(char));
    strncpy(event->date, date, strlen(date) + 2);

}
void setEventPlace(Event *event, char *place){

    if(event->place != NULL){
        free(event->place);
    }

    if(place == NULL){
        event->place = calloc(2, sizeof(char));
        strncpy(event->place, "", 2);
        return;
    }

    event->place = calloc(strlen(place) + 9, sizeof(char));
    strncpy(event->place, place, strlen(place) + 2);

}
void insertEventOtherFields(Event *event, Field *field){

    if(field == NULL){
        return;
    }

    insertBack(&event->otherFields, field);

}

//************************************************************************************

// function that gets the ref and returns it;
void getRef(char *src, char *dest){
    int j = 0;
    for(int i = 0; i < strlen(src); i++){
        if(src[i] == '@'){
            i++;
            while(src[i] != '@'){
                dest[j] = src[i];
                j++;
                i++;
            }
            return;
        }
    }
}

// check if string is an event
bool checkIndiEvents(char *str){

    if(str  == NULL){
        return false;
    }
    for(int i = 0; eventTags[i] != NULL; i++){
        if(strstr(str, eventTags[i]) != 0){
            return true;
        }
    }
    return false;
}

bool checkFamEvents(char *str){

    if(str == NULL){
        return false;
    }
    for(int i = 0; famTags[i] != NULL; i++){
        if(strcmp(str, famTags[i]) == 0){
            return true;
        }
    }
    return false;
}


void linker(ListRef *myList){
    /*
    if(myList == NULL){
        return;
    }
    Node *indiNode = myList->IndiRef.head;
    Node *famNode = myList->FamRef.head;

    while(indiNode != NULL){
        IndiRef *indiref = (IndiRef*)indiNode->data;
        while(famNode != NULL){
            FamRef *famref = (FamRef*)famNode->data;
            if(strcmp(famref->ref, indiref->ref) == 0){
                if(strcmp(famref->tag, "HUSB") == 0){
                    Individual *indi = (Individual*)indiref->individual;
                    Family *fam = (Family*)famref->family;
                    if(indi != NULL && fam != NULL){
                        fam->husband = indi;
                        insertBack(&(indi->families), fam);
                    }
                }
                else if(strcmp(famref->tag, "WIFE") == 0){
                    Individual *indi = (Individual*)indiref->individual;
                    Family *fam = (Family*)famref->family;
                    if(indi != NULL && fam != NULL){
                        fam->wife = indi;
                        insertBack(&(indi->families), fam);
                    }
                    
                }
                else if(strcmp(famref->tag, "CHIL") == 0){
                    Individual *indi = (Individual*)indiref->individual;
                    Family *fam = (Family*)famref->family;
                    if(indi != NULL && fam != NULL){
                        //insertChildInFamily(fam, indi);
                        insertBack( &(fam->children), indi);
                        insertBack(&(indi->families), fam);
                    }
                    
                }
            }
            famNode = famNode->next;
        }
        indiNode = indiNode->next;
        famNode = myList->FamRef.head;

    }
    */
    if(myList == NULL){
        return;
    }

    Node *indi_node = myList->IndiRef.head;
    Node *fam_node = myList->FamRef.head;

    while(fam_node != NULL){
        indi_node = myList->IndiRef.head;
        FamRef *famref = fam_node->data;
        while(indi_node != NULL){
            IndiRef *indiref = indi_node->data;
            if(strcmp(famref->ref, indiref->ref) == 0){
                if(strcmp(famref->tag, "HUSB") == 0){
                    famref->family->husband = indiref->individual;
                    insertBack(&indiref->individual->families, famref->family);
                }
                else if(strcmp(famref->tag, "WIFE") == 0){
                    famref->family->wife = indiref->individual;
                    insertBack(&indiref->individual->families, famref->family);
                }

                else if(strcmp(famref->tag, "CHIL") == 0){
                    insertBack(&famref->family->children, indiref->individual);
                    insertBack(&indiref->individual->families, famref->family);
                }
            }
            indi_node = indi_node->next;
        }

        fam_node = fam_node->next;
    }
}

bool cmp(const void *first, const void *second){
    if(first == NULL || second == NULL){
        return false;
    }
    Individual *a = (Individual*)first;
    Individual *b = (Individual*)second;
    int x = compareIndividuals(a, b);

    if(x == 0){
        return true;
    }
    return false;
}


void getFam(Individual *child, List* descList){
  //  printf("\nentering function: %s %s\n",child->givenName, child->surname);
  //  printf ("number of families: %d\n", getLength(child->families));

    if(child == NULL){
        return;
    }
   // insertBack(descList, child);
    Node *node = child->families.head;

    while(node != NULL){
        Family *baseFam = node->data;
        Node *child_node = baseFam->children.head;

        while(child_node != NULL){
            Individual *tempChild = child_node->data;
            insertBack(descList, copy(tempChild));

            //getFam(copy(tempChild), descList);

            child_node = child_node->next;
        }

        node = node->next;
    }
    //Family *baseFam = node->data;

    
    
    
    return;
}


// void getFam(Family *family, List *descList){

//     if(getLength(family->children) == 0){
//         return;
//     }

//     Node *child_node = family->children.head;

//     while(child_node !=NULL){
//        Individual *child = child_node->data;
//        insertBack(descList, child);

//        Node *child_fam = child->families.head;
//        while(child_fam != NULL){
//            Family *childFam = child_fam->data;
//            getFam(childFam, descList);
//            child_fam = child_fam->next;
//        }

//        child_node = child_node->next;
//     }


// }

Individual *copy(void *toBeCopied){

    if(toBeCopied == NULL){
        return NULL;

    }

    Individual *new = createIndividual();
    Individual *ori = (Individual*)toBeCopied;

    setIndividualGivenName(new, ori->givenName);
    setIndividualSurname(new, ori->surname);

    new->events = ori->events;
    new->families = ori->families;
    new->otherFields = ori->otherFields;

    return new;
}


char *encodingToStr(CharSet encoding){

    char *str = calloc(1000, sizeof(char));
    strncpy(str, "", 2);
    if(encoding == ASCII){
        strncpy(str, "ASCII", 7);
    }
    else if(encoding == ANSEL){
        strncpy(str, "ANSEL", 7);
    }
    else if(encoding == UNICODE){
        strncpy(str, "UNICODE", 9);
    }
    else if(encoding == UTF8){
        strncpy(str, "UTF-8", 5);
    }
    return str;
}
       
void getDescendantsN(Individual *person, List *list, int max){



}

Tracker *createTracker(){
  Tracker *tracker = malloc(sizeof(Tracker));
  tracker->individual = NULL;
  tracker->level = 1;
  return tracker;  
}
void deleteTracker(void *toBeDeleted){
    if(toBeDeleted == NULL){
        return;
    }
    Tracker *tracker = (Tracker*)toBeDeleted;
    tracker->individual = NULL;
    free(tracker);
}
char *printTracker(void *toBePrinted){
    if(toBePrinted == NULL){
        return NULL;
    }
    char *str  =  calloc(100, sizeof(char));
    strncpy(str, "           ", 10);
    return str;
}
int compareTracker(const void *first, const void *second){
    if(first == NULL || second == NULL){
        return 0;
    }
    Tracker *a = (Tracker*)first;
    Tracker *b = (Tracker*)second;

    if(a->level == b->level){
        return 0;
    }
    else if(a->level > b->level){
        return -1;
    }
    else{
        return 1;
    }

}

int customCompare(const void *first, const void *second){

    if(first == NULL || second == NULL){

        return -2;
    }

    Individual *a;
    Individual *b;
    a = (Individual*)first;
    b = (Individual*)second;

    if(a->surname != NULL && b->surname != NULL){
        if(strcmp(a->surname, b->surname) > 0){

            return 1;
        }
        else if(strcmp(a->surname, b->surname) < 0) {
            return -1;
        }
    }

    if(a->givenName !=  NULL && b->givenName != NULL){
        if(strcmp(a->givenName, b->givenName) > 0){
            return 1;
        }
        else if(strcmp(a->givenName, b->givenName) < 0){
            return -1;
        }
    }

    return 0;

   
}
       
void deleteMotherList(void *toBeDeleted){
    if(toBeDeleted == NULL){
        return;
    }
    List *l = (List*)toBeDeleted;
    Node *n = l->head;
    while(n != NULL){
        Individual *i = n->data;
        free(i->givenName);
        free(i->surname);
        free(i);
        Node * delete = n;
        n =  n->next;
        free(delete);
    }
    //clearList(l);
    free(l);
}

void parth(void *toBeDeleted){
    if(toBeDeleted == NULL){
        return;
    }
    Individual *i = (Individual*)toBeDeleted;
    free(i->givenName);
    free(i->surname);

    free(i);
}

char *random(){
	char *s = calloc(50, sizeof(char));
	strcpy(s, "{}");
	
	return s;
}





//************************************************************************************************************************************
// 	a3 functions;

char* returnIndiList(){
		GEDCOMobject *obj = NULL;
		char *filename = calloc(50, sizeof(char));
		//strcpy(filename, "./uploads/simpleValid.ged");
		
		if(filename == NULL){
				return NULL;
		}
		
		//createGEDCOM(filename, &obj);
		//Node *n = obj->individuals.head;
		//while(n != NULL){
		//		Individual *i = n->data;
		//		printf("First Name %s Last Name %s\n", i->givenName, i->surname);
		//		n = n->next;
		//}
		
		free(filename);
		return NULL;
}

void createSimpleGedcom(char *source, float version, char* encoding, char *filename, char *subName, char *subAddr){
		
		GEDCOMobject *obj = createObject();
		char *file = calloc(250, sizeof(char));
		strcpy(file, "./uploads/");
		strcat(file, filename);
		Header *header = createHeader();
		Submitter *submitter = createSubmitter();
		
		setHeaderSource(header, source);
		//setHeaderGEDCversion(header, version);
		header->gedcVersion = 5.5;
		setHeaderEncoding(header, encoding);
		
		setSubmitterName(submitter, subName);
		setSubmitterAddress(submitter, subAddr);
		
		obj->header = header;
		header->submitter = submitter;
		obj->submitter = submitter;
		
		writeGEDCOM(file, obj);
		free(file);
		
	
		return;
}

void addAnIndividual(char *firstName, char *surname, char *filename){
	
		GEDCOMobject *obj  = NULL;
		
		char *f = calloc(150, sizeof(char));
		strcpy(f, "./uploads/");
		strcat(f, filename);
		printf("First name is:  %s last name: %s\n", firstName, surname);
		createGEDCOM(f, &obj);
		Individual *indi = createIndividual();
		setIndividualGivenName(indi, firstName);
		setIndividualSurname(indi, surname);
		
		insertBack(&obj->individuals, indi);
		
		writeGEDCOM(f, obj);
		
		return;
	
}

char *returnFileLogData(char *filename){
	// create an object;
	GEDCOMobject *obj;
	
	char  *file = calloc(200, sizeof(char));
	strcpy(file, "./uploads/");
	strcat(file, filename);
	createGEDCOM(file, &obj);
	
	// object is created;
	
	
	
}

char *returnDescList(char *filename, char *firstName, char *surname, char *maxGen){
		char *list;
		GEDCOMobject *obj = NULL;
		char *file = calloc(250, sizeof(char));
		strcpy(file, "./uploads/");
		strcat(file, filename);
		createGEDCOM(file, &obj);
		float max = atoi(maxGen);
		
		Individual *indi = createIndividual();
		setIndividualGivenName(indi, firstName);
		setIndividualSurname(indi, surname);
		
		Individual *i = findPerson(obj, cmp, indi);
		
		List myList = getDescendantListN(obj, i, max);
		printf("list length is %d\n",myList.length);
		
		list = gListToJSON(myList);
		
		printf("The list is %s\n", list);
		
		return list;
}

char *returnAncList(char *filename, char *firstName, char *surname, char *maxGen){
		
		
		char *list;
		GEDCOMobject *obj = NULL;
		char *file = calloc(250, sizeof(char));
		strcpy(file, "./uploads/");
		strcat(file, filename);
		createGEDCOM(file, &obj);
		float max = atoi(maxGen);
		
		Individual *indi = createIndividual();
		setIndividualGivenName(indi, firstName);
		setIndividualSurname(indi, surname);
		
		Individual *i = findPerson(obj, cmp, indi);
		
		List myList = getAncestorListN(obj, i, max);
		printf("list length is %d\n",myList.length);
		
		list = gListToJSON(myList);
		
		printf("The list is %s\n", list);
		
		return list;
		
}



char *ObjectToJSON(char *filename){
		
		char *JSONstr = calloc(10000, sizeof(char));
		char *file = calloc(150,  sizeof(char));
		strcpy(file, "./uploads/");
		strcat(file, filename);
		
		printf("1\n");
		GEDCOMobject *obj = NULL;
		createGEDCOM(file, &obj);
		printf("2\n");
		Header *h = obj->header;
		Submitter *sub = obj->submitter;
		
		char *nI  = calloc(10, sizeof(char));
		char *nF = calloc(10, sizeof(char));
		char *vers = calloc(10, sizeof(char));
		printf("3\n");
		
		sprintf(nI, "%d", getLength(obj->individuals) );
		
		sprintf(nF, "%d", getLength(obj->families) );
		sprintf(vers, "%.2f", h->gedcVersion);
		
		printf("4\n");
		
		char *x = encodingToStr(h->encoding);
		printf("5\n");
		
		
		char quote = '"';
		// src, version, encoding, subName, subAddr, numIndi, numFam, fileName;
		
		sprintf(JSONstr, "{%csource%c:%c%s%c,%cversion%c:%c%s%c,%cencoding%c:%c%s%c,%csubName%c:%c%s%c,%csubAddr%c:%c%s%c,%cnumIndi%c:%c%s%c,%cnumFam%c:%c%s%c,%cfileName%c:%c%s%c}",quote, quote, quote, h->source, 
		quote, quote, quote, quote, vers, quote, quote, quote, quote, x, quote, quote, quote, quote, sub->submitterName, quote,
		quote, quote, quote, sub->address, quote, quote, quote, quote, nI, quote, quote, quote, quote, nF, quote, quote, quote, quote, file, quote); 
		
		//sprintf(JSONstr, "{%cfileName%c:%c%s%c}", quote, quote, quote, file, quote);
		printf("Resulting string is %s\n", JSONstr);
		
		return JSONstr;
	
}
