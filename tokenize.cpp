#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "dictionaries"

struct Token
{
	public:
		enum TokenType{ Number, String, List, Matrix, NumVar, StrVar, ListVar, MatVar, Grammar, Operation };
		TokenType type;
		
		string contents;
		
		Token(){
			//type = NULL;
			contents = "";
		}
};

bool isNum(char c){
if(48<=c&&c<=57)return true;//ASCII
if(c=='.')return true;
return false;
}

vector<Token> tokenize(string code)
{
	vector<Token> tokens;
	int numTokens = 0;
	//DictionaryNode dict;
	int index = 0;
	size_t codeLength = code.length();
	
	while(index < codeLength)
	{
		if(code[index] == 0 || code[index] == 13 || code[index] == 10)
			break;
		if(index >= codeLength)
			break;
		Token t;
		char character=code[index];
		if(character=='"'){
			index++;
			size_t startI = index;
			while(code[index] != '"' && index<codeLength)
			{ index++; }
			size_t endI = index;
			t.type=Token::String;
			t.contents=code.substr(startI,endI-startI);
		}else if(isNum(character)){
			size_t startI = index;
			while(index<codeLength && isNum(code[index])){
				index++;
			}
			size_t endI = index;
			t.type=Token::Number;
			t.contents=code.substr(startI,endI-startI);
		}else{
			int startIndex = index;
			int lastTokenIndex = -1;
			int lastIndex = index+1;
			int tokenIndex = 0;
			int len = 1;
			while(index<codeLength)
			{
				while(keywords[tokenIndex].substr(0, len) < code.substr(startIndex, len))
				{
					tokenIndex++;
				}
				if(tokenIndex == lastTokenIndex)
					break;
				if(keywords[tokenIndex] == code.substr(startIndex, len))
				{
					lastTokenIndex = tokenIndex;
					lastIndex = index+1;
				}
				len++;
				index++;
			}
			t.type=Token::Grammar;
			if(lastTokenIndex == -1)
			{
				t.contents="?";
			}
			else
				t.contents=keywords[lastTokenIndex];
			index = lastIndex;
		}
		tokens.push_back(t);
		/*		
		dict = TokenDictionary.getChild(code.charAt(index));
		validLength = 0;
		length = 1;
		
		while(dict != NULL and index+length < codeLength)
		{
			if(dict.isValid)
				validLength = length;
			dict = dict.getChild(code.charAt(index+length))//could factor addition out of the loop
			length+=1; //adding more may be appropriate with quotes
		}
		tokens[numTokens] = new Token(code.substring(index,validLength));
		numTokens++;
		index += validLength;
		length = 1;
		validLength = 0;
		*/
	}
	return tokens;
}

vector<Token> reduce(vector<Token> tokens)
{
	int parenCount = 0;
	for(int i=0;i<tokens.size();i++)
	{
		Token t = tokens[i];
		int tLen = t.contents.length();
		for(int j=0;j<tLen;j++)
		{
			if(t.contents[j] == '(')
				parenCount++;
			else if (t.contents[j] == ')')
				parenCount--;
			//match with dictionary:
			
						
		}
	}
	return tokens;
}

int main(int argc, char* argv[])
{
	char acLine[1024];
	while (fgets(acLine, 1024, stdin) != NULL)
	{
		vector<Token> tokens = tokenize(acLine);
		for(int i=0;i<tokens.size();i++)
		{
			cout << tokens[i].contents;
		}	cout << endl;
	}
	/*	
	reduce(tokens);
	for(int i=0;i<tokens.size();i++)
	{
		cout << tokens[i].contents << " ";
	}	cout << endl;
	*/
	return 0;
}
