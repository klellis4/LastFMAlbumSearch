package ser321.assign3.klellis4.server;

import java.io.Serializable;
import org.json.*;

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
 * Purpose: represents a Track in an Album object. It contains the information for a Track
 * that was obtained via a json string from a Last.fm search. 
 *
 * @author Kelly Ellis (klellis4@asu.edu),
 *	Tim Lindquist (Tim.Linquist@asu.edu),
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version January 2020
 */

public class Track extends Object implements Serializable {

	public String trackName;
	public String artist;
	public String rank;
	public String duration;

	public Track(JSONObject object) {
		this.trackName = object.getString("name");
		this.artist = object.getJSONObject("artist").getString("name");
		this.rank = object.getJSONObject("@attr").getString("rank");
		this.duration = object.getString("duration");
	}

	/**
	 * Helper function to convert the duration of the track into string format to be printed to the GUI.
	 *
	 * @param String duration The duration of the track that was received from the json file
	 * @return String a formatted string in hh:mm format
	*/
	public String durationFormat(String duration) {

		int time = Integer.parseInt(duration);
		return String.format("%02d:%02d", time / 60, time % 60);
	}

	public String getTrackName() {
		
		return trackName;

	}
}
