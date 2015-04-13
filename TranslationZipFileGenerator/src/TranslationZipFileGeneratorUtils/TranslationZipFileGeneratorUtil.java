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

    
    private static int runningTotalCountWords = 0;
    private static int runningTotalDoubleCountLines = 0;
    private static int runningtTotalErrorLineCount = 0;
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

    public static int calculateTranslatableWords
        (TranslationZipFileGeneratorStateHolder tzfgsh) {
        File file = tzfgsh.getExportedKeysFileRef();
        calculateDoWork(file,"total_words");
        calculateDoWork(file,"double_lines");
        calculateDoWork(file,"num_Errors");
        int totalTranslatableWordCount = runningTotalCountWords -(
                runningTotalDoubleCountLines-runningtTotalErrorLineCount);
        return totalTranslatableWordCount;
    }
        
    private static void calculateDoWork(File file, String taskToDo){
       String line;
       try {
            BufferedReader  br = new BufferedReader(new FileReader(file));
            try {
		while ((line = br.readLine()) != null) {
                    if (taskToDo.equals("total_words")){
                        countWords(line);
                    }else if(taskToDo.equals("double_lines")){
                        doubleCountLines();
                    }else if(taskToDo.equals("num_Errors")){
                        uisErrorsLinesCount(line);
                    }
		}
            }catch(IOException e){
                e.printStackTrace();
            } 
        }catch(FileNotFoundException e){
            e.printStackTrace();
        }
    }  

    private static void countWords(String lineOfFile) {
       String []parts = lineOfFile.split(" ");
       for(String part : parts){
           runningTotalCountWords += 1;        
        }
    }

    private static void doubleCountLines() {
       runningTotalDoubleCountLines += 2;
    }

    private static void uisErrorsLinesCount(String lineOfFile) {
       if(lineOfFile.contains("ERRORS".toLowerCase())){//Refine later 
           runningtTotalErrorLineCount += 1;
       }
    }

}

   