/**
 * 
 */
package com.servercommon.abstractclasses;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;


import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

import com.serverprotocolcommon.interfaces.ServerProtocol;

/**
 * @author Adrian
 *
 */
public abstract class GenericThreadPoolTCPApplicationServer {

	protected ServerSocket serverSocket;
	
	public GenericThreadPoolTCPApplicationServer(int serverPort){
		try {
			serverSocket = new ServerSocket(serverPort);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public final void doWorkThreadPoolExecutor(){
		Executor serverExcecutor = Executors.newCachedThreadPool();
		Socket clientSocket = null;
		while(true){
			try {
				clientSocket = serverSocket.accept();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			serverExcecutor.execute(new TemplateServerProtocol(clientSocket));
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
