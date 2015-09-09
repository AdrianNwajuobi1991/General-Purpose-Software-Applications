package com.guiclient.common.abstractclasses;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

import com.datapackagecommon.interfaces.DataPackage;
import com.encoderdecodercommon.interfaces.EncoderDecoder;
import com.framercommon.interfaces.Framer;
import com.guiclient.common.beans.ServerDataBean;


public abstract class GenericTCPApplicationClient {
	/**
	 * the socket for the client to establish a connection to the Server
	 */
	protected Socket socket;
	/**
	 * the Socket's input stream
	 */
	protected InputStream inputStream;
	/**
	 * the Socket's output stream
	 */
	protected OutputStream outputStream;
	/**
	 * information about the server to connect to
	 */
	protected ServerDataBean serverDataBean;
	/**
	 * frames messages for transmission
	 */
	protected Framer applicationFramer;
	/**
	 * Encodes messages according to the application protocol
	 */
	protected EncoderDecoder applicationEncoderDecoder;
	/**
	 * the data package of items to be sent to the server for processing
	 */
	protected com.datapackagecommon.interfaces.DataPackage dataPackage;

	public GenericTCPApplicationClient(ServerDataBean serverDataBean, com.datapackagecommon.interfaces.DataPackage dataPackage){
		this.serverDataBean = serverDataBean;
		this.dataPackage = dataPackage;
	}
	/**
	 * sets the framer at runtime
	 * @param framer
	 */
	public void setFramer(Framer framer){
		this.applicationFramer = framer;
	}
	/**
	 * sets the encoder/decoder at runtime
	 * @param encoderDecoder
	 */
	public void setEncoder(EncoderDecoder encoderDecoder){
		this.applicationEncoderDecoder = encoderDecoder;
	}
	/**
	 * delegates encoding to the application encoder to perform encoding
	 * @return
	 */
	public byte[] encodeData(){
		return applicationEncoderDecoder.encodeToWire(dataPackage);
	}
	/**
	 * delegates framing to the application framer to perform framing
	 * @param encodedData
	 */
	public void frameData(byte[] encodedData){
		this.applicationFramer.frameData(encodedData, outputStream);
	}
	/**
	 * establishes a connection with the server
	 */
	public void establishConnection() {
		// TODO Auto-generated method stub
		try {
			socket = new Socket(serverDataBean.getServerIPAddress(), serverDataBean.getServerPort());
			inputStream = socket.getInputStream();
			outputStream = socket.getOutputStream();
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	/**
	 * abstract method for subclasses to implement a way of processing data and interacting with the server
	 * @return
	 */
	public abstract DataPackage processData();

	/**
	 * closes the connection to the server
	 */
	public void closeConnection() {
		// TODO Auto-generated method stub
		try {
			socket.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	/**
	 * Template method for this abstract class. It defines the main procedure for the flow of client operations for the 
	 * subclasses of this class.
	 * @return
	 */
	public final DataPackage doWorkTCPClient(){
		DataPackage dataPackage;
		establishConnection();
		dataPackage = processData();
		closeConnection();
		return dataPackage;
	}

}
