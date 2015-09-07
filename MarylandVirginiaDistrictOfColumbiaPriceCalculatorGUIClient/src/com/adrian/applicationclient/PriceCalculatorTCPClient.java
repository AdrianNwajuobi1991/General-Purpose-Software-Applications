/**
 * 
 */
package com.adrian.applicationclient;

import com.adrian.applicationencoderdecoder.BinaryEncoderDecoder;
import com.adrian.applicationframer.LengthFramer;
import com.encoderdecodercommon.interfaces.DataPackage;
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
		establishConnection();
		applicationEncoderDecoder = new BinaryEncoderDecoder();
		applicationFramer = new LengthFramer(inputStream);
		byte[]encodedData = applicationEncoderDecoder.encodeToWire(dataPackage);
		applicationFramer.frameData(encodedData, outputStream);
		/*****************************/
		encodedData = applicationFramer.nextData();
		dataPackage = applicationEncoderDecoder.decodeFromWire(encodedData);
		return dataPackage;
	}
	

}
