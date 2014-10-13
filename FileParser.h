/*
 * FileParser.h
 *
 *  Created on: Feb 25, 2012
 *      Author: Brian
 */

#ifndef FILEPARSER_H_
#define FILEPARSER_H_

class FileParser
{
	public:
		static int parseTag(char** tag, char* line, int offset);
		static int parseLocation(char** location, char* line, int offset);
};

#endif /* FILEPARSER_H_ */
