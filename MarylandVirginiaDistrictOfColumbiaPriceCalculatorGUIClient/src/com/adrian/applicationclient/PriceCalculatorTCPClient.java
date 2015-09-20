/**
 * 
 */
package com.adrian.applicationclient;

import com.encoderdecodercommon.implementations.*;
import com.framercommon.implementations.*;
import com.datapackagecommon.interfaces.DataPackage;
import com.guiclient.common.abstractclasses.GenericTCPApplicationClient;
import com.guiclient.common.beans.ServerDataBean;

/**
 * @author Adrian
 *
 */
public class PriceCalculatorTCPClient extends GenericTCPApplicationClient {

	public PriceCalculatorTCPClient(ServerDataBean serverDataBean, DataPackage dataPackage) {
		super(serverDataBean, dataPackage);
		// TODO Auto-generated constructor stub
	}

	/* (non-Javadoc)
	 * @see com.guiclient.common.abstractclasses.GenericTCPApplicationClient#processData()
	 */
	@Override
	public DataPackage processData() {
		// TODO Auto-generated method stub
		/*****************************Sending data below*****************************/
		establishConnection();
		applicationEncoderDecoder = new TextEncoderDecoder();
		applicationFramer = new DelimiterFramer(inputStream);
		byte[]encodedData = applicationEncoderDecoder.encodeToWire(dataPackage);
		applicationFramer.frameData(encodedData, outputStream);
		/*****************************Receiving data below************************/
		encodedData = applicationFramer.nextData();
		dataPackage = applicationEncoderDecoder.decodeFromWire(encodedData);
		return dataPackage;
	}
	

}
