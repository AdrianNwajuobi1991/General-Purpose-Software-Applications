/**
 * 
 */
package com.adrian.totalcostcalculators;

import java.math.BigDecimal;

import com.adrian.interfaces.TotalCostCalculator;

/**
 * @author Adrian Nwajuobi
 *
 */
public class MarylandTotalCostCalculator implements TotalCostCalculator {
	
	private final float MD_SALES_TAX_RATE = 6e-2f;

	/* (non-Javadoc)
	 * @see com.adrian.interfaces.EastCoastTotalCostCalculator#calculatePricePlusTax(float)
	 */
	@Override
	public String calculatePricePlusTax(float basePrice, String commodityName) {
		// TODO Auto-generated method stub
		float totalCost = basePrice + (MD_SALES_TAX_RATE * basePrice);
		BigDecimal bigDecimal = new BigDecimal(totalCost);
		bigDecimal = bigDecimal.setScale(2, BigDecimal.ROUND_HALF_UP);
		return "The total cost of "+ commodityName +" in Maryland is "+"$ "+bigDecimal.toString()+"\n";
	}

}
