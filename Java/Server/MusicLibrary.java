package ser321.assign3.klellis4.server;

import java.io.*;
import java.util.*;
import java.net.URL;
import org.json.JSONObject;
import org.json.JSONArray;
import org.json.JSONTokener;
import java.io.FileInputStream;
import java.rmi.server.*;
import java.rmi.*;
import java.util.ArrayList;
import ser321.assign3.klellis4.server.Album;
import ser321.assign3.klellis4.server.Track;
//import ser321.assign3.klellis4.client.MediaLibraryApp;

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
 * Purpose: is the implementing class for the music library. Includes functionality for holding a list of 
 *   albums that are stored in the library, as well as getting, adding, and removing albums from the library.
 *
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 *	   Kelly Ellis (klellis4@asu.edu)
 * @version January 2020
 */


public class MusicLibrary extends java.rmi.server.UnicastRemoteObject implements MusicLibraryServer { 
   
   public ArrayList<Album> albumList;

   private static final String fileName="music.json";

   private static JSONObject albumDetails;
   private static JSONObject trackDetails;
   private static JSONObject album;
   
   public MusicLibrary () throws RemoteException {

      this.albumList = new ArrayList<Album>();

     try{
         InputStream is = this.getClass().getClassLoader().getResourceAsStream(this.fileName);
         if(is==null){
            is = new FileInputStream(new File(this.fileName));
         }
         JSONObject media = new JSONObject(new JSONTokener(is));
         Iterator<String> it = media.keys();
         while (it.hasNext()){
            String mediaTitle = it.next();
            JSONObject aMed = media.optJSONObject(mediaTitle);
            if (aMed != null){
            }
         }
      }catch (Exception ex){
         System.out.println("Exception reading "+fileName+": "+ex.getMessage());
      }
   }

   public ArrayList<Album> getAlbumList() throws RemoteException {

	return albumList;

   }


   public Album getAlbum(String albumName) throws RemoteException {

	for (int i = 0; i < albumList.size(); i++) {
		if (albumList.get(i).albumName.equals(albumName)) {
			System.out.println("Album get successful!: " + albumList.get(i).albumName);
			return albumList.get(i);
		}
	}

	return null;
   }

   public void addAlbum(Album album) throws RemoteException {
      
      System.out.println("Adding: "+album.albumName);
      albumList.add(album);
      
   }

   public void removeAlbum(String album) throws RemoteException {

	try {
		for (int i = 0; i < albumList.size(); i++) {
			if (albumList.get(i).albumName.equals(album)) {
				albumList.remove(albumList.get(i));
			}
		}
   	} catch (Exception exc) {
		exc.printStackTrace();
	}
   }

   public static void main(String args[]) throws RemoteException {
	
	try {
		String hostId="localhost";
		String regPort="1099";
		if (args.length >= 2) {
			hostId = args[0];
			regPort = args[1];
		}
		MusicLibraryServer obj = new MusicLibrary();
		Naming.rebind("rmi://" + hostId + ":" + regPort + "/MusicLibraryServer", obj);
		System.out.println("Server bound in registry as: " +
			"rmi://" + hostId + ":" + regPort + "/MusicLibraryServer");
	} catch (Exception e) {
		e.printStackTrace();
	}
   }

   public void saveLibraryToFile(String fileName, String jsonURL) throws RemoteException {

	try {

		PrintWriter out = new PrintWriter(fileName);

		out.println(jsonURL);

		out.close();
	} catch (Exception aa) {
		aa.printStackTrace();
	}
   }

   public void restoreLibraryFromFile(String fileName) throws RemoteException {
	
	try {

	    FileInputStream in = new FileInputStream(fileName);

	    BufferedReader br = new BufferedReader(new InputStreamReader(in));

	    StringBuilder sb = new StringBuilder();
	    String line;
	    if (br != null) {
		int ch;
		while ((ch = br.read()) != -1) {
			sb.append((char)ch);
		}
		br.close();
	    }
	    in.close();

	    String result = sb.toString();

	    JSONObject resultObject = new JSONObject(result);
	    Album album = new Album(resultObject.getJSONObject("album"));
	    albumList.add(album);	    

	} catch (Exception abc) {
		abc.printStackTrace();
	}

   }

}
