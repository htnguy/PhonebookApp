//  CECS 130-02 lab 6
// Hieu Nguyen
// Phone Book program that allows the user to add, view, and delete their phone entries
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

// struct for entry
 typedef struct {
    char firstName[20];
     char lastName[20];
    char strPhone[13];
}Entry;

// function prototyping
void displayMenu();
void getUserInput();
void displayEntries(Entry* entries,  int total);
Entry new(char* first, char* last, char* phone);
int delete(char* first, char* last, int total, Entry* entries);
int findPhone(char* first, char* last, Entry* entries, int total);
int deleteAll(Entry* entries,int total);
void alphabetize(char* firstOrLast, Entry* entries, int total);


int userInput;
// declaring userInput in the global scope in order to make it accessible in multiple functions; helps to dry up the code.

int main(int argc, const char * argv[]) {
	
	srand(time(NULL));
    char first[20], last[20], phone[13], firstOrLast[6];
    int total=1, random;
    
   
    //allocating memory for the Entry* and checking if memory allocation was successful
   Entry* entries = (Entry*)malloc(1*(sizeof*entries));
    if(entries!=NULL){
    getUserInput();
    }else{
        printf("There was an error allocating the memory for the phone entries");
    }

    while(userInput!=8){
        switch (userInput) {
            case 1:// adding a new entry
                
                printf("First name: ");
                scanf("%s", first);
                printf("Last name: ");
                scanf("%s", last);
                getchar();
                printf("Phone Number: \n");
                fgets(phone, 13 , stdin);
                
                entries = realloc(entries,total*sizeof(Entry));  
                entries[total-1] = new(first, last, phone);   
				total++;
				getUserInput();
                break;
                
            case 2: // deleting the matching entry
                
                printf("First Name: ");
                scanf("%s", first);
                printf("Last Name: ");
                scanf("%s", last);
                if(delete(first , last, total, entries)){
                    printf("the entry was successfully deleted\n");
                    total--;
                    entries = realloc(entries, (total)*sizeof(Entry)); 
                }else{
                    printf("Could not find the entry\n");
                }
    			getUserInput();
                break;
                
            case 3: // viewing all of the entries
                
                displayEntries(entries, total);
                getUserInput();
                break;
                
            case 4:// alphabetizing the phone entries
            
            	printf("Do you want to sort by first or last name?\n");
            	scanf("%s", &firstOrLast);
            	alphabetize(firstOrLast, entries, total);
            	getUserInput();
				break;
			
			case 5:// finding the phone number given a name
			
				printf("First Name: ");
				scanf("%s", first);
				printf("Last Name: ");
				scanf("%s", last);
				if(findPhone(first, last, entries, total)){
					//code to be executed if the phone number was found.
					printf("Phone Search Success\n");
				}else {
					printf("Can not find phone number\n");
				}
				getUserInput();
				break;
				
			case 6:// calling a random friend
			
				random=(int) rand()%(total-1);
				printf("calling %s %s %s ...\n", entries[random].firstName, entries[random].lastName, entries[random].strPhone);
				getUserInput();
				break;
				
			case 7:// deleting everything
				
				entries = realloc(entries, 0);
				total = 1;
				getUserInput();
				break;	
				
			default:
				
				printf("The request is not valid please try again\n");
				getUserInput();
				break;
    	
        }
        
    }
    return 0;
}

// displays the menu
void displayMenu(){
    char * options[10] ={"PhoneBook Application", "1. new entry", "2. delete an entry", "3. view all entry","4. Alphabetized by last or first name", "5.find phone number by name", "6.Call random friend", "7. Delete everything", "8.exit", "Select your option"};
    int i;
    for (i=0; i<10;i++) {
        printf("%s \n", options[i]);
    }
}

// getting user input and checking for input error
void getUserInput(){
	displayMenu();
	scanf("%d", &userInput);
}

