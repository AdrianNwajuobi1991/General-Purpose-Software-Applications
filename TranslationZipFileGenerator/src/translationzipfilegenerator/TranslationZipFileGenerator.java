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

    private void createBundleFolder(String FolderPath) {
        TranslationZipFileGeneratorUtil.createFileFromPath
        (tzfgsh, FolderPath);
        if(tzfgsh.getExportedKeysFileRef() == null){
            System.out.println("File Creation Failed, Retry again.");
            System.exit(1);
        }
    }

    private void startGenerator() {
        createBundleFolder(argsRef[0]);
        completeTranslationWork();
    }

    private void completeTranslationWork() {
        tzfgsh.setWordCountToBeTranslated
        (TranslationZipFileGeneratorUtil.calculateTranslatableWords(tzfgsh));
        //Continue From Here later
    }
    
}
