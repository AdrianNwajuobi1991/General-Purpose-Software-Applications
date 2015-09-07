package com.guiclient.common.abstractclasses;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

import com.encoderdecodercomon.interfaces.DataPackage;
import com.encoderdecodercomon.interfaces.EncoderDecoder;
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
	protected Framer applictionFramer;
	/**
	 * Encodes messages according to the application protocol
	 */
	protected EncoderDecoder applicationEncoderDecoder;
	
	protected DataPackage dataPackage;

	public GenericTCPApplicationClient(ServerDataBean serverDataBean, DataPackage dataPackage){
		this.serverDataBean = serverDataBean;
		this.dataPackage = dataPackage;
	}
	
	public void setEncoder(EncoderDecoder encoderDecoder){
		this.applicationEncoderDecoder = encoderDecoder;
	}
	
	public void setFramer(Framer framer){
		this.applictionFramer = framer;
	}
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

	public abstract void processData();

	public void closeConnection() {
		// TODO Auto-generated method stub
		try {
			socket.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
