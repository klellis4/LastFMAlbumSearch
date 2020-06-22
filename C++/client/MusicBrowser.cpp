#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated"

#include "MediaClientGui.cpp"
#include "../server/MusicLibrary.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Check_Browser.H>
#include <FL/Fl_PNG_Image.H>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <jsonrpccpp/client/connectors/httpclient.h>
#include "musiccollectionstub.h"
#include <thread>
#include <json/json.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>
#include <curl/easy.h>
#include <curlpp/Exception.hpp>
#include <curl/curl.h>

using namespace std;
using namespace jsonrpc;

std::string cmd;
void run(){
   system(cmd.c_str());
}

/**
 * Copyright (c) 2020 Kelly Ellis,
 * Software Engineering,
 * Arizona State University at the Polytechnic campus
 * <p/>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * <p/>
 * This program is distributed in the hope that it will be useful,
 * but without any warranty or fitness for a particular purpose.
 * <p/>
 * Please review the GNU General Public License at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 * see also: https://www.gnu.org/licenses/gpl-faq.html
 * so you are aware of the terms and your rights with regard to this software.
 * Or, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,USA
 * <p/>
 * Purpose: C++ FLTK client UI for Music management.
 * This class extends the Gui component class MediaClientGui and demonstrates
 * sample control functions that respond to button clicks and menu item selects.
 * This software is meant to run on Linux and MacOS using g++.
 * Students may use this source code as the basis for their solution to
 * the programming projects in Ser321 to create a distributed music browser.
 * <p/>
 * This example depends on the library curlpp to download a file or make
 * a URL request. This should be used in problem solutions to search
 * last.fm for album and track information. Download the library from: 
 *   http://www.curlpp.org/
 * Extract the archive and from a terminal change to the directory created.
 * Execute the following commands looking for errors. Note the following steps
 * require that libcurl first be installed, if its not already present.
 *   cmake ./
 *   make
 *   sudo make install
 *   sudo ldconfig
 * To see the make options available do: make help
 * you may want to note the files that are installed by make install since there
 * does not appear to be any "uninstall" target in the makefile.
 *
 * To use this library, the compile and link must reference the appropriate
 * libraries:
 * g++ -std=c++14 MusicBrowser.cpp -o exampleCurlPP -lstdc++ -ljsoncpp -lcurlpp -lcurl
 * If the includes and libraries are not installed in a location known to g++, then
 * the command will need -I and -L options which point g++ to the include and
 * library files respectively.
 *
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Kelly Ellis klellis4@asu.edu
 *	   Tim Lindquist (Tim.Lindquist@asu.edu) CIDSE - Software Engineering,
 *                       IAFSE, ASU at the Polytechnic campus
 * @file    MusicBrowser.cpp
 * @version February 2020
 * @date Modification: February 6, 2020.
 **/

class MediaClient : public MediaClientGui, musiccollectionstub {

public:

   std::string userId;
   std::string lastfmkey;
   std::string jsonString;
   
   std::thread * playThread;
   MusicLibrary * library;
   Album md;
   musiccollectionstub *stubby;
   
/** ClickedX is one of the callbacks for GUI controls.
    * Callbacks need to be static functions. But, static functions
    * cannot directly access instance data. This program uses "userdata"
    * to get around that by passing the instance to the callback
    * function. The callback then accesses whatever GUI control object
    * that it needs for implementing its functionality.
    */
   static void ClickedX(Fl_Widget * w, void * userdata) {
      std::cout << "You clicked Exit" << std::endl;
      exit(1);
   }

/**
 * Helper method to add an album to the library. Takes in an album object
**/

   void addSearchedAlbum(Album *album) {
	library->addAlbum(*album);
   }


   void addAlbumObject(Json::Value albObj) {
	bool ret = stubby->addAlbum(albObj);
   }


/**
 * Helper method to save the PNG image
**/
  static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
   }

