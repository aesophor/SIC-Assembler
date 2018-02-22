package assembler;

import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.util.HashMap;

public class Assembler {
    
    private static final HashMap<String, String> OPTAB;
    private static final HashMap<String, Integer> SYMTAB;
    
    private final File source;
    private final File intermediate;
    private final File object;
    
    private String progName;
    private int progStartAddr;
    private int progLength;
    
    private int locctr;
    
    
    
    static {
        OPTAB = Tables.getOperationTable();
        SYMTAB = Tables.getSymbolTable();
    }
    
    public Assembler(String source, String intermediate, String object) {
        this.source = new File(source);
        this.intermediate = new File(intermediate);
        this.object = new File(object);
        
        this.progStartAddr = 0;
        this.progLength = 0;
        this.locctr = 0;
    }
    
    
    public void assemble() {
        try {
            firstPass();
            secondPass();
        } catch(IOException e) {
            e.printStackTrace();
        }
    }
            
    // Process the first pass which uses source code as input,
    // producing an intermediate file.
    public void firstPass() throws IOException {
        
        BufferedReader src = new BufferedReader(new FileReader(source));
        BufferedWriter inter = new BufferedWriter(new FileWriter(intermediate));
        
        // Read first input line.
        Statement instruction = new Statement(src.readLine());
        
        // Check if OPCODE is "START".
        if (instruction.getMnemonic().equals("START")) {
            // Save #[OPERAND] as starting address, and #[LABEL] as program name.
            progStartAddr = Integer.parseInt(instruction.getFirstOperand(), 16);
            progName = instruction.getLabel();
            
            // Initialize LOCCTR to starting address.
            locctr = progStartAddr;
            
            // Write line to intermediate file.
            inter.write(String.format("%04x\t%s\n", locctr, instruction));
            
            // Read next input line.
            instruction = new Statement(src.readLine());
        } else {
            // Initialize LOCCTR to 0.
            locctr = 0;
        }
        
        
        int growthSize = 0;
        
        while (!instruction.getMnemonic().equals("END")) {
            
            if (!instruction.isComment()) {
                
                try {
                    if (instruction.hasLabel()) {
                        // Search SYMTAB for LABEL.
                        if (SYMTAB.containsKey(instruction.getLabel())) {
                            // Set error flag (duplicate symbol).
                            throw new DuplicateSymbolException(instruction);
                        } else {
                            // Insert (LABEL, LOCCTR) into SYMTAB.
                            SYMTAB.put(instruction.getLabel(), locctr);
                        }
                    }
                
                    // Search OPTAB for OPCODE.
                    if (OPTAB.containsKey(instruction.getMnemonic())) {
                        growthSize = 3;
                    } else {
                        
                        switch (instruction.getMnemonic()) {
                            case "WORD":
                                growthSize = 3;
                                break;
                                
                            case "RESW":
                                growthSize = Integer.parseInt(instruction.getFirstOperand()) * 3;
                                break;
                                
                            case "RESB":
                                growthSize = Integer.parseInt(instruction.getFirstOperand());
                                break;
                                
                            case "BYTE":
                                // Find length of constant in bytes.
                                // Add length to LOCCTR.
                                String[] operand = instruction.getFirstOperand().split("\\'");
                                String dataType = operand[0];
                                int length = operand[1].length();
                                
                                switch (dataType) {
                                    case "C":
                                        growthSize = 1 * length;
                                        break;
                                    case "X":
                                        if (length%2 != 0) { length++; }
                                        growthSize = 1 * length / 2;
                                        break;
                                    default:
                                        throw new InvalidDataTypeException(instruction);
                                }
                                break;
                                
                            default:
                                // Set error flag (invalid operation code).
                                throw new InvalidOperationCodeException(instruction);
                        }
                    }
                } catch(DuplicateSymbolException | InvalidDataTypeException | InvalidOperationCodeException e) {
                    System.out.println(e);
                }
                
                // Write line to intermediate file.
                inter.write(String.format("%04x\t%s\n", locctr, instruction));
                
                // Add offset to locctr.
                locctr += growthSize;
                growthSize = 0;
            }
            
            // Read next input line.
            instruction = new Statement(src.readLine());
        }
        
        // Write last line to intermediate file.
        inter.write(String.format("\t%s\n", instruction));
        progLength = locctr - progStartAddr;
        
        // Close BufferedReader and BufferedWriter.
        src.close();
        inter.close();
    }
    
    
    // Process the second pass which uses intermediate file as input,
    // producing an object file.
    public void secondPass() throws IOException {
        
        BufferedReader inter = new BufferedReader(new FileReader(intermediate));
        BufferedWriter obj = new BufferedWriter(new FileWriter(object));
                
        // Intermedate statement will be split into a String[].
        // address refers to the first item, while statement to the second.
        String[] statementParts;
        String currentAddress;
        String currentStatement;
        
        
        // Read first input line {from intermediate file}.
        statementParts = Statement.parseIntermediateLine(inter.readLine());
        currentAddress = statementParts[0];
        currentStatement = statementParts[1];
        Statement s = new Statement(currentAddress, currentStatement);
        
        if (s.getMnemonic().equals("START")) {
            // Read next input line.
            statementParts = Statement.parseIntermediateLine(inter.readLine());
            currentAddress = statementParts[0];
            currentStatement = statementParts[1];
            s = new Statement(currentAddress, currentStatement);
        }
        
        // Write Header record to object program.
        obj.write(new HeaderRecord(progName, progStartAddr, progLength).toString());
        
        
        // Initialize first Text record.
        TextRecord textRecord = new TextRecord(s.getAddress());
        ObjectCode objectCode = null;
        
        String operationCode;
        int operand = 0;
        
        while (!s.getMnemonic().equals("END")) {
            
            if (!s.isComment()) {
                
                try {
                    
                    if (OPTAB.containsKey(s.getMnemonic())) {
                        // Store mnemonic value as operation code.
                        operationCode = OPTAB.get(s.getMnemonic());
                        
                        if (s.hasFirstOperand()) {
                            
                            if (SYMTAB.containsKey(s.getFirstOperand())) {
                                // Store symbol value as operand address.
                                operand = SYMTAB.get(s.getFirstOperand());
                                
                                // The presence of second operand indicates 
                                // index addressing mode.
                                if (s.hasSecondOperand() && 
                                    s.getSecondOperand().equals("X")) {
                                    operand |= 0b1000_0000_0000_0000;
                                }
                                
                            } else {
                                // Set error flag (undefined symbol)
                                throw new UndefinedSymbolException(s);
                            }
                            
                        } else {
                            // Store 0 as operand address.
                            operand = 0;
                        }
                        
                        // Assemble the object code instruction.
                        objectCode = new ObjectCode(operationCode, operand);
                        
                    } else {
                        
                        int constantValue;
                        
                        switch (s.getMnemonic()) {
                            case "BYTE":
                                // Convert constant to object code (in ascii).
                                String[] operands = s.getFirstOperand().split("\\'");
                                String dataType = operands[0];
                                String constant = operands[1];
                                
                                switch (dataType) {
                                    case "C":
                                        String constantAscii = "";
                                        for (int i = 0; i < constant.length(); i++) {
                                            constantAscii += Integer.toHexString((int) constant.charAt(i));
                                        }
                                        objectCode = new ObjectCode(constantAscii);
                                        break;
                                    
                                    case "X":
                                        objectCode = new ObjectCode(constant);
                                        break;
                                        
                                    default:
                                        throw new InvalidDataTypeException(s);
                                }
                                break;
                                
                            case "WORD":
                                // Convert constant to object code.
                                constantValue = Integer.parseInt(s.getFirstOperand());
                                objectCode = new ObjectCode(String.format("%06x", constantValue));
                                break;
                                
                            case "RESB":
                                objectCode = new ObjectCode("");
                                break;
                                
                            case "RESW":
                                objectCode = new ObjectCode("");
                                break;
                                
                            default:
                                throw new InvalidOperationCodeException(s);
                        }
                    }
                } catch(UndefinedSymbolException | InvalidDataTypeException | InvalidOperationCodeException e) {
                    System.out.println(e);
                }
                
                if (textRecord.isFull(s, objectCode)) {
                    // Write Text record to object program.
                    obj.write(textRecord.toString());
                    
                    // Initialize new Text record.
                    textRecord = new TextRecord(currentAddress);
                }
                
                // Add object code to Text record.
                textRecord.append(objectCode);
            }
            
            // Read next input line.
            statementParts = Statement.parseIntermediateLine(inter.readLine());
            currentAddress = statementParts[0];
            currentStatement = statementParts[1];
            s = new Statement(currentAddress, currentStatement);
        }
        
        // Write last Text record to pbject program.
        obj.write(textRecord.toString());
        
        // Write End record to object program.
        obj.write(new EndRecord(progStartAddr).toString());
        
        // Close BufferedReader and BufferedWriter.
        inter.close();
        obj.close();
    }
    
}