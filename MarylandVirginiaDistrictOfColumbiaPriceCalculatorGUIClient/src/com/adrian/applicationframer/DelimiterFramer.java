/**
 * 
 */
package com.adrian.applicationframer;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import com.framercommon.interfaces.Framer;

/**
 * @author Adrian
 *
 */
public class DelimiterFramer implements Framer {

	private InputStream inputStream;
	
	private final byte DELIMITER = '\n';
	
	public DelimiterFramer(InputStream inputStream) {
		// TODO Auto-generated constructor stub
		this.inputStream = inputStream;
	}

	/* (non-Javadoc)
	 * @see com.framercommon.interfaces.Framer#frameData(byte[], java.io.OutputStream)
	 */
	@Override
	public void frameData(byte[] dataToFrame, OutputStream out) {
		// TODO Auto-generated method stub
		try {
			out.write(dataToFrame);
			out.write(DELIMITER);
			out.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	/* (non-Javadoc)
	 * @see com.framercommon.interfaces.Framer#nextData()
	 */
	@Override
	public byte[] nextData() {
		// TODO Auto-generated method stub
		ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
		int nextByte;
		try {
			while((nextByte = inputStream.read()) != DELIMITER){
				if(nextByte == -1){
					if(byteArrayOutputStream.size() == 0){
						return null;
					}
				}
				byteArrayOutputStream.write(nextByte);
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return byteArrayOutputStream.toByteArray();
	}

}
