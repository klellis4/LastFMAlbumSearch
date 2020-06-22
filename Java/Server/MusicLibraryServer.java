package ser321.assign3.klellis4.server;

import java.rmi.*;
import java.util.*;

public interface MusicLibraryServer extends Remote {
	public Album getAlbum(String albumName) throws RemoteException;
	public ArrayList<Album> getAlbumList() throws RemoteException;
	public void addAlbum(Album album) throws RemoteException;
	public void removeAlbum(String album) throws RemoteException;
	public void saveLibraryToFile(String fileName, String jsonURL) throws RemoteException;
	public void restoreLibraryFromFile(String fileName) throws RemoteException;
}
