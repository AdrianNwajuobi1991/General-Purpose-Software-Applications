/**
 * 
 */
package com.adrian.server.testharness;

import com.servercommon.abstractclasses.GenericThreadPoolTCPApplicationServer;
import com.adrian.server.tcpserver.*;

/**
 * @author Adrian
 *
 */
public class PriceCalculatorServerHarness {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		int serverPort = 5667;
		
		GenericThreadPoolTCPApplicationServer gtcpas = new PriceCalculatorThreadPoolTCPApplicationServer(serverPort);
		gtcpas.doWorkThreadPoolExecutor();;
	}
	

}
