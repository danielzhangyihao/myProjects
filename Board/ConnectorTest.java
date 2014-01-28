import static org.junit.Assert.*;
import junit.framework.TestCase;

import org.junit.Test;


public class ConnectorTest extends TestCase{

	@Test
	public void testConstructor() {
		Connector b = new Connector(1 , 2);
		assertEquals(1, b.endPt1());
		assertEquals(2, b.endPt2());
		
		Connector c = new Connector(5 , 3);
	    assertEquals(3, c.endPt1());
		assertEquals(5, c.endPt2());}

	public void testEquals(){ 
		Connector d = new Connector(3 , 4);
		d.equals(new Connector(3,4));
		
		Connector e = new Connector(2 , 6);
		d.equals(new Connector(6,2));
		
		
	}
	
	public void testToString(){
		Connector f = new Connector(3 , 4);
		assertEquals("34", f.toString());
		
		Connector g = new Connector(5 , 6);
		assertEquals("56", g.toString());
		
		
	}
	
	
		
	
		
	
		
		
		
		
		
		
	}


