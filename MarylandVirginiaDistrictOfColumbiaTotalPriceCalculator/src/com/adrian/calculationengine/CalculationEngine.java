package com.adrian.calculationengine;

import java.util.ArrayList;

import com.adrian.abstractandmodelclasses.CommodityItem;
import com.adrian.interfaces.TotalCostCalculator;
import com.adrian.totalcostcalculators.DistrictOfColumbiaTotalCostCalculator;
import com.adrian.totalcostcalculators.MarylandTotalCostCalculator;
import com.adrian.totalcostcalculators.VirginiaTotalCostCalculator;

public class CalculationEngine {

	/**
	 * a reference to a commodity item for the calculation engine to work on
	 */
	private CommodityItem commodityItem;
	/**
	 * an Array List instance variable that holds references to the three kinds of total cost calculators
	 * for Maryland, DC, and Virginia
	 */
	private ArrayList<TotalCostCalculator>listOfTotalCostCalculators;
	
	public CalculationEngine(CommodityItem commodityItem){
		this.commodityItem = commodityItem;
		listOfTotalCostCalculators = new ArrayList<TotalCostCalculator>();
		listOfTotalCostCalculators.add(new MarylandTotalCostCalculator());
		listOfTotalCostCalculators.add(new DistrictOfColumbiaTotalCostCalculator());
		listOfTotalCostCalculators.add(new VirginiaTotalCostCalculator());
	}
	
}
