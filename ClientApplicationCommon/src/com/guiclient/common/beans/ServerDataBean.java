/**
 * This is a Java Bean class representing the data about the server a client needs to connect to.
 */
package com.guiclient.common.beans;

/**
 * @author Adrian Nwajuobi
 *
 */
public class ServerDataBean {

	/**
	 * a field representing the server's IP address in dotted-quad notation.
	 */
	private String serverIPAddress;
	/**
	 * a field representing the port the server is listening on
	 */
	private int serverPort;
	
	public ServerDataBean(String serverIPAddress, int serverPort){
		this.serverIPAddress = serverIPAddress;
		this.serverPort = serverPort;
	}
	/**
	 * this method can be used to change to a new server's IP address at runtime
	 * @param serverIPAddress
	 */
	public void setServerIPAddress(String serverIPAddress){
		this.serverIPAddress = serverIPAddress;
	}
	/**
	 * this method can be used to change to a new server's port number at runtime
	 * @param serverPort
	 */
	public void setServerPort(int serverPort){
		this.serverPort = serverPort;
	}
	/**
	 * this method returns the server's IP Address
	 * @return
	 */
	public String getServerIPAddress(){
		return serverIPAddress;
	}
	/**
	 * the method returns the server's Port
	 * @return
	 */
	public int getServerPort(){
		return serverPort;
	}
}
