package assembler;

public class TextRecord extends Record {
    
    private static final int MAX_SIZE = 30; /* Sizes are in 'bytes' */
    
    private String recordStartAddr;   /* Col.   2-7 */
    private int recordLength;         /* Col.   8-9 */
    private String objectCode;        /* Col. 10-69 */
    
    
    static {
        indicator = "T";
    }
    
    public TextRecord(String recordStartAddr) {
        this.recordStartAddr = recordStartAddr;
        recordLength = 0;
        objectCode = "";
    }
     
    
    // Append object code to current Text record.
    public void append(ObjectCode o) {
        objectCode += o.toString();
        recordLength += o.getSize();
    }
    
    // Check if the ObjectCode o will fit in current Text record.
    public boolean isFull(Statement i, ObjectCode o) {
        int recordStartAddr = Integer.parseInt(this.recordStartAddr, 16);
        int currentAddr = Integer.parseInt(i.getAddress(), 16);
        int updateSize = o.getSize();
        return (currentAddr + updateSize - recordStartAddr > MAX_SIZE);
    }
    
    @Override
    public String toString() {
        String textRecord = indicator;
        textRecord += String.format("%6s", recordStartAddr).replace(" ", "0");
        textRecord += String.format("%02x", recordLength);
        textRecord += String.format("%s", objectCode);
        return textRecord + "\n";
    }
    
}
