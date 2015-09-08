/**
 * 
 */
package com.adrian.applicationdatapackage;

import java.util.ArrayList;

import com.encoderdecodercommon.interfaces.DataPackage;

/**
 * @author Adrian Nwajuobi
 *
 */
public class PriceCalculatorDataPackage implements DataPackage{

	private String itemName;
	
	private String itemBasePriceAsString;
	
	private String itemTotalPriceAsString;
	
	private String state;
	
	public PriceCalculatorDataPackage (String itemName, String itemBasePriceAsString, String state, String itemTotalPriceAsString){
		this.itemName = itemName;
		this.itemBasePriceAsString = itemBasePriceAsString;
		this.itemTotalPriceAsString = itemTotalPriceAsString;
		this.state = state;
	}
	
	public void setItemTotalPrice(String itemTotalPrice){
		this.itemTotalPriceAsString = itemTotalPrice;
	}
	
	public String getItemTotalPrice(){
		return itemTotalPriceAsString;
	}
	
	public String getItemName(){
		return itemName;
	}
	
	public String getItemBasePrice(){
		return itemBasePriceAsString;
	}
	
	public String getState(){
		return state;
	}

	@Override
	public ArrayList<String> getDataPackageTokens() {
		// TODO Auto-generated method stub
		ArrayList<String> dataPackageTokens = new ArrayList<String>();
		dataPackageTokens.add(itemName);
		dataPackageTokens.add(itemBasePriceAsString);
		dataPackageTokens.add(itemTotalPriceAsString);
		dataPackageTokens.add(state);
		return dataPackageTokens;
	}
}
