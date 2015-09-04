/**
 * A general model class representing a general consumer item for purchase. This class can be extended using inheritance and reused in other
 * applications involving consumer items
 */
package com.adrian.abstractandmodelclasses;

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
	
	protected String calculationStatement;
	/**
	 * name -- is a field that represents the name of the item
	 */
	protected String name;
	
	/**
	 * doTotalPriceCalculation -- delegates the calculation of the Total Price of an item to the 
	 * eastCoastTaxCalculator
	 */
	public void doTotalPriceCalculation(){
		calculationStatement =  eastCoastTotalCostCalculator.calculatePricePlusTax(basePrice, name);
	}
	/**
	 * 
	 * @param eastCoastTotalCostCalculator - the reference to the desired total cost calculator to use
	 * setTotalCostCalculator -- a setter method(mutator) that dynamically sets the total calculator at runtime
	 */
	public void setTotalCostCalculator(EastCoastTotalCostCalculator eastCoastTotalCostCalculator){
		this.eastCoastTotalCostCalculator = eastCoastTotalCostCalculator;
	}
	/**
	 *  getName -- returns the name of this item.
	 */
	public String getName(){
		return name;
	}
	/**
	 * getCalculationStatement -- returns the statement result of the computation of the total price.
	 */
	public String getCalculationStatement(){
		return calculationStatement;
	}
	
}
