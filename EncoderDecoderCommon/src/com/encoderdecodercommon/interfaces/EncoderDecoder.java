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
	/**
	 * encodes a DataPackage to a specific wire frame format for transmission
	 * @param dataPackage
	 * @return
	 */
	public byte[] encodeToWire(DataPackage dataPackage);
	/**
	 * decodes a byte stream as an array from the network to construct a DataPackage
	 * @param inputFromWire
	 * @return
	 */
	DataPackage decodeFromWire(byte[] inputFromWire);
	
}
