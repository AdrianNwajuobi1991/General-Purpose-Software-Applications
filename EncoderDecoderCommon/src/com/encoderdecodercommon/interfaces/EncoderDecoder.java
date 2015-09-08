/**
 * 
 */
package com.encoderdecodercommon.interfaces;

import com.datapackagecommon.interfaces.DataPackage;

/**
 * @author Adrian
 *
 */
public interface EncoderDecoder {

	public byte[] encodeToWire(DataPackage dataPackage);
	
	DataPackage decodeFromWire(byte[] inputFromWire);
	
}
