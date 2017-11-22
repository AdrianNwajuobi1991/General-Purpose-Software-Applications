package gov.bep.client;


import static org.junit.Assert.*;

import org.junit.Test;

public class StandAloneClientTest {

	@Test
	public void testWebserviceCall() {
		String result = StandAloneClient.callGeoCodeAPI("xml", "AIzaSyAQjiEf39EeoIjC5h2qEQKtpKdOiVOgeV4", "9334+Madison+Avenue,+Laurel,+MD,+20723");
		assertFalse(result.equals("blank data"));
		assertFalse(result == null);
		assertTrue(result.length() > 0);
	}

}
