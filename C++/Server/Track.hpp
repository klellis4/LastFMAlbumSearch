#include <string>
#include <cmath>
#include <json/json.h>
#include <vector>

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
 * Purpose: Serves as the Track header that defines all objects/variables and methods
 * for the Track.cpp class.
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 *
 * @author Kelly Ellis klellis4@asu.edu
 * @version February 2020
 * Modification: February 6, 2020.
 */

class Track {
protected:
public:

   // Variables/objects
   string trackName;
   string artist;
   string album;
   string rank;
   string rank2;
   string duration;

   // Methods
   Track();
   Track(const Json::Value& jsonObj);
   Json::Value toJson();
   ~Track();
};

