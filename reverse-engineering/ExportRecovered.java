// Export recovered pseudocode and function metadata. Output is not buildable source.
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.nio.charset.StandardCharsets;
public class ExportRecovered extends GhidraScript {
 public void run() throws Exception {
  File dir=new File(getScriptArgs()[0]);dir.mkdirs();
  DecompInterface dec=new DecompInterface();dec.openProgram(currentProgram);
  int total=0,ok=0;
  try(PrintWriter out=new PrintWriter(new File(dir,"recovered.c"),StandardCharsets.UTF_8);
      PrintWriter index=new PrintWriter(new File(dir,"functions.tsv"),StandardCharsets.UTF_8);
      PrintWriter errors=new PrintWriter(new File(dir,"decompile-errors.tsv"),StandardCharsets.UTF_8)) {
   out.println("/* Ghidra pseudocode recovered from Hebdomad N900 v1.1.0. NOT compilable source. */");
   index.println("address\tname\tbytes\tdecompiled");
   FunctionIterator funcs=currentProgram.getFunctionManager().getFunctions(true);
   while(funcs.hasNext()&&!monitor.isCancelled()) {
    Function f=funcs.next();if(f.isExternal()||f.isThunk())continue;total++;
    DecompileResults r=dec.decompileFunction(f,15,monitor);
    boolean success=r.decompileCompleted()&&r.getDecompiledFunction()!=null;
    index.println(f.getEntryPoint()+"\t"+f.getName()+"\t"+f.getBody().getNumAddresses()+"\t"+success);
    if(success){ok++;out.println("\n/* Address: "+f.getEntryPoint()+" */\n"+r.getDecompiledFunction().getC());}
    else errors.println(f.getEntryPoint()+"\t"+f.getName()+"\t"+r.getErrorMessage());
    if(total%250==0){out.flush();index.flush();println("EXPORT_PROGRESS "+total+" successful="+ok);}
   }
  } finally {dec.dispose();}
  try(PrintWriter x=new PrintWriter(new File(dir,"string-xrefs.tsv"),StandardCharsets.UTF_8)) {
   x.println("string_address\tfunction\tvalue");
   DataIterator it=currentProgram.getListing().getDefinedData(true);
   while(it.hasNext()){
    Data d=it.next();if(!d.hasStringValue())continue;
    String s=d.getValue().toString().replace("\t"," ").replace("\n","\\n").replace("\r","");
    ReferenceIterator refs=currentProgram.getReferenceManager().getReferencesTo(d.getAddress());
    while(refs.hasNext()){
     Reference ref=refs.next();Function f=getFunctionContaining(ref.getFromAddress());
     x.println(d.getAddress()+"\t"+(f==null?ref.getFromAddress():f.getEntryPoint())+"\t"+s);
    }
   }
  }
  try(PrintWriter s=new PrintWriter(new File(dir,"summary.txt"),StandardCharsets.UTF_8)){
   s.println("Functions attempted: "+total);s.println("Functions decompiled: "+ok);
   s.println("Language: "+currentProgram.getLanguageID());
  }
  println("EXPORT_COMPLETE total="+total+" successful="+ok);
 }
}
