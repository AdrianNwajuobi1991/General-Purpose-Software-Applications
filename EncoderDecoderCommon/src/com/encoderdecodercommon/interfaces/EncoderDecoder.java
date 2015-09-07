/**
 * 
 */
package com.encoderdecodercommon.interfaces;

/**
 * @author Adrian
 *
 */
public interface EncoderDecoder {

	public byte[] encodeToWire(DataPackage dataPackage);
	DataPackage decodeFromWire(byte[] inputFromWire);
	
}
