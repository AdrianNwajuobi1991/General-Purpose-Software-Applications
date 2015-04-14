package tcpEchoClient;

import java.net.*;
import java.io.*;

public class TCPEchoClient {

	public static void main(String[] args) throws IOException {
		// TODO Auto-generated method stub
		if((args.length < 2) || (args.length > 3)) {
			throw new IllegalArgumentException();
		}
		
		//Server name or IP address
		String server = args[0];
		byte[] data = args[1].getBytes();
		
		int servPort = (args.length == 3) ? Integer.parseInt(args[2]) : 7;
		
		//Create socket that is connected to server on specified port
		Socket socket =  new Socket(server, servPort);;
		System.out.println("Connected to server.....sending echo string");
		
		InputStream in = socket.getInputStream();
		OutputStream out = socket.getOutputStream();
		
		out.write(data);
		
		int totalBytesRcvd = 0; // total bytes received so far
		int bytesRcvd; //Bytes received in last read
		
		while(totalBytesRcvd < data.length) {
			if((bytesRcvd = in.read(data, totalBytesRcvd, data.length - totalBytesRcvd))
					== -1){
				throw new SocketException();
			}
			totalBytesRcvd += bytesRcvd;
		}// data Array is full
		
		System.out.println("Received: "+new String(data));
		socket.close(); // Close the socket and its streams
	}

}
