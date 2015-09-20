/**
 * 
 */
package com.adrian.TestHarness;

import com.adrian.abstractandmodelclasses.CommodityItem;
import com.adrian.calculationengine.CalculationEngine;

/**
 * @author Adrian Nwajuobi
 *
 */
public class PriceCalulatorApplication {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		String commodityItemArgument = args[0];
		Float commodityItemPriceAsFloat = Float.parseFloat(args[1]);
		CommodityItem commodityItem = new CommodityItem(commodityItemArgument, commodityItemPriceAsFloat);
		CalculationEngine calculationEngine = new CalculationEngine(commodityItem);
		calculationEngine.calculateGrandTotalMD_VA_DC();
	}

}
