/**
 * 
 */
package com.adrian.totalcostcalculators;

import com.adrian.interfaces.EastCoastTotalCostCalculator;

/**
 * @author Adrian Nwajuobi
 *
 */
public class DistrictOfColumbia implements EastCoastTotalCostCalculator {
	
	private final float DC_SALES_TAX_RATE = 0.0575f;

	/* (non-Javadoc)
	 * @see com.adrian.interfaces.EastCoastTotalCostCalculator#calculatePricePlusTax(float, java.lang.String)
	 */
	@Override
	public String calculatePricePlusTax(float basePrice, String commodityName) {
		// TODO Auto-generated method stub
		float totalCost = basePrice + (DC_SALES_TAX_RATE * basePrice);
		return "The total cost of "+ commodityName +"in Washington, DC is "+totalCost;
	}

}
