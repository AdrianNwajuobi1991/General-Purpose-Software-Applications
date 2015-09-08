/**
 * 
 */
package com.adrian.applicationgui;

import javax.swing.JButton;
import javax.swing.JFrame;

import com.adrian.applicationclient.PriceCalculatorTCPClient;
import com.guiclient.common.beans.ServerDataBean;

/**
 * @author Adrian Nwajuobi
 *
 */
public class PriceCalculatorClientGUI {
	/**
	 * the underlying TCP client for this application
	 */
	private PriceCalculatorTCPClient priceCalculatorTCPClient;
	/**
	 * the server data to be populated from the GUI text box. It will also hold the server's response for diplay
	 * to the user
	 */
	private ServerDataBean serverDataBean;
	/**
	 * The data package to be populated from the GUI text box
	 */
	private com.datapackagecommon.interfaces.DataPackage serverDataPackage;
	/**
	 * The main JFrame of the GUI
	 */
	private JFrame applicationFrame;
	/**
	 * the button that connects to the server and sends data to a Price Calculation Server for evaluation
	 */
	private JButton sendForPriceCalculation;
	/**
	 * closes the connection to the server
	 */
	private JButton closeConnectionToServer;
	
}
