/*
 * SymlinkCpp.cpp
 *
 *  Created on: Feb 24, 2012
 *      Author: Brian
 */

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "FileParser.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <io.h>
#include <stdio.h>
#include <string.h>

using namespace std;

class node
{
	public:
		char *tag;
		char *location;
		node *next;

		node(char* tag, char* location)
		{
			int size = 0;
			//Get Size of Tag and Copy It
			while(tag[size] != 0)
				size++;
			this->tag = (char*)malloc(size);
			for(int i = 0; i < size; i++)
			{
				this->tag[i] = tag[i];
			}
			this->tag[size] = 0;

			size = 0;
			//Get Size of Location and Copy It
			while(location[size] != 0)
				size++;
			this->location = (char*)malloc(size);
			for(int i = 0; i < size; i++)
			{
				this->location[i] = location[i];
			}
			this->location[size] = 0;
			next = 0;
		}
};

class list
{
	public:
		node *head;
		int size;

		list()
		{
			head = 0;
			size = 0;
		};

		list(node *head)
		{
			this->head = head;
			size = 0;
		}

		void addEntry(node *newNode)
		{
			newNode->next = head;
			head = newNode;
			size++;
			cout << "Added Node " << size - 1 << ":\n\tTag: " << newNode->tag << "\n\tLocation: " << newNode->location << endl;
		}
};

