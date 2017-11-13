
import java.util.logging.Level;
import java.util.logging.Logger;
import java.io.IOException;
import java.io.OutputStream;

import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Servlet implementation class GeoCodeServletTest
 */
public class GeoCodeServletTest extends HttpServlet {
	private static final long serialVersionUID = 1L;
	private final static Logger LOGGER = Logger.getLogger(GeoCodeServletTest.class.getName());

    /**
     * Default constructor. 
     */
    public GeoCodeServletTest() {
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see Servlet#init(ServletConfig)
	 */
	public void init(ServletConfig config) throws ServletException {
		// TODO Auto-generated method stub
	}

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String apiKey  = request.getParameter("apikey");
		LOGGER.finer("apiKey is: "+apiKey);
		String address = request.getParameter("address");
		LOGGER.finer("address is: "+address);
		String json_or_xml = request.getParameter("jsonORxml");
		LOGGER.finer("json_or_xml is: "+json_or_xml);
		boolean jsonFlag = false;
		if(json_or_xml != null && json_or_xml.contains("json")){
			jsonFlag = true;
		}
		LOGGER.setLevel(Level.FINER);
		String result = GeoCodingAPIDelegate.geoCode(jsonFlag, apiKey, address.replaceAll(" ", "+"));
		if(result == null){
			LOGGER.finer("result is null");
		}else{
			LOGGER.finer(result);
		}
		OutputStream out = response.getOutputStream();
		if(out == null){
			LOGGER.finer("out is null");
		}else{
			out.write(result.getBytes());
			out.flush();
		}
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
	}

	/**
	 * @see HttpServlet#doPut(HttpServletRequest, HttpServletResponse)
	 */
	protected void doPut(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
	}

	/**
	 * @see HttpServlet#doDelete(HttpServletRequest, HttpServletResponse)
	 */
	protected void doDelete(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
	}

}
