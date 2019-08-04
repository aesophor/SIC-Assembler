package assembler;

public class InvalidOperationCodeException extends Exception {
    
    public InvalidOperationCodeException(Statement i) {
        super("Invalid operation code found: " + i.getMnemonic());
    }
}
