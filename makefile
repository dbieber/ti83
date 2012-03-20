tokenTest: tokenize.cpp dictionaries
	g++ -o tokenTest tokenize.cpp
dictionaries: createDicts.py keywords.xml
	python createDicts.py