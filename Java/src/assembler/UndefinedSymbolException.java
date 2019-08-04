package assembler;

public class UndefinedSymbolException extends Exception {
    
    public UndefinedSymbolException(Statement i) {
        super("Undefined symbol found: " + i.getLabel());
    }
}
