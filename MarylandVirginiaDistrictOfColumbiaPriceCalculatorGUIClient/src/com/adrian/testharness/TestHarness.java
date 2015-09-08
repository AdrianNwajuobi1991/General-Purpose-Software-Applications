package com.adrian.testharness;

import com.adrian.applicationclient.PriceCalculatorTCPClient;
import com.adrian.applicationdatapackage.PriceCalculatorDataPackage;
import com.encoderdecodercommon.interfaces.DataPackage;
import com.guiclient.common.abstractclasses.GenericTCPApplicationClient;
import com.guiclient.common.beans.ServerDataBean;

public class TestHarness {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		ServerDataBean serverDataBean = new ServerDataBean("127.0.0.1", 5667);
		DataPackage dataPackage = new PriceCalculatorDataPackage("PlayStation 4", "392.99", "MD", "");
		GenericTCPApplicationClient applicationClient = new PriceCalculatorTCPClient(serverDataBean, dataPackage);
		dataPackage = applicationClient.processData();

	}

}
