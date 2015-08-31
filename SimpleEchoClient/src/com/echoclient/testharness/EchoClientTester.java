package com.echoclient.testharness;

import java.io.IOException;

import com.echoclient.clientprogram.EchoClientProgram;
import com.echoclient.interfaces.ServerCommunicatorClientSide;

public class EchoClientTester {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		try {
			ServerCommunicatorClientSide scc = new EchoClientProgram();
			scc.sendData("This is a Test of echo client/server application test number three four five six final echo server test");
			String recvd_data = scc.receieveData();
			System.out.println(recvd_data);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

}
