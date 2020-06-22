

#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <json/json.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <csignal>
#include "MusicLibrary.hpp"
#include "musiclibserverstub.h"
//#include <map>
//#include <string>

//#include "Album.hpp"
//#include "Track.hpp"

using namespace jsonrpc;
using namespace std;

class MusicLibraryServer : public musiclibserverstub {
public:
   MusicLibraryServer(AbstractServerConnector &connector, int port);
   
   virtual string test(const string& param1);

   virtual bool addAlbum(const Json::Value& param1);
   virtual bool removeAlbum(const std::string& param1);
   virtual bool initLibraryFromJsonFile(const string& param1);
   virtual bool toJsonFile(const string& param1, const string& param2);


private:
   MusicLibrary *library;
   int portNum;
};

MusicLibraryServer::MusicLibraryServer(AbstractServerConnector &connector, int port) 
	: musiclibserverstub(connector) {
   library = new MusicLibrary();
   library->initLibraryFromJsonFile("albums.json");
   portNum = port;
}

string MusicLibraryServer::test(const string& param1) {
   string testy;
   testy = param1;
   return testy;
}


bool MusicLibraryServer::addAlbum(const Json::Value& param1) {
   //cout << "Adding " << param1["name"] << " to library." << endl;
   bool ret = true;
   library->addAlbum(param1);
   return ret;
}

bool MusicLibraryServer::removeAlbum(const string& param1) {
   cout << "Removing " << param1 << " from library." << endl;
   bool ret = library->removeAlbum(param1);
   return ret;
}


bool MusicLibraryServer::initLibraryFromJsonFile(const string& param1) {
   cout << "Restoring library from albums.json" << endl;
   bool ret = library->initLibraryFromJsonFile(param1);
   return ret;
}


bool MusicLibraryServer::toJsonFile(const string& param1, const string& param2) {
   cout << "Saving library to albums.json" << endl;
   bool ret = library->toJsonFile(param1, param2);
   return ret;
}


void exiting() {
   cout << "Server has been terminated. Exiting normally" << endl;
}

int main(int argc, char * argv[]) {
   int port = 1099;
   if (argc > 1) {
      port = atoi(argv[1]);
   }
   HttpServer httpserver(port);
   MusicLibraryServer ms(httpserver, port);
   std::atexit(exiting);
   auto ex = [] (int i) {cout << "server terminating with signal " << i << endl;
                         exit(0);
                        };
   std::signal(SIGINT, ex);
   std::signal(SIGABRT, ex);
   std::signal(SIGTERM, ex);
   std::signal(SIGTSTP, ex);
   cout << "Music library server listening on port " << port
        << " use ps to get pid. To quit: kill -9 pid " << endl;
   ms.StartListening();
   while (true) {
   }
   ms.StopListening();
   return 0;
}
