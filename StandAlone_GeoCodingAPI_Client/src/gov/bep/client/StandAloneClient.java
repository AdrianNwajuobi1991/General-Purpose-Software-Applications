package gov.bep.client;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.nio.charset.Charset;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.cert.CertificateException;
import java.util.Properties;

import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSocketFactory;
import javax.net.ssl.TrustManagerFactory;

public class StandAloneClient {
	
	private static String endpoint="https://maps.googleapis.com/maps/api/geocode/";

	public static void main(String[] args) {
		Properties prop = new Properties();
		InputStream input = null;
		try {
			input = new FileInputStream("geocode_api_call.properties");
			prop.load(input);
		} catch (IOException ex) {
			ex.printStackTrace();
		}
		String result = callGeoCodeAPI(prop.getProperty("json_or_xml"), prop.getProperty("api_key"), prop.getProperty("address"));
		System.out.println(result);
		
	}
	
	public static String callGeoCodeAPI(String json_or_xml, String apiKey, String address){
		String response = "";
		String requestEndpoint=endpoint;
		requestEndpoint += json_or_xml+"?"+"address="+address+"&key="+apiKey;
		try {
			URL url = new URL(requestEndpoint);
			HttpsURLConnection conn = (HttpsURLConnection)url.openConnection();
			try {
				doTLS(conn);
			} catch (KeyManagementException | KeyStoreException | NoSuchAlgorithmException | CertificateException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			conn.setDoInput(true);
			BufferedReader in = new BufferedReader(new InputStreamReader(conn.getInputStream(), Charset.forName("UTF-8")));
			String chunk = "";
			while((chunk = in.readLine()) != null){
				response += chunk;
			}
			in.close();
		} catch (MalformedURLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return response;
	}
	
	private static void doTLS(HttpsURLConnection conn) throws KeyStoreException, NoSuchAlgorithmException, CertificateException, IOException, KeyManagementException{
		
		TrustManagerFactory tmf = TrustManagerFactory.getInstance("SunX509");
		KeyStore trustStore = KeyStore.getInstance("JKS");
        char[] trustStorePassword = "changeit".toCharArray();            
        trustStore.load(new FileInputStream("C:\\PROGRA~1\\Java\\jdk1.8.0_144\\jre\\lib\\security\\cacerts"), trustStorePassword);
        tmf.init(trustStore);
		/*
		 * Creates a socket factory for HttpsURLConnection using JKS
		 * contents
		 */
		SSLContext sc = SSLContext.getInstance("TLS");
		sc.init(null, tmf.getTrustManagers(), new java.security.SecureRandom());
		SSLSocketFactory socketFactory = sc.getSocketFactory();
		conn.setSSLSocketFactory(socketFactory);
	}

}
