package assembler;

public class Main {
    
    public static void printHeader() {
        System.out.println("SASM Assembler written in Java.");
        System.out.println("Created by @aesophor 2018\n");
    }
    
    public static void main(String[] args) {
        printHeader();
        
        if (args.length < 1) {
            System.out.println("Please provide an SIC ASM file.");
            System.exit(1);
        } else {
            System.out.println("Assembling...\n");
        }
        
        String filename = args[0].split("[.]")[0];
        String intermediate = filename + ".tmp";
        String object = filename + ".obj";
        
        Assembler asm = new Assembler(args[0], intermediate, object);
        asm.assemble();
    }
    
}