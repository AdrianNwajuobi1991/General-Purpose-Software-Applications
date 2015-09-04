/**
 * 
 */
package com.adrian.totalcostcalculators;

import com.adrian.interfaces.EastCoastTotalCostCalculator;

/**
 * @author Adrian Nwajuobi
 *
 */
public class MarylandTotalCostCalculator implements EastCoastTotalCostCalculator {
	
	private final float MD_SALES_TAX_RATE = 0.06f;

	/* (non-Javadoc)
	 * @see com.adrian.interfaces.EastCoastTotalCostCalculator#calculatePricePlusTax(float)
	 */
	@Override
	public String calculatePricePlusTax(float basePrice, String commodityName) {
		// TODO Auto-generated method stub
		float totalCost = basePrice + (MD_SALES_TAX_RATE * basePrice);
		return "The total cost of "+ commodityName +"in Maryland is "+totalCost;
	}

}
