/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package TranslationZipFileGeneratorUtils;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author CSV6NHM
 */
public class TranslationZipFileGeneratorUtil {

    /**
     * This method will create the Directory out of a String parameter.
     * @param tzfgsh is the object that holds the state of the Translation Zip
     * File Generator.
     * @param pathString is the String to create the directory from.
     */
    public static void createFileFromPath
        (TranslationZipFileGeneratorStateHolder tzfgsh, String pathString) {
        File file = new File(pathString);       
        tzfgsh.setFolderPathFileRef(file);
         
    }

    public static void calculateTranslatableWords
        (TranslationZipFileGeneratorStateHolder tzfgsh) {
        File file = tzfgsh.getExportedKeysFileRef();
        int totalWordCount = countWords(file);
        int linesTimesTwo = doubleCountLines(file);
        int numUISErrorsLines = uisErrorsLinesCount(file);
    }

    private static int countWords(File file) {
       String line;
       int runningTotal = 0;
       try {
            BufferedReader  br = new BufferedReader(new FileReader(file));
            try {
		while ((line = br.readLine()) != null) {
                    String []parts = line.split(" ");
                    for( String part : parts){
                        runningTotal += 1;        
                    }
		}
                return runningTotal;
            }catch(IOException e){
                e.printStackTrace();
            } 
        }catch(FileNotFoundException e){
            e.printStackTrace();
        }
        return runningTotal;
    }

    private static int doubleCountLines(File file) {
       String line;
       int runningTotal = 0;
       try {
            BufferedReader  br = new BufferedReader(new FileReader(file));
            try {
		while ((line = br.readLine()) != null) {
                    runningTotal += 1;
		}
                return runningTotal;
            }catch(IOException e){
                e.printStackTrace();
            } 
        }catch(FileNotFoundException e){
            e.printStackTrace();
        }
        return (runningTotal*2);
    }

    private static int uisErrorsLinesCount(File file) {
       String line;
       int runningTotal = 0;
       try {
            BufferedReader  br = new BufferedReader(new FileReader(file));
            try {
		while ((line = br.readLine()) != null) {
                    if(line.contains("ERRORS".toLowerCase())){ 
                        runningTotal += 1;
                    }
		}
                return runningTotal;
            }catch(IOException e){
                e.printStackTrace();
            } 
        }catch(FileNotFoundException e){
            e.printStackTrace();
        }
        return (runningTotal);
    }

}

   