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
		CommodityItem commodityItem = new CommodityItem("PlayStation 4", (float)392.99);
		CalculationEngine calculationEngine = new CalculationEngine(commodityItem);
		calculationEngine.calculateGrandTotalMD_VA_DC();
	}

}
