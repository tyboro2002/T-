#include <stdio.h>
#include <stdlib.h>

int main() {
	printf("hello world!, this is a tpp default program.\n");
	int x=5;
	int b=x;
	x=5;
	int y=5;
	int z=5;
if (x){
	printf("hey I passed the test");
}else if (y){
	printf("oof i passed the second one");
}
else if (z){
	printf("oof i passed the third one");
}
else{
	printf("shoot I didnt pass the test");
}
	return(b);
    return 0;
}