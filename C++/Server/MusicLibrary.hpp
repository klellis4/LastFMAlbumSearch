#include "Album.hpp"
#include <string>
#include <map>
#include <vector>
#include <json/json.h>

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
 * Purpose: The header file that defines all of the attributes and
 * methods for the MusicLibrary class
 *
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 *	   Kelly Ellis klellis4@asu.edu
 * @version February 2020
 * Modification: February 6, 2020.
 */

class MusicLibrary {

public:
   // Variables/Objects
   std::map<std::string, Album> media;

   // Methods
   MusicLibrary();
   ~MusicLibrary();
   void addAlbum(Album album);
   bool addAlbum(Json::Value albObj);
   bool removeAlbum(string album);
   bool initLibraryFromJsonFile(string jsonFileName);
   bool toJsonFile(string jsonFileName, string jsonString);
   string serviceInfo();
};
