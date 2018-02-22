package assembler;


public class Statement {
    
    private static final String COMMENT_INDICATOR = ".";
    private static final String BLANK_INDICATOR = "-";
    
    private static final int LABEL     = 0;
    private static final int MNEMONIC  = 1;
    private static final int FIRST_OP  = 2;
    private static final int SECOND_OP = 3;
    
    private String[] tokens;
    private String   line;
    private String   address;
    
    
    // For use with parsing source files.
    public Statement(String line) {
        
        this.line = line;
        
        // Split the line using whitespaces and tabs as deliminators.
        // Also attempt to extract the second operand with a comma (,).
        if ( !isComment() ) {
            tokens = line.split("\\s+|\\,");
        } else {
            tokens = new String[0];
        }
    }
    
    // For use with parsing intermediate files.
    public Statement(String location, String line) {
        this(line);
        this.address = location;
    }
    
    
    public String getAddress() {
        return address;
    }
    
    public String getLabel() {
        return (tokens.length > 0) ? tokens[LABEL] : "";
    }
    
    public String getMnemonic() {
        return (tokens.length > 1) ? tokens[MNEMONIC] : "";
    }
    
    public String getFirstOperand() {
        return (tokens.length > 2) ? tokens[FIRST_OP] : "";
    }
    
    public String getSecondOperand() {
        return (tokens.length > 3) ? tokens[SECOND_OP] : "";
    }
    
    public boolean hasLabel() {
        return (tokens.length > 0) ? (!tokens[LABEL].equals(BLANK_INDICATOR)) : false;
    }
    
    public boolean hasFirstOperand() {
        return (tokens.length > 2) ? (!tokens[FIRST_OP].equals(BLANK_INDICATOR)) : false;
    }
    
    public boolean hasSecondOperand() {
        return (tokens.length > 3) ? (!tokens[SECOND_OP].equals(BLANK_INDICATOR)) : false;
    }
    
    public boolean isComment() {
        return line.startsWith(COMMENT_INDICATOR);
    }
    
    public static String[] parseIntermediateLine(String line) {
        return line.split("\\s+", 2);
    }
    
    @Override
    public String toString() {
        return line;
    }
    
    /*
    public String show() {
        return java.util.Arrays.toString(tokens);
    }
    
    public static void main(String[] args) {
        String s = "LOOP:    ADD eax,0x3c";
        //String s = ". ."; // show() --> null
        System.out.println(new Statement(s).show());
    }
    */
    
}
