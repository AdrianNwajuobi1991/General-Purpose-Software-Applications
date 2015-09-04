/**
 * 
 */
package com.adrian.totalcostcalculators;

import com.adrian.interfaces.TotalCostCalculator;

/**
 * @author Adrian Nwajuobi
 *
 */
public class VirginiaTotalCostCalculator implements TotalCostCalculator {
	
	private final float VA_SALES_TAX_RATE = 0.043f;

	/* (non-Javadoc)
	 * @see com.adrian.interfaces.EastCoastTotalCostCalculator#calculatePricePlusTax(float, java.lang.String)
	 */
	@Override
	public String calculatePricePlusTax(float basePrice, String commodityName) {
		// TODO Auto-generated method stub
		float totalCost = basePrice + (VA_SALES_TAX_RATE * basePrice);
		return "The total cost of "+ commodityName +" in Virginia is "+"$ "+totalCost+"\n";
	}

}
