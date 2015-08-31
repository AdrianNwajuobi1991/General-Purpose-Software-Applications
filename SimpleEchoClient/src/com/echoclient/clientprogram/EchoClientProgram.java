/**
 * 
 */
package com.echoclient.clientprogram;

/**
 * @author Adrian Nwajuobi
 *
 */

import java.net.*;
import java.io.*;

import com.echoclient.constants.*;
import com.echoclient.interfaces.ServerCommunicatorClientSide;


public class EchoClientProgram implements ServerCommunicatorClientSide {
	/**
	 * socket - The socket for connection
	 */
	private Socket socket;
	/**
	 * echoData - The byte array of the string to be echoed back from the server
	 */
	private byte[]echoData;
	/**
	 * in - The socket input stream
	 */
	private InputStream in;
	/**
	 * out - The socket output stream
	 */
	private OutputStream out;
	/**
	 * EchoClientProgram - The constructor creates a socket that is connected to the server on the specified port
	 * 						and Initializes the Input and Output Streams of the Socket instance.
	 * @throws IOException
	 */
	public EchoClientProgram() throws IOException{
		socket = new Socket(EchoClientConnectionConstants.SERVER_IP_ADDRESS, EchoClientConnectionConstants.SERVER_PORT);
		in = socket.getInputStream();
		out = socket.getOutputStream();
	}	

	@Override
	public void sendData(String dataToSend) throws IOException {
		// TODO Auto-generated method stub
		echoData = dataToSend.getBytes();
		out.write(echoData);
	}
	@Override
	public String receieveData() throws IOException {
		// TODO Auto-generated method stub
		int totalBytesRcvd = 0;
		int bytesRcvd;
		
		while(totalBytesRcvd < echoData.length){
			if((bytesRcvd = in.read(echoData, totalBytesRcvd, echoData.length - totalBytesRcvd)) == -1){
				throw new SocketException("Connection closed prematurely");
			}
			totalBytesRcvd += bytesRcvd;
		}
		return new String(echoData);
	}
	@Override
	public void closeConnection() throws IOException {
		// TODO Auto-generated method stub
		socket.close();
	}
	
}
