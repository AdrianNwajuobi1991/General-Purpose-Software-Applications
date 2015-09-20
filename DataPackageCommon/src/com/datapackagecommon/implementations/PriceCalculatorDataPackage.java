/**
 * This class represents a concrete implementation of the DataPackage interface
 */
package com.datapackagecommon.implementations;

import com.datapackagecommon.interfaces.DataPackage;

/**
 * @author Adrian Nwajuobi
 *
 */
public class PriceCalculatorDataPackage implements DataPackage {
	/**
	 * this field represents an item's name
	 */
	private String itemName;
	/**
	 * this field represents an item's MSRP or base price as a string
	 */
	private String itemBasePriceAsString;
	/**
	 * this field represents an item's total price as a string
	 */
	private String itemTotalPriceAsString;
	/**
	 * this item represent the U.S state (MD, DC or VA) that the price of the item will be computed for
	 */
	private String state;
	
	public PriceCalculatorDataPackage (String itemName, String itemBasePriceAsString, String state, String itemTotalPriceAsString){
		this.itemName = itemName;
		this.itemBasePriceAsString = itemBasePriceAsString;
		this.state = state;
		this.itemTotalPriceAsString = itemTotalPriceAsString;
	}
	/**
	 * this method sets the items total price as a string
	 * @param itemTotalPrice
	 */
	public void setItemTotalPrice(String itemTotalPrice){
		this.itemTotalPriceAsString = itemTotalPrice;
	}
	/**
	 * returns the item's total price as a string
	 * @return
	 */
	public String getItemTotalPrice(){
		return itemTotalPriceAsString;
	}
	/**
	 * returns the item's name
	 * @return
	 */
	public String getItemName(){
		return itemName;
	}
	/**
	 * returns the item's base price as a string
	 * @return
	 */
	public String getItemBasePrice(){
		return itemBasePriceAsString;
	}
	/**
	 * returns the state whose total price the item has
	 * @return
	 */
	public String getState(){
		return state;
	}
	
}