// add new entry in the struc array of entries
Entry new(char* first, char* last, char* phone){
    //creating the new entry
    Entry newEntry;
    strcpy(newEntry.firstName, first);
    strcpy(newEntry.lastName, last);
    strcpy(newEntry.strPhone, phone);
    printf("The new entry has been saved\n");
    return newEntry;
}

// displaying all of the entries
void displayEntries(Entry* entries, int total){
    int k;
    for(k=0; k < total-1; k++){
       if(strlen(entries[k].firstName)!=0){// check that the firstName is not empty; which is a temporary check
            // that the entry is not null or invalid.
           printf("%s %s: %s\n", entries[k].firstName,entries[k].lastName, entries[k].strPhone);
		}
    }
}

// deleting a specific entry with the matching first and last  name
int delete(char* first, char* last, int total, Entry* entries){
    int i,k=0, successOrFail= 0;
    char firstNameCopy[20];
    char lastNameCopy[20];
    char strPhoneCopy[13];
    for(i=0; i < total-1; i++){
        // checking if the element at that index is not empty(all char* ends with a null character \0)
        if(strlen(entries[i].firstName)!=0){
        	
            // since strcmp only takes two constant char*, strcpy has to be used.
            strcpy(firstNameCopy, entries[i].firstName);
            strcpy(lastNameCopy,entries[i].lastName);
            strcpy(strPhoneCopy, entries[i].strPhone);
            // returns 0(false) if the two strings are equal.I don't know know why they decided to do that, but
            // we have to explicitly check that the string are equal by comparing the return value of the strcmp() to 0. It would have made more sense if the strcmp() return 1 if the strings are equal, but whatever.
            if(strcmp(lastNameCopy, last)==0 && strcmp(firstNameCopy, first)==0){
                successOrFail = 1;   
            }else {
            	entries[k] = entries[i];	
            	k++;
            }
        }  
    }
    return successOrFail;
}
//finding a phone number with matching name
int findPhone(char* first, char* last, Entry* entries, int total){
	int i, foundOrNot=1;
	for(i= 0; i< total-1; i++){
		if(strlen(entries[i].firstName)!=0){
			if(strcmp(first, entries[i].firstName)+ strcmp(last, entries[i].lastName)==0){
				printf("here is %s %s\'s phone number:%s\n", first, last, entries[i].strPhone);
				foundOrNot= 1;
			}
			else foundOrNot = 0;
		}
	}
	return foundOrNot;
}

// alphabetizing the entries by first or last name
void alphabetize(char* firstOrLast, Entry* entries, int total){
	//allocating dynamic memory for arrays
	char* tempName1[1];
	char* tempName2[1];
	Entry temp;
	int k,j;
	if(strcmp(firstOrLast, "first")==0){
	// code to be executed when the user input "first"			
		for(j=0; j< total-2; j++){
			for(k= j+1; k< total-1;k++){
				if(strlen(entries[j].firstName)+strlen(entries[k].firstName)!=0){// check that the element is not empty
				strcpy(tempName1, entries[j].firstName);
				strcpy(tempName2, entries[k].firstName);
					if(strcmp(tempName1,tempName2 )>0){
						temp = entries[j];
						entries[j]= entries[k];
						entries[k]= temp;
					}
				}	
			}	
		}		
	}
	else if(strcmp(firstOrLast, "last")==0){
		// code to be executed when the user input "last"
		for(j = 0; j < total-2; j++){
			for(k=j+1; k<total-1;k++){
				if(strlen(entries[j].lastName)+ strlen(entries[k].lastName)!=0){
					strcpy(tempName1, entries[j].lastName);
					strcpy(tempName2, entries[k].lastName);
					if(strcmp(tempName1,tempName2 )>0){
						temp = entries[j];
						entries[j]= entries[k];
						entries[k]= temp;
					}
				}	
			}	
		}
	}
	else{
		// code to be executed when the user enter neither
		printf("Invalid request. Please enter \"first\" or \"last\" \n");
		
		
	}
	
}

