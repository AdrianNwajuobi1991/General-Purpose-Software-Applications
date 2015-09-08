/**
 * 
 */
package com.adrian.applicationencoderdecoder;

import java.io.ByteArrayInputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.util.Scanner;

import com.adrian.applicationdatapackage.PriceCalculatorDataPackage;
import com.encoderdecodercommon.interfaces.DataPackage;
import com.encoderdecodercommon.interfaces.EncoderDecoder;

/**
 * @author Adrian
 *
 */
public class TextEncoderDecoder implements EncoderDecoder {

	/**
	 * magic string to distinguish the data from random garbage on the network
	 */
	private final String MAGIC = "Pricing";
	/**
	 * The delimiter String
	 */
	private final String DELIMSTR = " ";
	/**
	 * The character set of the byte array for text based encoding
	 */
	private final String CHARSETNAME = "US-ASCII";
	/* (non-Javadoc)
	 * @see com.encoderdecodercommon.interfaces.EncoderDecoder#encodeToWire(com.encoderdecodercommon.interfaces.DataPackage)
	 */
	@Override
	public byte[] encodeToWire(DataPackage dataPackage) {
		// TODO Auto-generated method stub
		String dataString = MAGIC + DELIMSTR;
		for(String str: dataPackage.getDataPackageTokens()){
			dataString += str + DELIMSTR;
		}
		byte encodedData[] = null;
		try {
			encodedData = dataString.getBytes(CHARSETNAME);
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return encodedData ;
	}

	/* (non-Javadoc)
	 * @see com.encoderdecodercommon.interfaces.EncoderDecoder#decodeFromWire(byte[])
	 */
	@Override
	public DataPackage decodeFromWire(byte[] inputFromWire){
		// TODO Auto-generated method stub
		ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(inputFromWire);
		Scanner scanner = null;
		try {
			scanner = new Scanner(new InputStreamReader(byteArrayInputStream, CHARSETNAME));
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		String token;
		
		token = scanner.next();
		if(!token.equals(MAGIC)){
			scanner.close();
			return null;
		}
		String itemName = scanner.next();
		String itemBasePriceAsString = scanner.next();
		String itemTotalPriceAsString = scanner.next();
		String state = scanner.next();
		
		DataPackage dataPackage = new PriceCalculatorDataPackage(itemName, itemBasePriceAsString, state, itemTotalPriceAsString);
		scanner.close();
		return dataPackage;
	}

}
