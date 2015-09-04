/**
 * The Interface that all concrete tax calculator classes must implement for the U.S East Coast
 */
package com.adrian.interfaces;

/**
 * @author Adrian Nwajuobi
 *
 */
public interface TotalCostCalculator {
	
	public String calculatePricePlusTax(float basePrice, String commodityName);
	
}
