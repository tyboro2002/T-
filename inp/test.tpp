/* say welcome */
say ("hello world!"); #prints hello world without a newline after it
shout(", this is a tpp default program."); #prints the rest of the welcome message with a newline after it
# hey this is al needed to be ignored
/*
	multiline
	comment
*/

/* ask for the age of the user */
say("whats your age?: ");
inputVariable = 0; #initialise variable for request
request(inputVariable); #request the value for the variable
say("you say you are: ");
shout(inputVariable); #print the variable with a newline after it

/* print what we think of the age */
if (inputVariable>=100) {
	shout("oldie but goldy!");
} elif(inputVariable>=20){
	shout("old enough");
}elif(inputVariable>3){
	shout("almost old enough to use this");
} else{
	shout("oof so young");
}

/* some testing for variables */
x = 8;
b = x;
x = 5;
y = 9;
z = 63;

{
	x = 9000;
	#mama = 0;
	dumpVar("in scope"); #TODO doesnt work with variables declared in scopes at the moment
}

/* print the vars for testing */
dumpVar("in main");

/* return the age of the user */
return (inputVariable);