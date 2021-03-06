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
public class VirginiaTotalCostCalculator implements TotalCostCalculator {
	
	private final float VA_SALES_TAX_RATE = 4.3e-2f;

	/* (non-Javadoc)
	 * @see com.adrian.interfaces.EastCoastTotalCostCalculator#calculatePricePlusTax(float, java.lang.String)
	 */
	@Override
	public String calculatePricePlusTax(float basePrice, String commodityName) {
		// TODO Auto-generated method stub
		float totalCost = basePrice + (VA_SALES_TAX_RATE * basePrice);
		BigDecimal bigDecimal = new BigDecimal(totalCost);
		bigDecimal = bigDecimal.setScale(2, BigDecimal.ROUND_HALF_UP);
		return "The total cost of "+ commodityName +" in Virginia is "+"$ "+bigDecimal.toString()+"\n";
	}

}
