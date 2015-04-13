/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package translationzipfilegenerator;

import TranslationZipFileGeneratorUtils.TranslationZipFileGeneratorStateHolder;
import TranslationZipFileGeneratorUtils.TranslationZipFileGeneratorUtil;

/**
 *
 * @author CSV6NHM
 */
public class TranslationZipFileGenerator {

    private static String[]argsRef;
    private final TranslationZipFileGeneratorStateHolder tzfgsh;
   
    public TranslationZipFileGenerator(){
       tzfgsh = new TranslationZipFileGeneratorStateHolder();
    }         
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        argsRef=args;
        TranslationZipFileGenerator tzfg = new TranslationZipFileGenerator();
        tzfg.startGenerator();
    }

    private void createExportedKeysFile(String FolderPath) {
       tzfgsh.setExportedKeysFileRef
        (TranslationZipFileGeneratorUtil.createFileFromPath
        (FolderPath));
        if(tzfgsh.getExportedKeysFileRef() == null){
            System.out.println("File Creation Failed, Retry again.");
            System.exit(1);
        }
        
    }

    private void startGenerator() {
        
        if (argsRef[0] == null){           
            System.out.println("You Must Specify the File Path For ExportKeys,"
                    + " Retry again.");
            System.exit(1);    
        }
        
        if (argsRef[1] == null){           
            System.out.println("You Must Specify the app name for this "
                    + "translation bundle, Retry again.");
            System.exit(1);    
        }
        
        if (argsRef[2] == null){           
            System.out.println("You Must Specify the File path for the "
                    + "Tranlation instrunctions, Retry again.");
            System.exit(1);    
        }
        
        createExportedKeysFile(argsRef[0]);
        tzfgsh.setAppNameForBundle(argsRef[1]);
        createTranslationInstructionsFile(argsRef[2]);
        completeTranslationWork();
    }

    private void completeTranslationWork() {
        tzfgsh.setWordCountToBeTranslated
        (TranslationZipFileGeneratorUtil.calculateTranslatableWords(tzfgsh));
        //Continue From Here later
    }

    private void createTranslationInstructionsFile(String filePath) {
        tzfgsh.setTranslationsInstructionsFilePath
        (TranslationZipFileGeneratorUtil.createFileFromPath
        (filePath));
        if(tzfgsh.getTranslationsInstructionsFilePath() == null){
            System.out.println("File Creation Failed, Retry again.");
            System.exit(1);
        }
    }
    
}
