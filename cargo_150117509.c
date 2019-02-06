// MUSTAFA ABDULLAH HAKKOZ - 150117509

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 6
char forbiddenItems[SIZE][50];
unsigned int customers[500][2];					
double transactions[500][730];

int searchCustomer(int customerNumber);
void getContent(char content[], double *money);
int checkFinish(char stringToCheck[]);
int isEmpty(char *s);												//checks the string if it consists only spaces for checkFinish function
void removeForbiddens(char content[], int itemIndex);
int searchWord(char str1[], char str2[]);
void reconstruct(char stringToConstruct[], int start,int end);
void seperateItem(char wholeString[], int *itemCount, char item[]);
void printCustomers();

int main(void)
{

    char tempStr[50], content[500];   
    int i,j,k=0,a,exist, cNumber, start, currentCustomerIndex;
    

    
    printf("Hello Admin Please Enter the Forbidden Items:\n");
    
    while(k<SIZE){																	//read forbidden items and save them in to forbiddenItems array
    	fgets(tempStr, 50, stdin);													
	    for (i=0; i<strlen(tempStr);i++){											//replace spaces with underscore sign 
	    	    if(tempStr[i]==' '){
	    	    	tempStr[i]='_';
				}	    
		}
		strcpy(forbiddenItems[k],tempStr);											
		k++;
	}
	    	
	printf("Do you want to start the system? 1 --> Yes, 0 --> No:\n");
	scanf("%d",&start);
	
	if(start==1){
		printf("Welcome to Cargo Tracking System\n");
		do{
			double *money,moneyInit=0;												//create new money pointer for each transaction
   			 money=&moneyInit;
		
			printf("Please Enter the Customer Number: \n");
			scanf("%d",&cNumber);											
			fflush (stdin);															//corrected scanf to work with fgets like this: https://stackoverflow.com/questions/10156696/fgets-not-waiting-for-input
			if(cNumber>0){
				exist=searchCustomer(cNumber);										//search if customer number entered by user exist in customers array or not
	
				if(exist==-1) {														//add new customer number to customers array if it doesnt exist yet
					for(j=0;j<500;j++){
						if(*(*(customers + j)+0)==0){
							customers[j][0]=cNumber;
							currentCustomerIndex=j;									//get the currentCustomerIndex value if it DOESNT EXIST
							break;
						}	
					}
				}
			
				else currentCustomerIndex=exist;									//get the currentCustomerIndex value if it EXISTS
			
				printf("Please Enter the Content of the Cargo : ");
				fgets(content, 500, stdin);
				getContent(content,money);
				if (*money>0){														//if transaction is valid add it to customers and transactions arrays
					customers[currentCustomerIndex][1]++;
					for(a=0;a<730;a++){
						if(transactions[currentCustomerIndex][a]==0){
							transactions[currentCustomerIndex][a]=*money;						
							break;
						}	
					}
				}
			}
		}
		while(cNumber>0);
		
		if(cNumber<0) printCustomers();												//print all customers and their transactions if customer number is negative
	}
    	
	return 0;  
}

int searchCustomer(int customerNumber) {
	int i;
	for(i=0;i<500;i++){
		if(*(*(customers + i)+0)==customerNumber) return i;
		else return -1;
	}
}

void getContent(char content[], double *money){
	int itemCount,price,total;
	char item[50];

	
	if(checkFinish(content)==0){																	//stopping condition
		printf("\nprice %f\n",*money);
		return;															
	}															
	else{
		removeForbiddens(content,SIZE-1);
		seperateItem(content, &itemCount, item);
		
		if(itemCount>0){
			printf("\nenter the approximate value for one of %d %s\n",itemCount,item);				// read value and calculate price for each item
			scanf("%d",&price);
			fflush (stdin);
			total=price * itemCount;
			printf("total: %d", total);			
			*money+=total;	
			printf("money: %f", *money);
			
		}	

		else printf("Price for item %d %s will not be calculated because of having invalid number of items.");
		
		getContent(content,money);																		//recursive step
	}
	
}

int checkFinish(char stringToCheck[]){
	if(*stringToCheck=='\0' || isEmpty(stringToCheck)==1)
		return 0;
	return 1;
}

int isEmpty(char *s)													//checks the string if it consists only spaces for using in checkFinish function
{
  while (*s) {
    if (!isspace(*s))
      return 0;
    s++;
  }
  return 1;
}

void removeForbiddens(char content[], int itemIndex){
	int found;
	if(itemIndex<0)	return;														//stopping condition
	
	else {	
		found=searchWord(content,forbiddenItems[itemIndex]);		
		if(found==1){
			removeForbiddens(content,itemIndex);								//recursive step for repetitive forbidden items
		}
																																	
		if(found==0){										
			removeForbiddens(content,itemIndex-1);								//recursive step for other forbidden items
		}
	}
}

int searchWord(char str1[], char str2[]){
	int len1=strlen(str1)-1;											//dont count '/0' characters
	int len2=strlen(str2)-1;

	int i,j,k,found,startIndex,endIndex,endOfNum,startOfNum;
	
	for(i=0;i<=len1-len2;i++){											//find str2 in str1
		found = 1;
        for(j=0; j<len2; j++){											//compare str2 to str1
            if(str1[i + j] != str2[j]){
                found = 0;
                break;
            }
        }
        
		if(found == 1){                                             	  //str2 is found
        	k=i;
        	break;
		}															 
	}
        
	if(found == 1){                                             	  
    	endIndex=k+len2-1;											 	 //assign end point of str2 to endIndex
	
		for(k-1;str1[k-1]==' ';k--){									  //find end point of count number of the content
			endOfNum=k-2;													
			
		}
		
		for(k=endOfNum;str1[k]>='0' && str1[k]<='9';k--){				//find start point of count number of the content
			startOfNum=k;
			
		}
		
    	startIndex=startOfNum;													
        reconstruct(str1,startIndex,endIndex);							
    	return 1;	
	} 
    else 
		return 0;
	
}

void reconstruct(char stringToConstruct[], int start,int end){					
	char tempStr[500];
	
	strcpy(tempStr, stringToConstruct);									
	strncpy(&stringToConstruct[start],&tempStr[end+1],500-end);				//remove interval[start,end] from string
}

void seperateItem(char wholeString[], int *itemCount, char item[]){	
	int count=0,i,j,intNumber, len;
	char word[50],number[10];
	
	while(wholeString[count]==' ') count++;									//ignore spaces at the beggining of the content
	
	for(i=0;wholeString[count]!=' ';i++){									//get the count number of first item of the content
		number[i]=wholeString[count];
		count++;
	}
	number[i+1]='\0';														
	intNumber=atoi(number);
	*itemCount=intNumber;													//save item count to itemCount pointer

	while(wholeString[count]==' ') count++;									//ignore spaces between count number and corresponding item
	
	for(j=0;isalpha(wholeString[count]);j++){									//get the item name
		word[j]=wholeString[count];
		count++;
	}		
	word[j+1]='\0';
	len=strlen(word);
	strncpy(item,word,len);													//save item to item[] array
	reconstruct(wholeString,0,count);										//remove first item and its count number
	
}

void printCustomers(){
	int i,j;
	
	for(i=0;i<500;i++){
		if(customers[i][0]>0){
			printf("\nUser %d has sent %d cargos price of each is as follows",customers[i][0],customers[i][1]);
			for(j=0;j<730;j++){
				if(transactions[i][j]>0){
					printf("\nprice for cargo %d -> %f   ",j+1,transactions[i][j]);
				}
			}
		}
	}
}
