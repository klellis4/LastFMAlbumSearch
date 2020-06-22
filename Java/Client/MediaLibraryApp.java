package ser321.assign3.klellis4.client;

import javax.swing.*;
import java.io.*;
import java.nio.file.Paths;
import java.nio.charset.Charset;
import javax.sound.sampled.*;
import java.beans.*;
import java.net.*;
import javax.swing.tree.*;
import javax.swing.event.*;
import javax.swing.text.html.*;
import javax.swing.filechooser.*;
import java.awt.event.*;
import java.awt.*;
import java.util.*;
import java.lang.Runtime;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpRequest.BodyPublishers;
import java.net.http.HttpResponse;
import java.net.http.HttpResponse.BodyHandlers;
import java.net.URLConnection;
import java.time.Duration;
import org.json.JSONObject;
import ser321.assign2.lindquis.MediaLibraryGui;
import java.rmi.*;
import ser321.assign3.klellis4.server.MusicLibraryServer;
import ser321.assign3.klellis4.server.MusicLibrary;
import ser321.assign3.klellis4.server.Album;
import ser321.assign3.klellis4.server.Track;

/**
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
 * Purpose: MediaLibraryApp instructor sample for solving Spring 2020 ser321 assignments.
 * This problem provides for browsing and managing information about
 * music albums. It uses a Swing JTree, and JTextField controls to 
 * realize a GUI with a split pane. The left pane contains an expandable
 * JTree of the media library.
 * This program is a sample controller for the non-distributed version of
 * the system.
 * The right pane contains components that allow viewing, modifying and adding
 * albums, tracks, and corresponding files.
 *
 * @author Tim Lindquist (Tim.Linquist@asu.edu),
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 *	   Kelly Ellis (klellis4@asu.edu)
 * @version January 2020
 */
