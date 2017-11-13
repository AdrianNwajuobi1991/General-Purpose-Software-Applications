import static org.junit.Assert.*;

import org.junit.Test;


public class GeoCodeAPIDelegateTestCases {

	@Test
	public void testBadRequest() {
		String result = GeoCodingAPIDelegate.geoCode(true, "AIzaSyAQjiEf39EeoIjC5h2qEQKtpKdOiVOgeV4", "1600+Amphitheatre+Parkway,+Mountain+View,+CA");
		assertFalse(result.equals("blank data"));
	}

}
