package ser321.assign3.klellis4.server;

import java.io.Serializable;
import org.json.JSONObject;
import org.json.JSONString;
import org.json.*;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Enumeration;

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
 * Purpose: Album represents an Album in the MusicLibrary. It contains the information for an Album
 * that was obtained via a json string from a Last.fm search. It uses arrays to hold information such
 * as genres and tracks. Functionality includes getting, adding, and removing tracks from an Album object.
 *
 * @author Kelly Ellis (klellis4@asu.edu),
 *	Tim Lindquist (Tim.Linquist@asu.edu),
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version January 2020
 */

public class Album extends Object implements Serializable {

	public String albumName;
	public String artist;
	public String image; // url to image
	public int runTime; // sum of all tracks' durations
	public ArrayList<String> genres = new ArrayList<String>();
	public ArrayList<Track> trackList = new ArrayList<Track>();
	public String summary;

	public Album(JSONObject object) {
		this.albumName = object.getString("name");

		this.artist = object.getString("artist");

		this.summary = object.getJSONObject("wiki").getString("summary");

		// get image url from jsonarray
		JSONArray array1 = object.getJSONArray("image");
		for (int j = 0; j < array1.length(); j++) {
			JSONObject innerIter = array1.getJSONObject(j);
			if(innerIter.has("size"))
				if(innerIter.getString("size").equals("extralarge"))
					this.image = innerIter.getString("#text");
		}	

		// get arraylist of tracks
		JSONObject trackObject = object.getJSONObject("tracks");
		JSONArray trackArray = trackObject.getJSONArray("track");
		for (int k = 0; k < trackArray.length(); k++) {
			JSONObject track = trackArray.getJSONObject(k);
			Track track1 = new Track(track);
			trackList.add(track1);
		}

		// get arraylist of genres
		JSONObject genreObject = object.getJSONObject("tags");
		JSONArray genreArray = genreObject.getJSONArray("tag");
		for (int b = 0; b < genreArray.length(); b++) {
			JSONObject genre = genreArray.getJSONObject(b);
			genres.add(genre.getString("name"));
		}

		// Get the total runTime of the album
		for (int a = 0; a < trackList.size(); a++) {
			runTime += Integer.parseInt(trackList.get(a).duration);
		}
		
	}

	/** 
	 * Converts the album runtime from an int to a string to be printed to the GUI
	 *
	 * @param runTime the total run time of all of the album's tracks
	 * @return the String result of that run time, formatted for hh:mm:ss
	**/
	public String runTimeFormat(int runTime) {

		int hours = runTime/3600;
		int secondsLeft = runTime - hours * 3600;
		int minutes = secondsLeft / 60;
		int seconds = secondsLeft - minutes * 60;

		return String.format("%02d:%02d:%02d", hours, minutes, seconds);
	}

	public Track getTrack(String trackName) {
		
		for (int i = 0; i < trackList.size(); i++) {
			if (trackList.get(i).trackName.equals(trackName)) {
				return trackList.get(i);			
			}		
		}

		return null;
	}

	public void addTrack(Track track) {
		System.out.println("Adding: " + track.trackName);
		trackList.add(track);
	}

	public void removeTrack(Track track) {
		trackList.remove(track);
	}
}
