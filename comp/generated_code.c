#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main( int argc, char* argv[]) {
	printf("hello world!");
	printf(", this is a tpp default program.\n");
	printf("whats your age?: ");
	int inputVariable = 0;
	scanf("%d",&inputVariable);
	printf("you say you are: ");
	printf("%d\n",inputVariable);
if (inputVariable >= 100){
	printf("oldie but goldy!\n");
}else if (inputVariable >= 20){
	printf("old enough\n");
}
else if (inputVariable > 3){
	printf("almost old enough to use this\n");
}
else{
	printf("oof so young\n");
}
	int x = 8 + 5 * 10 - 30 / 5;
	int b = x;
	x = 5;
	int y = 9;
	int z = 63;
	int a = 5;
	a ^= 3;
	int k = (10 - 2 * 3) / 2;
{
	x = 9000;
	printf("data dump of variables: (in scope) \n");
	printf("the variable inputVariable contains: %d\n",inputVariable);
	printf("the variable x contains: %d\n",x);
	printf("the variable b contains: %d\n",b);
	printf("the variable y contains: %d\n",y);
	printf("the variable z contains: %d\n",z);
	printf("the variable a contains: %d\n",a);
	printf("the variable k contains: %d\n",k);
}
	int counter = inputVariable;
while( counter ){
	printf("%d\n",counter);
	counter -= 1;
}
	printf("data dump of variables: (in main) \n");
	printf("the variable inputVariable contains: %d\n",inputVariable);
	printf("the variable x contains: %d\n",x);
	printf("the variable b contains: %d\n",b);
	printf("the variable y contains: %d\n",y);
	printf("the variable z contains: %d\n",z);
	printf("the variable a contains: %d\n",a);
	printf("the variable k contains: %d\n",k);
	printf("the variable counter contains: %d\n",counter);
	return(inputVariable);
    return 0;
}