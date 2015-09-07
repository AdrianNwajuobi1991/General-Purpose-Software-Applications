/**
 * 
 */
package com.framercommon.interfaces;

import java.io.OutputStream;

/**
 * @author Adrian
 *
 */
public interface Framer {

	public void frameData(byte[] dataToFrame, OutputStream out);
	
	public byte[] nextData();
	
}
