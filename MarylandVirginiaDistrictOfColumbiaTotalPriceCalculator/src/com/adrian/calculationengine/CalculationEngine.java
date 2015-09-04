package com.adrian.calculationengine;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
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
	/**
	 * a File instance variable that holds the text of the itemized total of the calculations
	 */
	private File itemizedTotal;
	
	public CalculationEngine(CommodityItem commodityItem){
		this.commodityItem = commodityItem;
		listOfTotalCostCalculators = new ArrayList<TotalCostCalculator>();
		listOfTotalCostCalculators.add(new MarylandTotalCostCalculator());
		listOfTotalCostCalculators.add(new DistrictOfColumbiaTotalCostCalculator());
		listOfTotalCostCalculators.add(new VirginiaTotalCostCalculator());
		itemizedTotal = new File("ItemTotalCosts.txt");
	}
	/**
	 * calculateGrandTotalMD_VA_DC -- the work horse method of this class. It does all the tasks necessary to compute total costs
	 * and create a file in the current directory with results.
	 */
	public void calculateGrandTotalMD_VA_DC(){
		try {
			BufferedWriter bufferedWriter  = new BufferedWriter(new FileWriter(itemizedTotal));
			/*
			 * sequence through the list of total cost calculators:- apply each one to the commodity item, compute the total cost and 
			 * write results to a buffer
			 */
			for(TotalCostCalculator tcc: listOfTotalCostCalculators){
				//apply each one to the commodity item
				commodityItem.setTotalCostCalculator(tcc);
				//compute the total cost
				commodityItem.doTotalPriceCalculation();
				//write results to a buffer
				bufferedWriter.write(commodityItem.getCalculationStatement());
			}
			bufferedWriter.flush();
			bufferedWriter.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
}
