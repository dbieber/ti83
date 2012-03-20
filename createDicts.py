import xml.parsers.expat
import string

out = open('dictionaries', 'w');
out.write("#ifndef INCLUDED_DICTIONARIES\n")
out.write("#define INCLUDED_DICTIONARIES\n")

keywords = []
variables = []
tokenTypes = []
commands = []

def start_element(name, attrs):
	if (name == "variable"):
		keyword = attrs['name'];
		if (keyword not in keywords):
			keywords.append(keyword)
		if (keyword not in variables):
			variables.append(keyword)
	if (name == "command"):
		newKeywords = string.split(attrs['format'], '$')
		argTypes = string.split(attrs['argt'], ',')
		retType = attrs['rett'];
		funcName = attrs['name'];
		priority = attrs['priority'];
		numArgs = len(argTypes)
		if(argTypes[0] == 'None' or argTypes[0] == 'Void' or argTypes[0] == ''):
			numArgs = 0;
		
		i = 0; #Number of keyword in command
		newCommand = [];
		for keyword in newKeywords:
			if (keyword == ''):
				if (i<numArgs):
					newCommand.append('$'+argTypes[i])
				i+=1
				continue
			if (keyword not in keywords):
				keywords.append(keyword)
			newCommand.append(keyword);
			if (i<numArgs):
				newCommand.append('$'+argTypes[i])
			i+=1
		commands.append([newCommand, retType, funcName, priority]);

p = xml.parsers.expat.ParserCreate()
p.StartElementHandler = start_element
p.ParseFile(open('keywords.xml', 'r'))

keywords.sort();
variables.sort();

print "*** KEYWORDS ***"
numK = 0;
str = "const string keywords[] = {"
for keyword in keywords:
	numK+=1
	str += ("\"%s\"") % keyword
	if(numK < len(keywords)):
		str +=","
str += "};\n"
str += "const int NUM_KEYWORDS = %d;\n\n" % numK
out.write(str)

# print "*** VARIABLES ***"
# str = ""
# for var in variables:
	# str += ("%16s") % var
# print str

print "*** COMMANDS ***"
longest = 0;
for i in range(0,len(commands)):
	for j in range(0,len(commands[i][0])):
		for k in range(0, numK):
			if(keywords[k] == commands[i][0][j]):
				commands[i][0][j] = k
				break
	if (len(commands[i][0])>longest):
		longest = len(commands[i][0])

commands.sort();

str = ("const int commands[][%d] = {") % (longest)
retStr = ("const int returnTypes[] = {")
cNum = 0
for command in commands:
	tokenSeq = command[0];
	retType = command[1];
	i=0
	str+="{"
	for token in tokenSeq:
		if(type(token) is int):
			str = str + ("%d")%(token)
		else:
			str = str + token[1:].upper()
			if(token[1:].upper() not in tokenTypes):
				tokenTypes.append(token[1:].upper())
		i+=1
		if(i<len(tokenSeq)):
			str+=","
	cNum+=1
	str+="}"
	if(retType.upper() not in tokenTypes):
		tokenTypes.append(retType.upper())
	retStr+=retType.upper();
	if(cNum < len(commands)):
		str+=","
		retStr+=","
str+="};\n"
retStr+="};\n"

str2 = "const int "
typeI = -len(tokenTypes)
for tokenType in tokenTypes:
	str2+="%s = %d" % (tokenType, typeI)
	typeI+=1
	if(typeI<0):
		str2+=","
str2+=";\n"
out.write(str2)
out.write(str)
out.write(retStr)
out.write("#endif\n")