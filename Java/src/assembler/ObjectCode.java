package assembler;

public class ObjectCode {
    
    private String objectCode;
    private int size;
    
    
    // For operation code + operand.
    public ObjectCode(String operationCode, int operand) {
        String operandString = String.format("%04x", operand);
        objectCode = operationCode;
        objectCode += operandString;
        
        size = (operationCode.length() + operandString.length()) / 2;
    }
    
    // For BYTE and WORD.
    public ObjectCode(String constant) {
        objectCode = constant;
        size = constant.length() / 2;
    }
    
    
    public int getSize() {
        return size;
    }
    
    @Override
    public String toString() {
        return objectCode;
    }
    
    /*
    public static void main(String[] args) {
        System.out.println(new ObjectCode("4c", 0).getSize());
    }
    */
    
}