/**
 * Helper method to retrieve the PNG image from the lastfm image url, using curlpp library.
 * This method saves the PNG as 'artwork.png' in the project directory so the image can
 * be displayed on the GUI when an album/track is clicked
**/
   void setImage(char* imgUrl) {
	CURL *image = curl_easy_init();
	CURLcode imageResult;

	if (image) {
	   FILE* file = fopen("artwork.png", "wb"); // File to save image to
	   curl_easy_setopt(image, CURLOPT_URL, imgUrl);
	   curl_easy_setopt(image, CURLOPT_WRITEFUNCTION, write_data);
	   curl_easy_setopt(image, CURLOPT_WRITEDATA, file);
	   curl_easy_setopt(image, CURLOPT_FOLLOWLOCATION, 1);
	   imageResult = curl_easy_perform(image);

	   // Close opened files and objects
	   curl_easy_cleanup(image);
	   fclose(file);
	}
   }

/**
 * Helper method to copy a string to be able to use globally
**/
   void stringCopy(string stringToCopy) {
        jsonString = stringToCopy;
   }

   /**
    * Static search button callback method. 
    */
   static void SearchCallbackS(Fl_Widget*w, void*data) {

      MediaClient *o = (MediaClient*)data;
      cout << "Search Clicked. You asked for a last.fm search of album: " <<
         o->albSrchInput->value() << " by artist: " <<
         o->artSrchInput->value() << endl;
      // Search last.fm and retrieve the json string
      try{
         std::string url = "http://ws.audioscrobbler.com/2.0/?method=album.getinfo&artist=";
	 url = url + o->artSrchInput->value();
	 url = url + "&album=";
	 url = url + o->albSrchInput->value();
	 url = url + "&format=json&api_key=517ecd7c267793bae0ea1f48fb4dd50f";
         //url = url + "517ecd7c267793bae0ea1f48fb4dd50f";
         //url = url + o->lastfmkey;
         cout << "sending request url: " << url << endl;
         std::ostringstream os;
         curlpp::Easy myRequest;
         myRequest.setOpt(new curlpp::options::WriteStream(&os));
         myRequest.setOpt(new curlpp::options::Url(url.c_str()));
         myRequest.perform();
         std::string aString = os.str();
	 o->stringCopy(aString);
         std::cout << aString << std::endl;

	 // Create a new album object out of the search result (json string)
	 Album *album = new Album(aString);

	 // Create a new json value from album
	 Json::Value albObj;
         albObj = album->toJson();
         
	 //o->addAlbum(albObj);

	 o->addAlbumObject(albObj);
	 o->addSearchedAlbum(album);

	 // Update GUI tree
	 o->buildTree();
	 
      }catch ( curlpp::LogicError & e ) {
         std::cout << e.what() << std::endl;
      }
      catch ( curlpp::RuntimeError & e ) {
         std::cout << e.what() << std::endl;
      }

   }

   // Static menu callback method
   static void TreeCallbackS(Fl_Widget*w, void*data) {
      MediaClient *o = (MediaClient*)data;
      o->TreeCallback(); //call the instance callback method
   }

   /**
    * TreeCallback is a callback for tree selections, deselections, expand or
    * collapse.
    */
   void TreeCallback() {
      // Find item that was clicked
      Fl_Tree_Item *item = (Fl_Tree_Item*)tree->item_clicked();
      cout << "Tree callback. Item selected: ";
         	cout << item->label();
		cout << endl;
      		std::string aStr("unknown");
      		std::string aTitle(item->label());
      		switch ( tree->callback_reason() ) {  // reason callback was invoked
      		case       FL_TREE_REASON_NONE: {aStr = "none"; break;}
      		case     FL_TREE_REASON_OPENED: {aStr = "opened";break;}
      		case     FL_TREE_REASON_CLOSED: {aStr = "closed"; break;}

		// Updates the right side of the GUI
      		case   FL_TREE_REASON_SELECTED: {

////////////// If selected item is an album (it has child nodes /////////////////
		if (item->has_children()) {
			aStr = "selected";
         		if(this){
            		   cout << "trying to get: " << item->label() << endl;
			   md = library->media[aTitle];
         		}else{
            		   cout << "library entry not found" << endl;
            		   break;
         		}
	
			// Update album attributes on GUI
			genreChoice->clear(); // Updates each time a new album is clicked
			trackInput->value(""); // Clear track just in case track info is still there
         		albumInput->value(md.title.c_str());
         		authorInput->value(md.author.c_str());
	 		summaryMLI->value(md.summary.c_str());

			// Format the runTime to hh:mm:ss
			char formatted[10];
			int hours = md.runTime/3600;
			int secondsLeft = md.runTime - hours * 3600;
			int minutes = secondsLeft/60;
			int seconds = secondsLeft - minutes * 60;
			sprintf(formatted, "%02d:%02d:%02d", hours, minutes, seconds);
	 	        timeInput->value(formatted);

			// Add image to window - convert imageUrl to char array
			// to use in setImage method (curl method only takes char* as param)
	 		string imageUrl = md.image.c_str();
			int n = imageUrl.length();
	 		char imageurl[n+1];
	 		strcpy(imageurl, imageUrl.c_str());
	 		setImage(imageurl);

			// Update image on GUI and redraw the box so the artwork changes
			box->image(png = new Fl_PNG_Image("artwork.png"));
			box->redraw();

			// Add genres of album to the dropdown box
			map<int, string>::iterator mapIter;
			for (mapIter = md.genreList.begin(); mapIter != md.genreList.end(); mapIter++) {
				genreChoice->add(mapIter->second.c_str());
			}

			// Set genre showing to first genre in list as default
			genreChoice->value(0);

         		break;
		} // End if album

/////////////// If selected item is a track (has no children nodes) ///////////////////////////
		else { 
			   aStr = "selected";

			// get parent of track clicked 
			   Fl_Tree_Item *item2 = (Fl_Tree_Item*)tree->item_clicked()->parent();
         		   Album md2;

			// Search through library to find album that matches parent album
			   Track tr;
         		   if(this){
            		      cout << "trying to get: " << item->label() << endl;
			      md2 = library->media[item2->label()];

			      genreChoice->clear(); // Updates each time a new track/album is clicked

			      // Search trackList using the track name
	    		      map<int, Track>::iterator itr;
	    		      for (itr = md2.trackList.begin(); itr != md2.trackList.end(); itr++) {
				   if (itr->second.trackName == item->label()) {
					   tr = itr->second;
				   }
	    		      }

         		   }else{
            		      cout << "library entry not found" << endl;
            		      break;
         		   }

			   // Update track attributes on GUI
	 		   trackInput->value(tr.trackName.c_str());
         		   albumInput->value(md2.title.c_str());
			   authorInput->value(md2.author.c_str());

			   // Add rank from initialized library (from json file)
			   if (!rankInput->value(tr.rank.c_str())) {
         	              rankInput->value(tr.rank2.c_str());
			   // Add rank from searched album/track
			   } else {
			      rankInput->value(tr.rank.c_str());
			   }

			   // Add genres to drop down box
			   map<int, string>::iterator mapIter;
			   for (mapIter = md2.genreList.begin(); mapIter != md2.genreList.end(); mapIter++) {
				genreChoice->add(mapIter->second.c_str());
			   }

			   // Set default genre showing to first in list
			   genreChoice->value(0);

			   // Format the track duration to mm:ss and update GUI
			   int time = std::stoi(tr.duration.c_str());
			   char formatted[10];
			   int minutes = time/60;
			   int seconds = time%60;
			   sprintf(formatted, "%02d:%02d", minutes, seconds);
	 	           timeInput->value(formatted);

	 		   summaryMLI->value(md2.summary.c_str());

			   // Add image using the same logic as when adding image for album
	 		   string imageUrl = md2.image.c_str();
			   int n = imageUrl.length();
	 		   char imageurl[n+1];
	 		   strcpy(imageurl, imageUrl.c_str());
	 		   setImage(imageurl);
			   box->image(png = new Fl_PNG_Image("artwork.png"));
			   box->redraw();
         		   break;	

		} // end if track
		} // end case
	
      
      case FL_TREE_REASON_DESELECTED: {
		// Reset all GUI boxes when something is deselected so info doesn't stay
		aStr = "deselected"; 
		trackInput->value("");
		albumInput->value("");
		authorInput->value("");
		rankInput->value("");
		timeInput->value("");
		summaryMLI->value("");
		break;
      }
      default: {break;}
      }
      cout << "Callback reason: " << aStr.c_str() << endl;
   }

   // Static menu callback method
   static void Menu_ClickedS(Fl_Widget*w, void*data) {
      MediaClient *o = (MediaClient*)data;
      o->Menu_Clicked(); //call the instance callback method
   }

   // Menu selection instance method that has ccess to instance vars.
   void Menu_Clicked() {
      char picked[80];
      menubar->item_pathname(picked, sizeof(picked)-1);
      string selectPath(picked);
      cout << "Selected Menu Path: " << selectPath << endl;
      int select = genreChoice->value();
      cout << "Selected genre: " << ((select==0)?"rock":"blues") << endl;
      // Handle menu selections
      if(selectPath.compare("File/Save")==0){
         bool ret = stubby->toJsonFile("albums.json", jsonString); // Save to json file
         library->toJsonFile("albums.json", jsonString);
         cout << "Library was saved." << endl;
      }else if(selectPath.compare("File/Restore")==0){
	 // Restore library from json file
	 bool ret = stubby->initLibraryFromJsonFile("albums.json");
         library->initLibraryFromJsonFile("albums.json");
	 // Update tree on GUI
	 buildTree();
      }else if(selectPath.compare("File/Tree Refresh")==0){
         buildTree();
      }else if(selectPath.compare("File/Exit")==0){
         if(playThread && playThread->joinable()){
            playThread->join();
         }
         exit(0);
      } else if (selectPath.compare("Album/Add")==0){
	 cout << "Album added to library." << endl;
      } else if (selectPath.compare("Album/Remove")==0){
	 // Remove album from library and refresh tree on GUI
	 bool ret = stubby->removeAlbum(md.title);
         library->removeAlbum(md.title);
	 //library->removeAlbum(md.title);
	 buildTree();
	 cout << "Album " << md.title << " was removed from library." << endl;
         cout << "Album removed from library." << endl;
      }else if(selectPath.compare("Track/Add")==0){
         cout << "Track added." << endl;
      }else if(selectPath.compare("Track/Remove")==0){
         cout << "Track removed." << endl;
      }else if(selectPath.compare("Track/Play")==0){
         std::string unameres = exec("uname");
         std::string pwdPath = exec("pwd");
         pwdPath = pwdPath.substr(0,pwdPath.length()-1);
         std::cout << "OS type is: " << unameres << " curr.dir is: "
                   << pwdPath << std::endl;
         // This path is only valid on linux so we will have to check ostype
         std::stringstream streamLinux;
         streamLinux << "/usr/bin/vlc "
                     << pwdPath << "/MediaFiles/" << trackInput->value()
                     << ".mp3" ;
         std::string aStr("Linux");
         std::stringstream streamMac;
         streamMac << "/Applications/VLC.app/Contents/MacOS/VLC "
                   << pwdPath << "/MediaFiles/" << trackInput->value()
                   << ".mp3" ;
         cout << "mac command: " << streamMac.str() << endl;
         cout << "linux command: " << streamLinux.str() << endl;
         // start vlc to play the media file
         //limit the comparison to the length of Linux to remove new line char
         if(unameres.compare(0,aStr.length(),aStr)==0){
            string argLinux(streamLinux.str());
            cmd = argLinux;
            playThread = new std::thread(run);
         }else{
            string arg(streamMac.str());
            cmd = arg;
            playThread = new std::thread(run);
         }
      }
   }

   /**
    * a method to execute a command line command and to return
    * the resulting string.
    */
   std::string exec(const char* cmd) {
      FILE* pipe = popen(cmd, "r");
      if (!pipe) return "ERROR";
      char buffer[128];
      std::string result = "";
      while(!feof(pipe)) {
         if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
      }
      pclose(pipe);
      return result;
   }

