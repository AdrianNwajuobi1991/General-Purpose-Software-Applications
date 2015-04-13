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
    

    public File getExportedKeysFileRef() {
        return exportedKeysFileRef;
    }

    public void setFolderPathFileRef(File file) {
        this.exportedKeysFileRef = file;
    }
    
    public int getWordCountToBeTranslated(){
        return wordCountToBeTranslated;
    }
    
    public void setWordCountToBeTranslated(int wordCountToBeTranslated){
        this.wordCountToBeTranslated = wordCountToBeTranslated;
    }
    
    
}
