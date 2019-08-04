package assembler;

public class HeaderRecord extends Record {
    
    private final String progName;     /* Col.   2-7 */
    private final int progStartAddr;   /* Col.  8-13 */
    private final int progLength;      /* Col. 14-19 */
    
    
    static {
        indicator = "H";
    }
    
    public HeaderRecord(String progName, int startAddr, int progLength) {
        this.progName = progName;
        this.progStartAddr = startAddr;
        this.progLength = progLength;
    }
    
    
    private static String appendSpace(String s, int maxLength) {
        String progName = s;
        for (int i = 0; i < (maxLength - s.length()); i++) {
            progName += " ";
        }
        return progName;
    }
    
    @Override
    public String toString() {
        String headerRecord = indicator;
        headerRecord += String.format(appendSpace(progName, 6));
        headerRecord += String.format("%06x", progStartAddr);
        headerRecord += String.format("%06x", progLength);
        return headerRecord + "\n";
    }
    
    /*
    public static void main(String[] args) {
        System.out.println(HeaderRecord.appendWithSpace("COPY", 6) + "|");
    }
    */
    
}
