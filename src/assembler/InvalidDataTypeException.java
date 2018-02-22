package assembler;

public class InvalidDataTypeException extends Exception {
    
    public InvalidDataTypeException(Statement i) {
        super("Invalid data type found: " + i.getFirstOperand());
    }
}
