#include "MusicLibrary.hpp"
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>

using namespace std;
/**
 * Copyright 2020 Kelly Ellis,
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Purpose: Acts as the music library that holds all of the album objects
 * and contains methods for adding/removing albums from the library, as
 * well as methods for outputting the library to a json file and restoring
 * a library from a saved json file.
 *
 * @author Kelly Ellis klellis4@asu.edu
 *	   Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version February 2020
 * Modification: February 6, 2020.
 */

/**
 * Generic constructor that initializes the library from a saved json file
**/
MusicLibrary::MusicLibrary(){
   initLibraryFromJsonFile("albums.json");
}

// MusicLibrary destructor
MusicLibrary::~MusicLibrary() {
   media.clear();
}

/**
 * Adds an album to the MusicLibrary
 *
 * @param Album album - The album object to be added.
**/
void MusicLibrary::addAlbum(Album album) {
   cout << "Adding: " << album.title << endl;
   media.insert(pair<string, Album>(album.title, album));
}

bool MusicLibrary::addAlbum(Json::Value albObj) {

    bool ret = false;
    Album *alb = new Album(albObj);
    media.insert(pair<string, Album>(alb->title, *alb));
    cout << "Adding album: " << alb->title << endl;
    ret = true;
    return ret;
}

/**
 * Removes an album from the library.
 *
 * @param string album - The name of the album that is to be removed.
**/
bool MusicLibrary::removeAlbum(string album) {
   bool ret = false;
   // iterate over list of albums, remove one that matches album passed in
   map<string, Album>::iterator itr = media.begin();
   while (itr != media.end()) {
	if (itr->second.title == album) {
		map<string, Album>::iterator toErase = itr; // temp iter
		++itr;
		media.erase(toErase);
	} else {
		++itr;
	}
   }
   ret = true;
   return ret;
}

/**
 * Initiates the GUI MediaBrowser with a saved library from a json file.
 *
 * @param string jsonFileName - the name of the json file that contains the library.
 * @return bool - Returns true if the library is initialized, false otherwise
**/
bool MusicLibrary::initLibraryFromJsonFile(string jsonFileName) {
   bool ret = false;
   string library;
   string line;

   std::ifstream json;
   json.open(jsonFileName); // Open the json file
   while (json) {
	getline(json, line);
        library = library + line; // Add contents to string
   }
   json.close();
   // Create a new album out of that json string
   Album *med = new Album(library);
   Json::Value album;
   album = med->toJson();
   addAlbum(album);
   ret = true;
   return ret;
}

/**
 * Saves the MusicLibrary to a json file.
 *
 * @param string jsonFileName - Name of the file to save to
 * @param string jsonString - The json string of the library
 * @return bool - Returns true if the library was saved, false otherwise
**/
bool MusicLibrary::toJsonFile(string jsonFileName, string jsonString) {
   bool ret = false;

   ofstream fout;
   fout.open(jsonFileName); // Open json file to save to
   fout << jsonString; // Write jsonString to file
   fout.close();
   return ret;

}
