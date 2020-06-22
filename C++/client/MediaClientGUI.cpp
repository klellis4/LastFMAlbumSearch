#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Tree.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Tree_Item.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Image.H>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

/**
 * Copyright (c) 2020 Tim Lindquist/Kelly Ellis,
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
 * Purpose: Sample C++ FLTK view class. MediaClientGui constructs the view
 * for media app. This class is extended by the client controller which is
 * the MediaClient class. MediaClient defines the call-backs for UI controls.
 * It contains sample control functions that respond to button clicks and tree
 * selects.
 * This software is meant to run on Debian Wheezy Linux
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist (Tim.Lindquist@asu.edu) CIDSE - Software Engineering,
 *                       IAFSE, ASU at the Polytechnic campus
 *	   Kelly Ellis klellis4@asu.edu
 * @file    MediaClientGui.cpp
 * @version    February, 2020
 * @date Modification: February 6, 2020.
 **/

class MediaClientGui : public Fl_Window {
protected:

   Fl_Box * box;
   Fl_PNG_Image * png;

   /**
    * tree is the Fl_Tree object that occupies the left side of the window.
    * this tree control provides the ability to add and remove items and to
    * manipulate and query the tree when an exception occurs.
    */
   Fl_Tree * tree;

   /**
    * titleInput is the Fl_Input object labelled Title
    * Its to display, or for the user to enter
    * the album's track tile.
    */
   Fl_Input * trackInput;

   /**
    * albumInput is the Fl_Input object labelled Album
    * Its for the user to enter the media album. For videos, genre is
    * used to organize video, and album is ignored.
    */
   Fl_Input * albumInput;

   /**
    * authorInput is the Fl_Input object labelled Artist
    * Its for the user to enter the artist of the track/album
    */
   Fl_Input * authorInput;

   /**
    * rankInput is the Fl_Input object labelled Rank
    * Its for the display and entry of a track's rank within the album.
    */
   Fl_Input * rankInput;

   /**
    * timeInput is the Fl_Input object labelled Time.
    * It provides the display of album or track play time in form: hh:mm:ss
    */
   Fl_Input * timeInput;

   /**
    * genreChoice is the Fl_Choice object labelled Genre.
    * It provides for the display of the album genre.
    */
   Fl_Choice * genreChoice;

   /**
    * summaryMLI is the Fl_Multiline_Input object in the lower right panel.
    * It provides for the display and changing an album's summary.
    */
   Fl_Multiline_Input * summaryMLI;

   /**
    * fileNameInput is the Fl_Input object labelled File Name.
    * It provides the display or entry of mp3 file associated with a track.
    */
   Fl_Input * fileNameInput;

   /**
    * albSrchInput is the Fl_Input object labelled Album in the left panel.
    * It provides entry for album last.fm search.
    */
   Fl_Input * albSrchInput;

   /**
    * artSrchInput is the Fl_Input object labelled Artist.
    * It provides the display or entry of artist for last.fm search
    */
   Fl_Input * artSrchInput;

   /**
    * searchButt is the Fl_Button object labelled Search.
    * After the user enters album (albSrchInput) and artist (artSrchInput)
    * information, the searchButt is clicked to initiate a last.fm album
    * search. The results of the search are parsed and displayed in the tree.
    */
   Fl_Button * searchButt;

   /**
    * menubar is the Fl_Menu_bar object with menus: File,Album,Track
    */
   Fl_Menu_Bar *menubar;

public:
   //MediaClientGui(const char * name = "Ser321") : Fl_Window(635,350,name) {
   MediaClientGui(const char * name = "Ser321") : Fl_Window(980,500,name) {
      begin();

      menubar = new Fl_Menu_Bar(0, 0, this->w(), 25);
      menubar->add("File/Save");
      menubar->add("File/Restore");
      menubar->add("File/Tree Refresh");
      menubar->add("File/Exit");
      menubar->add("Album/Add");
      menubar->add("Album/Remove");
      menubar->add("Album/Play");
      menubar->add("Track/Add");
      menubar->add("Track/Remove");
      menubar->add("Track/Play");

      albSrchInput = new Fl_Input(55, 35, 220, 25);
      albSrchInput->label("Album");
      albSrchInput->value("");

      artSrchInput = new Fl_Input(50, 80, 150, 25);
      artSrchInput->label("Artist");
      artSrchInput->value("");

      searchButt = new Fl_Button(220, 80, 90, 25,"Search");

      // create a tree control at position x=10, y=10. Its 150 pixels wide
      // and window height less 20 pixels high. Add some sample tree nodes.
      tree = new Fl_Tree(10, 120, 325, this->h()-135);
      tree->add("Flintstones/Fred");
      tree->add("Flintstones/Wilma");
      tree->close("/Flintstones");

      /*
       * add a text input control at x=250, y=35 of width 200 pixels and
       * height of 25 pixels. Initialize it contents to media title.
       */
      /*
       * add a text input control at x=250, y=35 of width 200 pixels and
       * height of 25 pixels. Initialize it contents to media title.
       */
      albumInput = new Fl_Input(385, 35, 265, 25);
      albumInput->label("Album");
      albumInput->value("");

      trackInput = new Fl_Input(700, 35, 265, 25);
      trackInput->label("Track");
      trackInput->value("");

      authorInput = new Fl_Input(385, 80, 220, 25);
      authorInput->label("Artist");
      authorInput->value("");

      rankInput = new Fl_Input(680, 80, 60, 25);
      rankInput->label("Rank");
      rankInput->value("");

      timeInput = new Fl_Input(815, 80, 100, 25);
      timeInput->label("Time");
      timeInput->value("");
      
      fileNameInput = new Fl_Input(420, 130, 310, 25);
      fileNameInput->label("File Name");
      fileNameInput->value("");
      
      // create the media genre drop-down (input_choice)
      genreChoice = new Fl_Choice(815, 130, 125, 25, "Genre");
      genreChoice->add("");
      genreChoice->value(0); // set the control initially to rock

      box = new Fl_Box(350,180,320-20,320-20);     
      png = new Fl_PNG_Image("");      
      box->image(png);
      

      summaryMLI = new Fl_Multiline_Input(665,180,300,300,0);
      summaryMLI->wrap(1);

      end();
      show();
   }
};
