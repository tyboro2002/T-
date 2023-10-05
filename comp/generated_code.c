#include <stdio.h>
#include <stdlib.h>

int main() {
	printf("hello world!, this is a tpp default program.\n");
	int inputVariable = 0;
	scanf("%d",&inputVariable);
	printf("%d\n",inputVariable);
	int x = 5;
	int b = x;
	x = 5;
	int y = 5;
	int z = 5;
if (inputVariable >= 10){
	printf("hey I passed the test\n");
}else if (inputVariable >= 5){
	printf("oof i passed the second one\n");
}
else if (inputVariable > 3){
	printf("oof i passed the third one\n");
}
else{
	printf("shoot I didnt pass the test\n");
}
	return(b);
    return 0;
}