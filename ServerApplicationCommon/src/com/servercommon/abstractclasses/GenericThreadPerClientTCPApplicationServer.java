/**
 * 
 */
package com.servercommon.abstractclasses;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import com.serverprotocolcommon.interfaces.ServerProtocol;

/**
 * @author Adrian
 *
 */
public abstract class GenericThreadPerClientTCPApplicationServer {

	protected ServerSocket serverSocket;


	public GenericThreadPerClientTCPApplicationServer(int serverPort){
		try {
			serverSocket = new ServerSocket(serverPort);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public final void doWorkThreadPerClient(){
		while(true){
			Socket clientSocket = null;
			try {
				clientSocket = serverSocket.accept();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			Thread clientThread = new Thread(new TemplateServerProtocol(clientSocket));
			clientThread.start();
		}	
	}
	
	public abstract void processClientDataRequest(Socket clientSocket);
	
	private class TemplateServerProtocol implements ServerProtocol{
		
		private Socket clientSocket;
		
		private TemplateServerProtocol(Socket clientSocket){
			this.clientSocket = clientSocket;
		}

		@Override
		public void run() {
			// TODO Auto-generated method stub
			processClientDataRequest(clientSocket);
		}
		
	}

}
