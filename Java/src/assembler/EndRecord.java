package assembler;

public class EndRecord extends Record {
    
    private final int progStartAddr;   /* Col.   2-7 */
    
    
    static {
        indicator = "E";
    }
    
    public EndRecord(int progStartAddr) {
        this.progStartAddr = progStartAddr;
    }
    
    
    @Override
    public String toString() {
        return indicator + String.format("%06x\n", progStartAddr);
    }
    
}
