package gov.bep.unittests;

import static org.junit.Assert.*;

import org.json.JSONObject;
import org.junit.Test;

import gov.bep.reversegeocode.main.ReverseGeocodeMain;

public class ReverseGeocodeTest {

	@Test
	public void testJSONPayload() {
		ReverseGeocodeMain revGeocode = new ReverseGeocodeMain();
		JSONObject jsonObject = revGeocode.doReverseGeocode("json", 
				"AIzaSyAQjiEf39EeoIjC5h2qEQKtpKdOiVOgeV4", "40.714224,-73.961452");
		assertTrue(jsonObject != null);
		assertTrue(jsonObject.length() > 0);
	}

}
