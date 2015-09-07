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
	
	public void setServerIPAddress(String serverIPAddress){
		this.serverIPAddress = serverIPAddress;
	}
	
	public void setServerPort(int serverPort){
		this.serverPort = serverPort;
	}
	
	public String getServerIPAddress(){
		return serverIPAddress;
	}
	
	public int getServerPort(){
		return serverPort;
	}
}
