#include "Album.hpp"
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
 * Purpose: Album represents an album object in the music library. It contains
 * the album's attributes such as title, artist, etc., as well as the album's list
 * of tracks. All of the album info is retrieved from the json string returned by 
 * the last fm search.
 *
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 *	   Kelly Ellis klellis4@asu.edu
 * @version February 2020
 * Modification: February 6, 2020.
 */

// Generic constructor
Album::Album(){
   title = "";
   author = "";
   album = "";
   genre = "";
   typeExt = "";
   url = "";
   summary = "";
   image = "";
}


Album::Album(const Json::Value& jsonObj){

/*
   Json::Value::Members mbr = jsonObj.getMemberNames();
   for(vector<string>::const_iterator i = mbr.begin(); i!= mbr.end(); i++){
      Json::Value jsonM = jsonObj[*i];
      if(*i=="title"){
         title = jsonM.asString();
      }else if(*i=="author"){
         author = jsonM.asString();
      }//else if(*i=="album"){
         //album = jsonM.asString();
      //}else if(*i=="genre"){
         //genre = jsonM.asString();
      //}else if(*i=="typeExt"){
         //typeExt = jsonM.asString();
      //}
   }
*/

   title = jsonObj["name"].asString();
   author = jsonObj["artist"].asString();
   url = jsonObj["url"].asString();
   summary = jsonObj["summary"].asString();
   image = jsonObj["image"].asString();
   runTime = stoi(jsonObj["runTime"].asString());

   // genres
   Json::Value genreArr(Json::arrayValue);
   genreArr = jsonObj["genre"];
   for (int i = 0; i < genreArr.size(); i++) {
	Json::Value genreObj = genreArr[i];
        genreList.insert(pair<int, string>(i, genreObj.asString()));
   }

   // Tracks
   Json::Value trackArr(Json::arrayValue);
   trackArr = jsonObj["tracks"];
   
   for (int j = 0; j < trackArr.size(); j++) {
	Json::Value trackObj = trackArr[j];
	Track *track = new Track(trackObj);
	// Format the track duration time and add it to the album runTime
	runTime = runTime + std::stoi(track->duration.c_str());
	trackList.insert(pair<int, Track>(j, *track));
   }

}


/**
 * Constructor for Album that takes in a json String and reads in all the attributes for
 * and album object
 *
 * @param string jsonString - The string returned by the last.fm search from MusicBrowser.
 * @return Album - The album object created
**/


Album::Album(string jsonString) {

   Json::Reader reader;
   Json::Value jobj;
   bool success = reader.parse(jsonString, jobj, false);
   if (success) {

	// Get json object 'album' from json string returned by last fm search
	Json::Value albVal = (!(jobj["album"].isNull()) && jobj["album"].isObject()) ?
		 jobj["album"] : Json::Value();
	
	// Get album attributes from album object
	title = (!(albVal["name"].isNull()) && albVal["name"].isString()) ? 
		 albVal["name"].asString() : "Unknown Album Name";
	author = (!(albVal["artist"].isNull()) && albVal["artist"].isString()) ?
		 albVal["artist"].asString() : "Unknown Artist";
	Json::Value sumVal = (!(albVal["wiki"].isNull()) && albVal["wiki"].isObject()) ? 
		 albVal["wiki"] : Json::Value();
	summary = sumVal["summary"].asString();

	// Get image array, then get the extralarge size image url from that array (3rd element)
	Json::Value imgArr = (!(albVal["image"].isNull()) && albVal["image"].isArray()) ?
		 albVal["image"] : Json::Value();
	Json::Value image3 = imgArr[3];
	image = image3["#text"].asString();
	url = (!(albVal["url"].isNull()) && albVal["url"].isString()) ?
		 albVal["url"].asString() : "No URL";

	// Get genre (tags) object, then get genre array from that
	Json::Value genreVal = (!(albVal["tags"].isNull()) && albVal["tags"].isObject()) ?
		 albVal["tags"] : Json::Value();
	Json::Value genreArr = (!(genreVal["tag"].isNull()) && genreVal["tag"].isArray()) ?
		 genreVal["tag"] : Json::Value();
	// Put genres into genreList
	for (int i = 0; i < genreArr.size(); i++) {
		Json::Value genreObj = genreArr[i];
		genreList.insert(pair<int, string>(i, genreObj["name"].asString()));
	}
	
	Json::Value genres = genreArr[0];
	genre = genres["name"].asString();

	// Get array of tracks
	Json::Value trackVal = (!(albVal["tracks"].isNull()) && albVal["tracks"].isObject()) ?
		albVal["tracks"] : Json::Value();
	Json::Value trackArray = (!(trackVal["track"].isNull()) && trackVal["track"].isArray()) ?
		trackVal["track"] : Json::Value();

	runTime = 0;

	// Add tracks to trackList
	for (int j = 0; j < trackArray.size(); j++) {
		Json::Value trackObj = trackArray[j];
		Track *track = new Track(trackObj);
		// Format the track duration time and add it to the album runTime
		runTime = runTime + std::stoi(track->duration.c_str());
		trackList.insert(pair<int, Track>(j, *track));
	}
   }
}


//////////////////////////////////////////////////////////////////////

Json::Value Album::toJson() {
   Json::Value jsonAlbum;
   jsonAlbum["name"] = title;
   jsonAlbum["artist"] = author;
   jsonAlbum["url"] = url;
   jsonAlbum["summary"] = summary;
   jsonAlbum["image"] = image;
   jsonAlbum["runTime"] = to_string(runTime);

   // Add genreList
   Json::Value tmp(Json::arrayValue);
   map<int, string>::iterator itr;
   int i = 0;
   for (itr = genreList.begin(); itr != genreList.end(); itr++) {
	tmp[i] = itr->second;
        i++;
   }
   jsonAlbum["genre"] = tmp;

   // Add trackList
   Json::Value tList(Json::arrayValue);
   map<int, Track>::iterator trackItr;
   int j = 0;
   for (trackItr = trackList.begin(); trackItr != trackList.end(); trackItr++) {
       Json::Value tr = trackItr->second.toJson();
       tList[j] = tr;
       j++;
   }
   jsonAlbum["tracks"] = tList;

   return jsonAlbum;
}

/**
 * Album destructor
**/
Album::~Album() {
   title = "";
   author = "";
   album = "";
   genre = "";
   typeExt = "";
}

