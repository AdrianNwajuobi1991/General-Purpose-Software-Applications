package gov.bep.reversegeocode.main;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.net.URL;
import java.nio.charset.Charset;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.cert.CertificateException;
import java.util.Properties;
import org.json.*;

import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSocketFactory;
import javax.net.ssl.TrustManagerFactory;

public class ReverseGeocodeMain {

	private static String endpoint="https://maps.googleapis.com/maps/api/geocode/";
	private TLSConnectionSetup tlsSetup = new TLSConnectionSetup("C:\\PROGRA~1\\Java\\jdk1.8.0_144\\jre\\lib\\security\\cacerts");;

	public static void main(String[] args) {
		ReverseGeocodeMain revGeocode = new ReverseGeocodeMain();
		Properties prop = new Properties();
		InputStream input = null;
		try {
			input = new FileInputStream("geocode_api_call.properties");
			prop.load(input);
		} catch (IOException ex) {
			ex.printStackTrace();
		}
		JSONObject jsonObject = revGeocode.doReverseGeocode(prop.getProperty("json_or_xml"), prop.getProperty("api_key"), prop.getProperty("address"));

		if(prop.getProperty("json_or_xml").equals("json")){
			System.out.println(jsonObject.toString(2));
		}else{
			System.out.println(XML.toString(jsonObject));
		}
	}

	public JSONObject doReverseGeocode(String json_or_xml, String api_key, String address) {
		String response = "";
		String requestEndpoint=endpoint;
		requestEndpoint += "json"+"?"+"latlng="+address+"&key="+api_key;
		try {
			URL url = new URL(requestEndpoint);
			HttpsURLConnection conn = (HttpsURLConnection)url.openConnection();
			try {
				tlsSetup.doTLSSetup(conn);
			} catch (KeyManagementException | KeyStoreException | NoSuchAlgorithmException | CertificateException e) {
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
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return new JSONObject(response);
	}

	private class TLSConnectionSetup{

		private String trustStorePath;

		public TLSConnectionSetup(String trustStorePath){
			this.trustStorePath = trustStorePath;
		}

		public void doTLSSetup(HttpsURLConnection conn) throws KeyStoreException, 
		NoSuchAlgorithmException, CertificateException, IOException, KeyManagementException{
			TrustManagerFactory tmf = TrustManagerFactory.getInstance("SunX509");
			KeyStore trustStore = KeyStore.getInstance("JKS");
			char[] trustStorePassword = "changeit".toCharArray();            
			trustStore.load(new FileInputStream(trustStorePath), trustStorePassword);
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

}
