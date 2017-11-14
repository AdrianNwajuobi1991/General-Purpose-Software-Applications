import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.nio.charset.Charset;
import java.util.logging.Level;
import java.util.logging.Logger;


public class GeoCodingAPIDelegate {
	private final static Logger LOGGER = Logger.getLogger(GeoCodeServletTest.class.getName());

	private static String endpoint="https://maps.googleapis.com/maps/api/geocode/";

	
	public static String geoCode(boolean jsonFlag, String apiKey, String address) {
		LOGGER.setLevel(Level.FINER);
		String json_or_xml ="json";
		String response = "";
		if(jsonFlag == false){
			json_or_xml = "xml";
		}
		String requestEndpoint=endpoint;
		requestEndpoint += json_or_xml+"?"+"address="+address+"&key="+apiKey;
		try {
			URL url = new URL(requestEndpoint);
			URLConnection conn = url.openConnection();
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
		LOGGER.finer(response);
		return response;
	}

}
