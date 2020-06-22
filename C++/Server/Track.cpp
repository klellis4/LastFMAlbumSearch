#include "Track.hpp"
#include <iostream>
#include <stdlib.h>

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
 * Purpose: Represents a Track object from an Album and contains all of the 
 * track information, as well as a method to retrieve all of that information
 * from a json Value.
 *
 * @author Kelly Ellis klellis4@asu.edu
 * @version February 2020
 * Modification: February 6, 2020.
 */

// Generic constructor
Track::Track(){
   trackName = "";
   artist = "";
   album = "";
   rank = "";
   rank2 = "";
   duration = "";
}

/**
 * Track constructor that sets up a new Track object by parsing the Album jsonObj passed in.
 *
 * @param Json::Value jsonObj - The Album json Value that the track belongs to
 * @return Track - The Track object that was created.
**/
Track::Track(const Json::Value& jsonObj){

   // Parse the Album jsonObj and get track info
   trackName = jsonObj["name"].asString();
   Json::Value artistObj = (!(jsonObj["artist"].isNull()) && jsonObj["artist"].isObject()) ?
	jsonObj["artist"] : Json::Value();
   artist = artistObj["name"].asString();
   Json::Value rankObj = (!(jsonObj["@attr"].isNull()) && jsonObj["@attr"].isObject()) ?
	jsonObj["@attr"] : Json::Value();
   rank = rankObj["rank"].asString();
   rank2 = jsonObj["rank"].asString();
   duration = jsonObj["duration"].asString();
   
}


Json::Value Track::toJson() {
   Json::Value jsonTrack;
   jsonTrack["name"] = trackName;
   jsonTrack["artist"] = artist;
   jsonTrack["rank"] = rank;
   jsonTrack["duration"] = duration;

   return jsonTrack;
}

Track::~Track() {
   //cout << "Track destructor.\n";
   trackName = "";
   artist = "";
   album = "";
   rank = "";
   duration = "";
}

