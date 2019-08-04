package assembler;

import java.io.File;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;
import java.util.HashMap;

public class Tables {
    
    private static final String OPTAB_PATH = "tables/OperationTable";
    
    private static final HashMap<String, Integer> SYMTAB;
    private static final HashMap<String, String> OPTAB;
    
    
    static {
        SYMTAB = new HashMap<>();
        OPTAB = new HashMap<>();
        setOperationTable(new File(OPTAB_PATH));
    }
    
    private Tables() {
        // Prevents instantiation.
    }
    
    
    public static HashMap<String, Integer> getSymbolTable() {
        return SYMTAB;
    }
    
    public static HashMap<String, String> getOperationTable() {
        return OPTAB;
    }
    
    public static void setOperationTable(File table) {
        try {
            BufferedReader opTable = new BufferedReader(new FileReader(table));
            String line;
            String[] lineTokens;
            
            while ((line = opTable.readLine()) != null) {
                lineTokens = line.split("\\s+");
                OPTAB.put(lineTokens[0], lineTokens[3]);
            }
        } catch(IOException e) {
            System.out.println(e);
        }
    }
    
    /*
    public static void main(String[] args) {
        try {
            Tables.setOperationTable(new File("tables/OperationTable"));
        } catch(IOException e) {
            System.out.println(e);
        }
        
        System.out.println(Tables.getOperationTable().get("DIVF"));
    }
    */
    
}
