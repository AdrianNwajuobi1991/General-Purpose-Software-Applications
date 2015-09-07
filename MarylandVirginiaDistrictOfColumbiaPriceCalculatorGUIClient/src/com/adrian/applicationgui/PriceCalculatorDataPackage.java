/**
 * 
 */
package com.adrian.applicationgui;

import com.encoderdecodercommon.interfaces.DataPackage;

/**
 * @author Adrian Nwajuobi
 *
 */
public class PriceCalculatorDataPackage implements DataPackage {

	private String itemName;
	
	private float itemBasePrice;
	
	private float itemTotalPrice;
	
	public PriceCalculatorDataPackage (String itemName, float itemBasePrice){
		this.itemName = itemName;
		this.itemBasePrice = itemBasePrice;
		this.itemTotalPrice = 0.0f;
	}
	
	public void setItemTotalPrice(float itemTotalPrice){
		this.itemTotalPrice = itemTotalPrice;
	}
	
	public float getItemTotalPrice(){
		return itemTotalPrice;
	}
	
	public String getItemName(){
		return itemName;
	}
	
	public float getItemBasePrice(){
		return itemBasePrice;
	}
	
}
