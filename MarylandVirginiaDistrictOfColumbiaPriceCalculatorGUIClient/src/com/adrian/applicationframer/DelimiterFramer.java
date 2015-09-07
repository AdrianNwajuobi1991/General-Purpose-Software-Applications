/**
 * 
 */
package com.adrian.applicationframer;

import java.io.InputStream;
import java.io.OutputStream;

import com.framercommon.interfaces.Framer;

/**
 * @author Adrian
 *
 */
public class DelimiterFramer implements Framer {

	public DelimiterFramer(InputStream inputStream) {
		// TODO Auto-generated constructor stub
	}

	/* (non-Javadoc)
	 * @see com.framercommon.interfaces.Framer#frameData(byte[], java.io.OutputStream)
	 */
	@Override
	public void frameData(byte[] dataToFrame, OutputStream out) {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see com.framercommon.interfaces.Framer#nextData()
	 */
	@Override
	public byte[] nextData() {
		// TODO Auto-generated method stub
		return null;
	}

}
