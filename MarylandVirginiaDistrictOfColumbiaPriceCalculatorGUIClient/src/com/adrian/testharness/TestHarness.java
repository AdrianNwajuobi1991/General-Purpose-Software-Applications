package com.adrian.testharness;

import com.adrian.applicationclient.PriceCalculatorTCPClient;
import com.guiclient.common.abstractclasses.GenericTCPApplicationClient;
import com.guiclient.common.beans.ServerDataBean;
import com.datapackagecommon.implementations.PriceCalculatorDataPackage;
import com.datapackagecommon.interfaces.*;

public class TestHarness {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		ServerDataBean serverDataBean = new ServerDataBean("127.0.0.1", 5667);
		DataPackage dataPackage = new PriceCalculatorDataPackage("PlayStation 4", "392.99", "MD", "");
		GenericTCPApplicationClient applicationClient = new PriceCalculatorTCPClient(serverDataBean, dataPackage);
		dataPackage = applicationClient.processData();
		com.datapackagecommon.implementations.PriceCalculatorDataPackage priceCalculatorDataPackage = (com.datapackagecommon.implementations.PriceCalculatorDataPackage)dataPackage;
		System.out.println(priceCalculatorDataPackage.getItemTotalPrice());
	}

}
