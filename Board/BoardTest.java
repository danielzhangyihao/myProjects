import java.awt.Color;
import java.util.ArrayList;
import java.util.Iterator;

import junit.framework.TestCase;

public class BoardTest extends TestCase {
	
	// Check empty board.
	public void testEmptyBoard ( ) {
		Board b = new Board ( );
		assertTrue (b.isOK ( ));
		checkCollection (b, 0, 0); // applies more tests
		assertTrue (!b.formsTriangle (new Connector (1, 2), Color.RED));
	}
	
	// Check one-connector board.
	public void test1Connector ( ) {
		Board b = new Board ( );
		b.add (new Connector (1, 2), Color.RED);
		assertTrue (b.isOK ( ));
		checkCollection (b, 1, 0);
		
		Iterator<Connector> iter = b.connectors (Color.RED);
		assertTrue (iter.hasNext ( ));
		Connector cnctr = iter.next ( );
		assertEquals (b.colorOf (cnctr), Color.RED);
		assertEquals (new Connector (1, 2), cnctr);
		assertTrue (!iter.hasNext ( ));
		
		assertTrue (!b.formsTriangle (new Connector(1,3), Color.RED));
		assertTrue (!b.formsTriangle (new Connector(5,6), Color.RED));
		assertTrue (!b.choice ( ).equals (new Connector (1, 2)));
		assertEquals (b.colorOf (b.choice ( )), Color.WHITE);
	}
	
	// More tests go here.
	
	// (a useful helper method)
	// Make the following checks on a board that should be legal:
	
	public void test2Connector ( ) {
//		Check connector counts (# reds + # blues + # uncolored should be 16.
		//Check red vs. blue counts.
		//	Check for duplicate connectors.
		//	Check for a blue triangle, which shouldn't exist.
		Board b = new Board ( );
		b.add (new Connector (1, 2), Color.RED);
		b.add (new Connector (1, 3), Color.BLUE);
		b.add (new Connector (1, 4), Color.RED);
		b.add (new Connector (1, 5), Color.BLUE);
		b.add (new Connector (1, 6), Color.RED);
		assertTrue (b.isOK ( ));
		b.add (new Connector (2, 5), Color.BLUE);
		assertTrue (b.isOK ( ));
		b.add (new Connector (2, 5), Color.BLUE);
		assertTrue (b.isOK ( ));
		b.add (new Connector (4, 5), Color.BLUE);
		assertFalse (b.isOK ( ));
		
		
		Board c = new Board ( );
		c.add (new Connector (1, 2), Color.RED);
		c.add (new Connector (1, 3), Color.RED);
		assertFalse (c.isOK ( ));
		
		
		Board d = new Board ( );
		d.add (new Connector (1, 2), Color.RED);
		d.add (new Connector (1, 3), Color.BLUE);
		d.add (new Connector (1, 4), Color.RED);
		d.add (new Connector (1, 5), Color.BLUE);
		d.add (new Connector (1, 6), Color.RED);
		assertTrue (d.formsTriangle(new Connector (3, 5), Color.BLUE));//TODO
		assertFalse (d.isOK ( ));
		
		Board e = new Board ( );
		e.add (new Connector (3, 4), Color.RED);
		e.add (new Connector (3, 5), Color.BLUE);
		e.add (new Connector (1, 3), Color.RED);
		e.add (new Connector (2, 3), Color.BLUE);
		e.add (new Connector (1, 6), Color.RED);
		assertFalse(d.formsTriangle(new Connector (1, 2), Color.BLUE));
		
		
	}
	
	private void checkCollection (Board b, int redCount, int blueCount) {
		// Fill this in if you'd like to use this method.
		

	}
}
