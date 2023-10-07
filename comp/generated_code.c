#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main( int argc, char* argv[]) {
	printf("i recieved: ");
if (argc != 2){
	printf("not exact 1 input\n");
	exit(1);
}
if (strcmp(argv[1],"exit") == 0){
	printf("exit\n");
	printf("gramarly: exit(<exitCodeExpresion>);\n");
	printf("exit with the exit code given between the ()\n");
}else if (strcmp(argv[1],"say") == 0){
	printf("say\n");
	printf("gramarly: say(<\"string\">);\n");
	printf("print the string given between () without putting a newline after it\n");
}
else if (strcmp(argv[1],"shout") == 0){
	printf("shout\n");
	printf("gramarly: shout(<\"string\">);\n");
	printf("print the string given between () with putting a newline after at\n");
}
else if (strcmp(argv[1],"return") == 0){
	printf("return\n");
	printf("gramarly: return(<returnValueExpresion>);\n");
	printf("return out of the function or main code with the value between ()\n");
}
else if (strcmp(argv[1],"if") == 0){
	printf("if\n");
	printf("gramarly: if(<expresion>){\n");
	printf("}\n");
	printf("if the expresion results in a truthy value the scope wil be executed otherwise we jump over the scope\n");
}
else if (strcmp(argv[1],"elif") == 0){
	printf("elif\n");
	printf("gramarly: elif(<expresion>){\n");
	printf("}\n");
	printf("if the expresion results in a truthy value the scope wil be executed otherwise we jump over the scope\n");
	printf("needs to be put after a if or another elif\n");
}
else if (strcmp(argv[1],"else") == 0){
	printf("else\n");
	printf("gramarly: else(<expresion>){\n");
	printf("}\n");
	printf("if al previous if and or elif's fail this scope wil be executed\n");
}
else if (strcmp(argv[1],"request") == 0){
	printf("request\n");
	printf("gramarly: request(<variable>);\n");
	printf("request the user for input and put it in the variable given between ()\n");
	printf("!the variable needs to be declared before the use here!\n");
}
else if (strcmp(argv[1],"tppInp") == 0){
	printf("tppInp\n");
	printf("gramarly: tppInp(<integer>)\n");
	printf("the <integer> th command line input\n");
}
else if (strcmp(argv[1],"tppCount") == 0){
	printf("tppCount\n");
	printf("gramarly: tppCount\n");
	printf("the amount of command line input\n");
}
else if (strcmp(argv[1],"varDump") == 0){
	printf("varDump\n");
	printf("gramarly: varDump;\n");
	printf("print out the values of all arguments\n");
	printf("print out format: \n");
	printf("    the variable <name> contains: <value>\n");
}
else{
	printf("not implemented\n");
}
    return 0;
}