/**
 * Updates the tree on the left side of the GUI by showing the albums/tracks
 * in the library.
**/
   void buildTree(){
	tree->clear();

	//map<string, Album> albList = library->getAlbumList();
	map<string, Album> albList = library->media;

	// Iterate over the albums in the library and each album's track list
	// and add everything to the tree under Music
	map<string, Album>::iterator itr;
	map<int, Track>::iterator itrTrack;
	for (itr = albList.begin(); itr != albList.end(); itr++) {
		map<int, Track> trackList = itr->second.trackList;
		std::stringstream stream;
		stream << "Music"
			<< "/";
			if (itr->second.album != "") {
				stream << itr->second.album;
				stream << "/" << itr->second.title;
			} else if (itr->second.album == "") {
				stream << itr->second.title;
			}
			stream << "/";
		// Add tracks to same tree node
		for (itrTrack = trackList.begin(); itrTrack != trackList.end(); itrTrack++) {
		std::stringstream stream2;
		stream2 << "Music"
			<< "/"
			<< itr->second.title
			<< "/"
			<< itrTrack->second.trackName;
		tree->add(stream2.str().c_str());
		}
		tree->add(stream.str().c_str());
	}
      cout << endl;
      tree->redraw(); 
   }

   MediaClient(string host, IClientConnector &conn, jsonrpc::clientVersion_t type, const char * name = "Tim", const char * key = "myKey") 
		: musiccollectionstub(conn, type), MediaClientGui(name) {
      
   //MediaClient(HttpClient http, const char * name = "Tim", const char * key = "myKey")
		//: MediaClientGui(name) {

      stubby = new musiccollectionstub(conn);

      //bool ret = stubby->initLibraryFromJsonFile("albums.json");
      
      searchButt->callback(SearchCallbackS, (void*)this);
      menubar->callback(Menu_ClickedS, (void*)this);
      tree->callback(TreeCallbackS, (void*)this);
      callback(ClickedX);
      lastfmkey = key;
      userId = "Kelly.Ellis";

      //bool stuff = this->initLibraryFromJsonFile("albums.json");
	
      library = new MusicLibrary();
      buildTree();
   }
};

int main(int argc, char * argv[]) {
   //std::string developer = (argc>2)?argv[3]:"Tim.Lindquist";
   //std::string lastfmkey = (argc>3)?argv[4]:"lastfmkey";
   //std::string windowTitle = developer + "'s Music Browser";

   string host = "https://127.0.0.1:";
   string developer = "Kelly.Ellis";
   string lastfmkey = "517ecd7c267793bae0ea1f48fb4dd50f";
   string windowTitle = developer + "'s Music Browser";
   string port = "1099";
   string hostPort = "http://127.0.0.1:1099";

/*
   if (argc>1) {
      host = string(argv[1]);
      port = string(argv[2]);
      developer = string(argv[3]);
      lastfmkey = string(argv[4]);
      hostPort = host + port;
   }
*/

   HttpClient httpClient(hostPort);
   
   cout << "Connecting to host: " << host << endl;
   
   //MediaClient cm(httpClient, windowTitle.c_str(), lastfmkey.c_str());
   
   MediaClient cm(hostPort, httpClient, JSONRPC_CLIENT_V2, windowTitle.c_str(),lastfmkey.c_str());
   return (Fl::run());
}

#pragma GCC diagnostic pop