public class MediaLibraryApp extends MediaLibraryGui implements
                                                       TreeWillExpandListener,
     					               ActionListener,
					               TreeSelectionListener {

   private static final boolean debugOn = true;
   private static final String pre = "http://ws.audioscrobbler.com/2.0/?method=album.getinfo&artist=";
   private String url;
   private boolean stopPlaying;         //shared, but not synchronized with playing thread.
   private MusicLibraryServer lib;
   private String lastFMKey;
   private static Remote remoteService;
   private static String hostId;
   private static String regPort;
   private static Album album;
   private static String result;

   public MediaLibraryApp(String author, String authorKey) {// String hostId, String regPort) {
          super(author);
          this.lastFMKey = authorKey;

	try {
		lib = (MusicLibraryServer)Naming.lookup("rmi://" + hostId + ":" + regPort +
			"/MusicLibraryServer");
		System.out.println("Client obtained remote object reference to the MusicLibraryServer");

	} catch (Exception bb) {
		bb.printStackTrace();
	}
	
          stopPlaying = false;

          // register this object as an action listener for menu item clicks. This will cause
          // my actionPerformed method to be called every time the user selects a menuitem.
          for(int i=0; i<userMenuItems.length; i++){
             for(int j=0; j<userMenuItems[i].length; j++){
                userMenuItems[i][j].addActionListener(this);
             }
          }
          // register this object as an action listener for the Search button. This will cause
          // my actionPerformed method to be called every time the user clicks the Search button
          searchJButt.addActionListener(this);
          try{
             tree.addTreeSelectionListener(this);
             rebuildTree();
          }catch (Exception ex){
             JOptionPane.showMessageDialog(this,"Handling "+
                                       " constructor exception: " + ex.getMessage());
          }
          setVisible(true);
   }

   /**
    * A method to facilite printing debugging messages during development, but which can be
    * turned off as desired.
    *
    **/

   private void debug(String message) {
      if (debugOn)
         System.out.println("debug: "+message);
   }

   /**
    * Create and initialize nodes in the JTree of the left pane.
    * buildInitialTree is called by MediaLibraryGui to initialize the JTree.
    * Classes that extend MediaLibraryGui should override this method to 
    * perform initialization actions specific to the extended class.
    * The default functionality is to set base as the label of root.
    * In your solution, you will probably want to initialize by deserializing
    * your library and displaying the categories and subcategories in the
    * tree.
    * @param root Is the root node of the tree to be initialized.
    * @param base Is the string that is the root node of the tree.
    */
   public void buildInitialTree(DefaultMutableTreeNode root, String base){
      //set up the context and base name
      try{
         root.setUserObject(base);
      }catch (Exception ex){
         JOptionPane.showMessageDialog(this,"exception initial tree:"+ex);
         ex.printStackTrace();
      }
   }

   /**
    *
    * method to build the JTree of music shown in the left panel of the UI. The
    * field tree is a JTree as defined and initialized by MediaLibraryGui class.
    * it is defined to be protected so it can be accessed by extending classes.
    * This version of the method uses the music library to get the names of
    * tracks. Your solutions will need to replace this structure with one that
    * keeps information particular to both Album and Track (two classes Album.java,
    * and Track.java). Your music library will store and provide access to Album
    * and Track objects.
    * This method is provided to demonstrate one way to add nodes to a JTree based
    * on an underlying storage structure.
    * See also the methods clearTree, valueChanged, and getSubLabelled defined in this class.
    **/
   public void rebuildTree(){

      tree.removeTreeSelectionListener(this);
      DefaultTreeModel model = (DefaultTreeModel)tree.getModel();
      DefaultMutableTreeNode root = (DefaultMutableTreeNode)model.getRoot();
      clearTree(root, model);
      DefaultMutableTreeNode musicNode = new DefaultMutableTreeNode("Music");
      model.insertNodeInto(musicNode, root, model.getChildCount(root));

      // put nodes in the tree and updates right side of window when clicked
      try {
	    ArrayList<Album> albumList = lib.getAlbumList();
	    int aSize = albumList.size();

	    for (int i = 0; i < aSize; i++) {
		    // insert albums into tree
		    DefaultMutableTreeNode aNode = new DefaultMutableTreeNode(albumList.get(i).albumName);
		    model.insertNodeInto(aNode, musicNode, model.getChildCount(musicNode));
		
		    // add tracks
		    int tSize = albumList.get(i).trackList.size();
		    for(int j = 0; j < tSize; j++) {
			    DefaultMutableTreeNode tNode = new DefaultMutableTreeNode(albumList.get(i).trackList.get(j).trackName);
			    model.insertNodeInto(tNode, aNode, model.getChildCount(aNode));
		    }
	    }

        // expand all the nodes in the JTree
        for(int r =0; r < tree.getRowCount(); r++){
          tree.expandRow(r);
         }

        tree.addTreeSelectionListener(this);
      } catch (Exception aaa) {
		aaa.printStackTrace();
      }
   }

   private void clearTree(DefaultMutableTreeNode root, DefaultTreeModel model){
      try{
         DefaultMutableTreeNode next = null;
         int subs = model.getChildCount(root);
         for(int k=subs-1; k>=0; k--){
            next = (DefaultMutableTreeNode)model.getChild(root,k);
            debug("removing node labelled:"+(String)next.getUserObject());
            model.removeNodeFromParent(next);
         }
      }catch (Exception ex) {
         System.out.println("Exception while trying to clear tree:");
         ex.printStackTrace();
      }
   }

   private DefaultMutableTreeNode getSubLabelled(DefaultMutableTreeNode root,
                                                 String label){
      DefaultMutableTreeNode ret = null;
      DefaultMutableTreeNode next = null;
      boolean found = false;
      for(Enumeration<TreeNode> e = root.children();
          e.hasMoreElements();){
         next = (DefaultMutableTreeNode)e.nextElement();
         debug("sub with label: "+(String)next.getUserObject());
         if (((String)next.getUserObject()).equals(label)){
            debug("found sub with label: "+label);
            found = true;
            break;
         }
      }
      if(found)
         ret = next;
      else
         ret = null;
      return (DefaultMutableTreeNode)ret;
   }

   public void treeWillCollapse(TreeExpansionEvent tee) {
      debug("In treeWillCollapse with path: "+tee.getPath());
      tree.setSelectionPath(tee.getPath());
   }

   public void treeWillExpand(TreeExpansionEvent tee) {
      debug("In treeWillExpand with path: "+tee.getPath());
   }

////////////////// updates right side of window with song info ////////////////////
   public void valueChanged(TreeSelectionEvent e) {
      try{
         tree.removeTreeSelectionListener(this);
         DefaultMutableTreeNode node = (DefaultMutableTreeNode)
            tree.getLastSelectedPathComponent();

	if(node != null && !((String)node.getUserObject()).equals("Music") && node.getParent() != null) {
		// Current selected node is an album
		if (!node.isLeaf()) {
			Album selectedAlbum = lib.getAlbum((String)node.getUserObject());
			albumJTF.setText(selectedAlbum.albumName);
			trackJTF.setText(" ");
			rankJTF.setText(" ");
			timeJTF.setText(selectedAlbum.runTimeFormat(selectedAlbum.runTime));
			setAlbumImage(selectedAlbum.image);
			authorJTF.setText(selectedAlbum.artist);
			summaryJTA.setText(selectedAlbum.summary);
			
			// add genres to drop-down box
			while (genreJCB.getItemCount() != 0) {
				genreJCB.removeItemAt(0);
			}
			for (int i = 0; i < selectedAlbum.genres.size(); i++) {
				genreJCB.addItem(selectedAlbum.genres.get(i));
			}


		}

		// Current selected node is a track
		else if (node.isLeaf()) {
			DefaultMutableTreeNode parentAlbum = (DefaultMutableTreeNode)node.getParent();
			Album selectedAlb = lib.getAlbum((String)parentAlbum.getUserObject());
			Track selectedTrack = selectedAlb.getTrack((String)node.getUserObject());
			setAlbumImage(selectedAlb.image);
			authorJTF.setText(selectedTrack.artist);
			albumJTF.setText(selectedAlb.albumName);
			summaryJTA.setText(selectedAlb.summary);
			trackJTF.setText(selectedTrack.trackName);
			rankJTF.setText(selectedTrack.rank);
			timeJTF.setText(selectedTrack.durationFormat(selectedTrack.duration));

			// add genres to drop-down box
			while (genreJCB.getItemCount() != 0) {
				genreJCB.removeItemAt(0);
			}
			for (int i = 0; i < selectedAlb.genres.size(); i++) {
				genreJCB.addItem(selectedAlb.genres.get(i));
			}
		}
	}

      }catch (Exception ex){
         ex.printStackTrace();
      }
      tree.addTreeSelectionListener(this);
   }

   public void actionPerformed(ActionEvent e) {
      tree.removeTreeSelectionListener(this);
	
          if(e.getActionCommand().equals("Exit")) {
             System.exit(0);
          }else if(e.getActionCommand().equals("Save")) {
		try {
			lib.saveLibraryToFile("music.json", result);
			System.out.println("Library saved.");
		} catch (Exception ll) {
			ll.printStackTrace();
		}
          }else if(e.getActionCommand().equals("Restore")) {
             boolean resRes = false;
	     try {
	     	lib.restoreLibraryFromFile("music.json");
             	rebuildTree();
		System.out.println("Library was restored.");
	     } catch (Exception abcd) {
		abcd.printStackTrace();
	     }
          }else if(e.getActionCommand().equals("AlbumAdd")) {
             System.out.println("Album was added.");
          }else if(e.getActionCommand().equals("TrackAdd")) {
             int typeInd = genreJCB.getSelectedIndex();
             rebuildTree();

///////////////////////////////// Search Last.fm Here ///////////////////////////////////////////////
          }else if(e.getActionCommand().equals("Search")) {
	
             String searchReqURL = pre+artistSearchJTF.getText().trim()+"&album="+albumSearchJTF.getText().trim()+
                               "&api_key="+lastFMKey+"&format=json";

             System.out.println("calling fetchAsyncURL with url: "+searchReqURL);

             fetchAsyncURL(searchReqURL);

	    // Create JSONObject from search result, then create album objects from that
	    result = fetchURL(searchReqURL);
	    JSONObject resultObject = new JSONObject(result);
	    album = new Album(resultObject.getJSONObject("album"));

	    try {
	         lib.addAlbum(album);
	    } catch (Exception excep) {
		excep.printStackTrace();
	    }

	    rebuildTree();

           }else if(e.getActionCommand().equals("Tree Refresh")) {
              	 rebuildTree();
           }else if(e.getActionCommand().equals("TrackRemove")) {
              	System.out.println("TrackRemove not implemented");
           }else if(e.getActionCommand().equals("AlbumRemove")) {
		try {
			lib.removeAlbum(album.albumName);
			rebuildTree();
			
		} catch (Exception mm) {
			mm.printStackTrace();
		}
		System.out.println("Album was removed.");
          }else if(e.getActionCommand().equals("AlbumPlay") || e.getActionCommand().equals("TrackPlay")){
             	 try{
                 	DefaultMutableTreeNode node = (DefaultMutableTreeNode)
                                    tree.getLastSelectedPathComponent();
                	if(node!=null){
                   		String nodeLabel = (String)node.getUserObject();
                	}
             	}catch(Exception ex){
                 	System.out.println("Execption trying to play media:");
                 	ex.printStackTrace();
             	}
          }
          tree.addTreeSelectionListener(this);
   }

   /**
    *
    * A method to do asynchronous url request printing the result to System.out
    * @param aUrl the String indicating the query url for the lastFM api search
    *
    **/
   public void fetchAsyncURL(String aUrl){
      try{
         HttpClient client = HttpClient.newHttpClient();
         HttpRequest request = HttpRequest.newBuilder()
            .uri(URI.create(aUrl))
            .timeout(Duration.ofMinutes(1))
            .build();
         client.sendAsync(request, BodyHandlers.ofString())
            .thenApply(HttpResponse::body)
            .thenAccept(System.out::println)
            .join();
      }catch(Exception ex){
         System.out.println("Exception in fetchAsyncUrl request: "+ex.getMessage());
      }
   }

   /**
    *
    * a method to make a web request. Note that this method will block execution
    * for up to 20 seconds while the request is being satisfied. Better to use a
    * non-blocking request.
    * @param aUrl the String indicating the query url for the lastFM api search
    * @return the String result of the http request.
    *
    **/
   public String fetchURL(String aUrl) {
      StringBuilder sb = new StringBuilder();
      URLConnection conn = null;
      InputStreamReader in = null;
      try {
         URL url = new URL(aUrl);
         conn = url.openConnection();
         if (conn != null)
            conn.setReadTimeout(20 * 1000); // timeout in 20 seconds
         if (conn != null && conn.getInputStream() != null) {
            in = new InputStreamReader(conn.getInputStream(),
                                       Charset.defaultCharset());
            BufferedReader br = new BufferedReader(in);
            if (br != null) {
               int ch;
               // read the next character until end of reader
               while ((ch = br.read()) != -1) {
                  sb.append((char)ch);
               }
               br.close();
            }
         }
         in.close();
      } catch (Exception ex) {
         System.out.println("Exception in url request:"+ ex.getMessage());
      } 
      return sb.toString();
   }

   public boolean sezToStop(){
      return stopPlaying;
   }

   public static void main(String args[]) {
      String name = "first.last";
      try {
      	String key = "517ecd7c267793bae0ea1f48fb4dd50f";
      	hostId = "localhost";
     	regPort = "2222";
      	if (args.length >= 2){
            hostId = args[0];
            regPort = args[1];
            name = args[2];
      	}
         MediaLibraryApp mla = new MediaLibraryApp(name,key);
      }catch (Exception ex){
         ex.printStackTrace();
      }
   }
}
