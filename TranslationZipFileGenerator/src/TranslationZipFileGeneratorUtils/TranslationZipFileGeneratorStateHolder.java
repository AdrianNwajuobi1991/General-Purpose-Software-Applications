/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package TranslationZipFileGeneratorUtils;

import java.io.File;

/**
 *
 * @author CSV6NHM
 */
public class TranslationZipFileGeneratorStateHolder {
    private File exportedKeysFileRef = null;
    private int wordCountToBeTranslated = 0;
    private String appNameForBundle = null;
    private File translationInstructionsFileRef;
    

    public File getExportedKeysFileRef() {
        return exportedKeysFileRef;
    }

    public void setExportedKeysFileRef(File file) {
        this.exportedKeysFileRef = file;
    }
    
    public int getWordCountToBeTranslated(){
        return wordCountToBeTranslated;
    }
    
    public void setWordCountToBeTranslated(int wordCountToBeTranslated){
        this.wordCountToBeTranslated = wordCountToBeTranslated;
    }
    
    public void setAppNameForBundle(String appNameForBundle){
        this.appNameForBundle = appNameForBundle;
    }
    
    public String getAppNameForBundle(){
        return appNameForBundle;
    }

    public void setTranslationsInstructionsFilePath(File file) {
        this.translationInstructionsFileRef = file;
    }

    public File getTranslationsInstructionsFilePath() {
        return translationInstructionsFileRef;
    }
    
}
