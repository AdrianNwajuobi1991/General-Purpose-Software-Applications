/**
 * All server-side communication programs use this interface to communicate with the server.
 */
package com.echoclient.interfaces;

import java.io.IOException;

/**
 * @author Adrian Nwajuobi
 *
 */
public interface ServerCommunicatorClientSide {

	/**
	 * sendData - creates an array of bytes and write the contents to socket output stream
	 * @param dataToSend
	 * @throws IOException
	 */
	public void sendData(String dataToSend) throws IOException;
	/**
	 * receieveData - receives the echoed response from the echo server and returns a string
	 * @return
	 * @throws IOException
	 */
	public String receieveData() throws IOException;
	/**
	 * closeConnection - closes connection to the server.
	 * @throws IOException
	 */
	public void closeConnection() throws IOException;
}
