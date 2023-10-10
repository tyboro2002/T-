say("i recieved: ");
if(tppCount != 2){
	shout("not exact 1 input");
	exit(1);
}

if(tppInp(1) == "exit"){
	shout("exit");
	shout("gramarly: exit(<exitCodeExpresion>);");
	shout("exit with the exit code given between the ()");
}elif(tppInp(1) == "say"){
	shout("say");
	shout("gramarly: say(<\"string\">);");
	shout("print the string given between () without putting a newline after it");
}elif(tppInp(1) == "shout"){
	shout("shout");
	shout("gramarly: shout(<\"string\">);");
	shout("print the string given between () with putting a newline after at");
}elif(tppInp(1) == "return"){
	shout("return");
	shout("gramarly: return(<returnValueExpresion>);");
	shout("return out of the function or main code with the value between ()");
}elif(tppInp(1) == "if"){
	shout("if");
	shout("gramarly: if(<expresion>){");
	shout("}");
	shout("if the expresion results in a truthy value the scope wil be executed otherwise we jump over the scope");
}elif(tppInp(1) == "elif"){
	shout("elif");
	shout("gramarly: elif(<expresion>){");
	shout("}");
	shout("if the expresion results in a truthy value the scope wil be executed otherwise we jump over the scope");
	shout("needs to be put after a if or another elif");
}elif(tppInp(1) == "else"){
	shout("else");
	shout("gramarly: else(<expresion>){");
	shout("}");
	shout("if al previous if and or elif's fail this scope wil be executed");
}elif(tppInp(1) == "request"){
	shout("request");
	shout("gramarly: request(<variable>);");
	shout("request the user for input and put it in the variable given between ()");
	shout("!the variable needs to be declared before the use here!");
}elif(tppInp(1) == "tppInp"){
	shout("tppInp");
	shout("gramarly: tppInp(<integer>)");
	shout("the <integer> th command line input");
}elif(tppInp(1) == "tppCount"){
	shout("tppCount");
	shout("gramarly: tppCount");
	shout("the amount of command line input");
}elif(tppInp(1) == "varDump"){
	shout("varDump");
	shout("gramarly: varDump;");
	shout("print out the values of all arguments");
	shout("print out format: ");
	shout("    the variable <name> contains: <value>");
}else{ # TODO add while
	shout("not implemented");
}