int main(int argc, char* argv[])
{
	char* newTag;

	if(argc >= 3 && strcmp(argv[1], "-g") == 0)
	{
		newTag = argv[2];
	}
	else
	{
		cerr << "Invalid Argument Format" << endl << "Please use: \"SymlinkCpp -g <game tag>\"" << endl;
		cin.get();
		exit(1);
	}

	//Initialize a list to hold our Entries.
	list* gameList;
	gameList = new list();
	char* ramdiskDirectory = 0;

	//Load the File from which we will load our Game Entries.
	ifstream gameListFile;
	gameListFile.open("list.txt");
	if(gameListFile.is_open() == false)
	{
		cerr << "Unable to open Game List File. Exiting." << endl;
		cin.get();
		exit(1);
	}

	int loopstatus = 0;
	for(int i = 0; !gameListFile.eof(); i++)
	{
		char line[1000];
		gameListFile.getline(line, 999);
		char* tag = 0;
		char* location = 0;

		FileParser parser;
		for(int j = 0; line[j] != 0; j++)
		{
			if(line[j] == '$')
			{
				//Check For Variables
				char ramdisk[] = {'R','A','M','D','I','S','K','='};
				int k;
				for(k = 0; line[k] != 0; k++)
				{
					if(line[k + j + 1] != ramdisk[k])
						break;
					if(k == 7)
					{
						k++;
						int size;
						for(size = 0; line[size + k + j + 1] != 0; size++){}
						ramdiskDirectory = (char*)malloc(size + 1);
						for(int l = 0; l < size; l++)
						{
							ramdiskDirectory[l] = line[l + k + j + 1];
						}
						break;
					}
				}

			}
			if(loopstatus == 0 && line[j] == '#')
			{
				//Begin Read of Tag
				cout << "I CAN HAZ BROKE" << endl;
				parser.parseTag(&tag, line, j);
				loopstatus = 1;
				continue;
			}
			if(loopstatus == 1 && line[j] == '#')
			{
				cerr << "Invalid Tag Formatting on Line " << i+1 << '.' << endl;
				loopstatus = 0;
				j--;
				continue;
			}
			if(loopstatus == 1 && line[j] == '|' && line[j-1] == '|')
			{
				//Being Read of Location
				cout << "I CANNOT HAZ BROKE" << endl;
				j += parser.parseLocation(&location, line, j);
				gameList->addEntry(new node(tag, location));
				loopstatus = 0;
			}
		}
	}

	//Begin checking to see which game is currently mounted.
	char* previousLink = 0;
	char* previousLocation = 0;
	char* previousLocation_LINK = 0;
	for(node* currentNode = gameList->head; currentNode != 0; currentNode = currentNode->next)
	{
		char searchdir[1000];
		int i, j;
		for(i = 0; ramdiskDirectory[i] != 0; i++)
			searchdir[i] = ramdiskDirectory[i];
		for(j = 0; currentNode->tag[j] != 0; j++)
			searchdir[i+j] = currentNode->tag[j];
		searchdir[i+j] = 0;

		cout << "Searching for: " << searchdir << endl;

		if(access(searchdir, 0) == 0)
		{
			struct stat status;
			stat(searchdir, &status);

			if(status.st_mode & S_IFDIR)
			{
				cout << "Directory Found: " << searchdir << endl;
				for(i = 0; searchdir[i] != 0; i++)
					continue;
				previousLink = (char*)malloc((sizeof(char) * i) + 1);
				for(j = 0; searchdir[j] != 0; j++)
					previousLink[j] = searchdir[j];
				previousLink[j] = 0;

				for(i = 0; currentNode->location[i] != 0; i++)
					continue;
				previousLocation = (char*)malloc((sizeof(char) * i) + 2);
				for(j = 0; currentNode->location[j] != 0; j++)
					previousLocation[j] = currentNode->location[j];
				searchdir[j++] = '\\';
				previousLocation[j] = 0;

				char* _LINK = (char*)"_LINK";
				for(i = 0; previousLocation[i] != 0; i++)
					continue;
				for(j = 0; _LINK[j] != 0; j++)
					continue;
				previousLocation_LINK = (char*)malloc((sizeof(char) * (i + j - 1)) + 2);
				for(i = 0; previousLocation[i] != 0; i++)
					previousLocation_LINK[i] = previousLocation[i];
				for(j = 0; _LINK[j] != 0; j++)
					previousLocation_LINK[i+j] = _LINK[j];
				previousLocation_LINK[i+j] = 0;

				cout << previousLink << endl << previousLocation << endl << previousLocation_LINK << endl;
			}
			else
			{
				cout << "???" << endl;
			}
		}
		else
			cout << "Unable to access: " << searchdir << endl;
	}
	if(previousLink == 0)
	{
		cout << "No Previous Link" << endl;
	}
	else
	{
		string sysin = "rmdir \"" + (string)previousLocation + "\" /S /Q";
		system(sysin.c_str());
		rename(previousLocation_LINK, previousLocation);
		sysin = "robocopy \"" + (string)previousLink + "\" \"" + (string)previousLocation + "\" /MIR";
		system(sysin.c_str());
		sysin = "rmdir \"" + (string)previousLink + "\" /S /Q";
		system(sysin.c_str());
	}

	char* newLink = 0;
	char* newLocation = 0;
	char* newLocation_LINK = 0;

	for(node* currentNode = gameList->head; currentNode != 0; currentNode = currentNode->next)
	{
		if(strcmp("unlink", newTag) == 0)
			exit(0);

		if(strcmp(currentNode->tag, newTag) == 0)
		{
			cout << "Tag Matched: " << currentNode->tag << endl;

			int i, j;
			for(i = 0; ramdiskDirectory[i]; i++)
				continue;
			for(j = 0; currentNode->tag[j]; j++)
				continue;
			newLink = (char*)malloc(sizeof(char) * (i+j) + 1);
			for(i = 0; ramdiskDirectory[i]; i++)
				newLink[i] = ramdiskDirectory[i];
			for(j = 0; currentNode->tag[j]; j++)
				newLink[i + j] = currentNode->tag[j];
			newLink[i + j] = 0;

			for(i = 0; currentNode->location[i]; i++)
				continue;
			newLocation = (char*)malloc(sizeof(char) * i + 1);
			for(i = 0; currentNode->location[i]; i++)
				newLocation[i] = currentNode->location[i];
			newLocation[i] = 0;

			char* _LINK = (char*)"_LINK";
			for(i = 0; newLocation[i]; i++)
				continue;
			for(j = 0; _LINK[j]; j++)
				continue;
			newLocation_LINK = (char*)malloc((sizeof(char) * (i + j - 1)) + 2);
			for(i = 0; newLocation[i]; i++)
				newLocation_LINK[i] = newLocation[i];
			for(j = 0; _LINK[j]; j++)
				newLocation_LINK[i+j] = _LINK[j];
			newLocation_LINK[i+j] = 0;
		}
	}
	if(newLink == 0)
	{
		cout << "No Tag Matched." << endl;
	}
	else
	{
		string sysin = "robocopy \"" + (string)newLocation + "\" \"" + (string)newLink + "\" /MIR";
		system(sysin.c_str());
		rename(newLocation, newLocation_LINK);
		sysin = "mklink /D \"" + (string)newLocation + "\" \"" + (string)newLink + "\"";
		system(sysin.c_str());
	}
}
