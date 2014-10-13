/*
 * FileParser.cpp
 *
 *  Created on: Feb 25, 2012
 *      Author: Brian
 */

#include <stdlib.h>
#include <iostream>
#include "FileParser.h"

using namespace std;

int FileParser::parseTag(char** tag, char* line, int offset)
{
	if(line[offset] != '#')
	{
		cerr << "Tag Parser given invalid tag. Returning empty string." << endl;
		*tag = (char*)malloc(sizeof(char));
		return 0;
	}

	int size = 0;
	for(int i = offset+1; line[i] != 0; i++)
	{
		if(line[i] == '|' && line[i+1] == '|')
			break;
		if(line[i] != '\t')
			size++;
	}

	*tag = (char*)malloc(sizeof(char) * size);
	for(int i = 0; i < size; i++)
	{
		(*tag)[i] = line[i + offset + 1];
	}
	(*tag)[size] = 0;

	return size;
}

int FileParser::parseLocation(char** location, char* line, int offset)
{
	if(line[offset] != '|' || line[offset-1] != '|')
	{
		cerr << "Location Parser given invalid location. Returning empty string." << endl;
		*location = (char*)malloc(sizeof(char));
		return 0;
	}

	int size = 0;
	for(int i = offset+1; line[i] != 0; i++)
	{
		if(line[i] == '#')
			break;
		if(line[i] != '\t')
			size++;
	}

	*location = (char*)malloc(sizeof(char) * size);
	for(int i = 0; i < size; i++)
	{
		(*location)[i] = line[i + offset + 1];
	}
	(*location)[size] = 0;

	return size;
}
