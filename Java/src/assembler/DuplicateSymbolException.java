package assembler;

public class DuplicateSymbolException extends Exception {
    
    public DuplicateSymbolException(Statement i) {
        super("Duplicate symbol found: " + i.getLabel());
    }
}
