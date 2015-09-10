/**
 * 
 */
package com.adrian.server.tcpserver;

import java.io.IOException;
import java.net.Socket;

import com.datapackagecommon.implementations.PriceCalculatorDataPackage;
import com.datapackagecommon.interfaces.DataPackage;
import com.encoderdecodercommon.implementations.TextEncoderDecoder;
import com.encoderdecodercommon.interfaces.EncoderDecoder;
import com.framercommon.implementations.DelimiterFramer;
import com.framercommon.interfaces.Framer;
import com.servercommon.abstractclasses.GenericThreadPoolTCPApplicationServer;

/**
 * @author Adrian
 *
 */
public class PriceCalculatorThreadPoolTCPApplicationServer extends GenericThreadPoolTCPApplicationServer {
	
	private static final float MD_SALES_TAX_RATE = 6e-2f;
	private static final float DC_SALES_TAX_RATE = 5.75e-2f;
	private static final float VA_SALES_TAX_RATE = 4.3e-2f;

	/**
	 * @param serverPort
	 */
	public PriceCalculatorThreadPoolTCPApplicationServer(int serverPort) {
		super(serverPort);
		// TODO Auto-generated constructor stub
	}

	/* (non-Javadoc)
	 * @see com.servercommon.abstractclasses.GenericThreadPoolTCPApplicaitonServer#processClientDataRequest(java.net.Socket)
	 */
	@Override
	public void processClientDataRequest(Socket clientSocket) {
		// TODO Auto-generated method stub
		try{
			clientSocket = serverSocket.accept();
			EncoderDecoder serverEncoderDecoder = new TextEncoderDecoder();
			Framer framer = new DelimiterFramer(clientSocket.getInputStream());
			byte[]request;
			DataPackage dataPackage;
			while((request = framer.nextData())!= null){
				dataPackage = handleRequest(serverEncoderDecoder.decodeFromWire(request));
				framer.frameData(serverEncoderDecoder.encodeToWire(dataPackage), clientSocket.getOutputStream());
			}
			clientSocket.close();

		}catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}finally{
			try {
				clientSocket.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	private DataPackage handleRequest(DataPackage dataPackageOfRequest){
		PriceCalculatorDataPackage priceCalculatorDataPackage = (PriceCalculatorDataPackage)dataPackageOfRequest;
		String basePriceString = priceCalculatorDataPackage.getItemBasePrice();
		float totalPrice;
		if(priceCalculatorDataPackage.getState().equals("MD")){
			totalPrice = Float.parseFloat(basePriceString) + (MD_SALES_TAX_RATE * Float.parseFloat(basePriceString));
		}else if(priceCalculatorDataPackage.getState().equals("DC")){
			totalPrice = Float.parseFloat(basePriceString) + (DC_SALES_TAX_RATE * Float.parseFloat(basePriceString));
		}else{
			totalPrice = Float.parseFloat(basePriceString) + (VA_SALES_TAX_RATE * Float.parseFloat(basePriceString));
		}
		priceCalculatorDataPackage.setItemTotalPrice(new Float(totalPrice).toString());
		return priceCalculatorDataPackage;
	}

}
