#include "LinkedListAPI.h"
#include "GEDCOMutilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

List initializeList(char*(*printFunction)(void* toBePrinted),void (*deleteFunction)(void* toBeDeleted),
	int (*compareFunction)(const void* first,const void* second)){

	List myList;
	myList.head = NULL;
	myList.tail = NULL;
	myList.length = 0;
	myList.deleteData = deleteFunction;
	myList.compare = compareFunction;
	myList.printData = printFunction;

	return myList;

}

void deleteFunction(void* toBeDeleted){

}

void compareFunction(const void* first, const void *second){



}
char *printFunction(void *toBePrinted){
	char *str = NULL;
	return str;
	
}

Node* initializeNode(void* data){

	Node *n;
	n = malloc(sizeof(Node));

	if (n == NULL){
		return NULL;
	}

	n->data = data;
	n->next = NULL;
	n->previous = NULL;

	return n;

}

void clearList(List* list){
	
    if (list == NULL){
		return;
	}
	
	if (list->head == NULL && list->tail == NULL){
		return;
	}
	
	Node* tmp;
	
	while (list->head != NULL){
		list->deleteData(list->head->data);
		tmp = list->head;
		list->head = list->head->next;
		free(tmp);
	}
	
	list->head = NULL;
	list->tail = NULL;
}

void insertBack(List* list, void* toBeAdded){
	if (list == NULL || toBeAdded == NULL){
		return;
	}
	
	Node* newNode = initializeNode(toBeAdded);
	
    if (list->head == NULL && list->tail == NULL){
        list->head = newNode;
        list->tail = list->head;
        list->length++;
    }else{
		newNode->previous = list->tail;
        list->tail->next = newNode;
    	//newNode = list->tail;
    	list->tail = newNode;
    	list->length++;
    }
	
}

void insertFront(List* list, void* toBeAdded){
	if (list == NULL || toBeAdded == NULL){
		return;
	}
	
	Node* newNode = initializeNode(toBeAdded);
	
    if (list->head == NULL && list->tail == NULL){
        list->head = newNode;
        list->tail = newNode;//list->head;
        list->length++;
    }else{

		newNode->next = list->head;
        list->head->previous = newNode;
    	list->head = newNode;
    	list->length++;
    }
    
    /*
    Node *tempNode = initializeNode(toBeAdded);
    //tempNode = (Node*)toBeAdded;
    
    if(list->head ==  NULL){
    	list->head = tempNode;
    	list->length++;
    }
    else{
    	tempNode->next = list->head;
    	list->head->previous = tempNode;
    	list->head = tempNode;
    	list->length++;

    	
    }
    */
}

void* getFromFront(List list){
	
	if (list.head == NULL){
		return NULL;
	}
	
	return list.head->data;
}

void* getFromBack(List list){
	
	if (list.tail == NULL){
		return NULL;
	}
	
	return list.tail->data;
	
}

void* deleteDataFromList(List* list, void* toBeDeleted){
	if (list == NULL || toBeDeleted == NULL){
		return NULL;
	}
	
	Node* tmp = list->head;
	
	while(tmp != NULL){
		if (list->compare(toBeDeleted, tmp->data) == 0){
			//Unlink the node
			Node* delNode = tmp;
			
			if (tmp->previous != NULL){
				tmp->previous->next = delNode->next;
			}else{
				list->head = delNode->next;
			}
			
			if (tmp->next != NULL){
				tmp->next->previous = delNode->previous;
			}else{
				list->tail = delNode->previous;
			}
			
			void* data = delNode->data;
			free(delNode);
			list->length--;
			return data;
			
		}else{
			tmp = tmp->next;
			list->length--;
		}

	}
	list->length--;
	return NULL;
}

void insertSorted(List *list, void *toBeAdded){
	if (list == NULL || toBeAdded == NULL){
		return;
	}
	
	if (list->head == NULL){
		insertBack(list, toBeAdded);
		list->length++;
		return;
	}
	
	if (list->compare(toBeAdded, list->head->data) <= 0){
		insertFront(list, toBeAdded);
		list->length++;
		return;
	}
	
	if (list->compare(toBeAdded, list->tail->data) > 0){
		insertBack(list, toBeAdded);
		list->length++;
		return;
	}
	
	Node* currNode = list->head;
	
	while (currNode != NULL){
		if (list->compare(toBeAdded, currNode->data) <= 0){
		
			//char* currDescr = list->printData(currNode->data); 
			//char* newDescr = list->printData(toBeAdded); 
		
			//printf("Inserting %s before %s\n", newDescr, currDescr);

			//free(currDescr);
			//free(newDescr);
		
			Node* newNode = initializeNode(toBeAdded);
			newNode->next = currNode;
			newNode->previous = currNode->previous;
			currNode->previous->next = newNode;
			currNode->previous = newNode;
			list->length++;
			return;
		}
	
		currNode = currNode->next;
	}
	list->length++;
	return;
}

char* toString(List list){
	ListIterator iter = createIterator(list);
	char* str;
		
	str = (char*)malloc(sizeof(char));
	strcpy(str, "");
	
	void* elem;
	while( (elem = nextElement(&iter)) != NULL){
		char* currDescr = list.printData(elem);
		int newLen = strlen(str)+50+strlen(currDescr);
		str = (char*)realloc(str, newLen);
		strcat(str, "\n");
		strcat(str, currDescr);
		
		free(currDescr);
	}
	
	return str;
}


ListIterator createIterator(List list){
    ListIterator iter;

    //Assert creates a partial function...
    //assert (list != NULL);
	
    iter.current = list.head;
    
    return iter;
}

void* nextElement(ListIterator* iter){
    Node* tmp = iter->current;
    
    if (tmp != NULL){
        iter->current = iter->current->next;
        return tmp->data;
    }else{
        return NULL;
    }
}

int getLength(List list){

	int counter;
	counter = 0;

	Node *n;

	if(list.head == NULL){
		return 0;
	}

	n = list.head;
    /*
	if(n == NULL){
		return 0;
	}
	*/

	while(n != NULL){

		counter++;

		n = n->next;

	}

	return counter;
}

void* findElement(List list, bool (*customCompare)(const void* first,const void* second), const void* searchRecord){


    Node *n = list.head;
	if(list.head == NULL){
		return NULL;
	}

	else{
			
		while(n != NULL){

			if(customCompare(searchRecord, n->data) == true){
				return n->data;
			}
			n = n->next;

		} 
		return NULL;
	}
   
}






