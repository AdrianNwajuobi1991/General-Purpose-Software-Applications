/**
 * A general model class representing A general consumer item for purchase
 */
package com.adrian.abstractclasses;

import com.adrian.interfaces.EastCoastTotalCostCalculator;

/**
 * @author Adrian Nwajuobi
 *
 */
public class CommodityItem {

	/**
	 * eastCoastTaxCalculatorHolds -- is a field that references an instance of
	 */
	protected EastCoastTotalCostCalculator eastCoastTotalCostCalculator;
	/**
	 * basePrice -- is a field to denote the MSRP of the product
	 */
	protected float basePrice;
	/**
	 * name -- is a field that represents the name of the item
	 */
	protected String name;
	
	/**
	 * doTotalPriceCalculation -- delegates the calculation of the Total Price of an item to the 
	 * eastCoastTaxCalculator
	 */
	public void doTotalPriceCalculation(){
		eastCoastTotalCostCalculator.calculatePricePlusTax(basePrice);
	}
	/**
	 * 
	 * @param eastCoastTotalCostCalculator - the reference to the desired total cost calculator to use
	 * setTotalCostCalculator -- a setter method(mutator) that dynamically sets the total calculator at runtime
	 */
	public void setTotalCostCalculator(EastCoastTotalCostCalculator eastCoastTotalCostCalculator){
		this.eastCoastTotalCostCalculator = eastCoastTotalCostCalculator;
	}
	
	public String getName(){
		return name;
	}
}
