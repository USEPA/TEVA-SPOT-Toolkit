package org.coinor.opents.net;
import  org.coinor.opents.*;

public interface Initializer
{
    
    public abstract java.awt.Component getCustomizer();
    
    public abstract boolean initialize( java.awt.Component customizer );
    
    public abstract Solution getStartingSolution();
    
    public abstract ObjectiveFunction getObjectiveFunction();
    
    public abstract TabuList getTabuList();
    
    public abstract MoveManager getMoveManager();
    
    public abstract void setBestSolution( Solution soln );
    
}   // end Initializer
