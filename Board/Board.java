import java.awt.Color;
import java.awt.Point;
import java.util.*;

public class Board {

	public static boolean iAmDebugging = true;
	private ArrayList pointCombinations = new ArrayList();
	private ArrayList donePoints = new ArrayList();
	private ArrayList donePoints1 = new ArrayList();
	private ArrayList donePoints2 = new ArrayList();
	private boolean computerDefeated = false;
	// Initialize an empty board with no colored edges.
	public Board ( ) {
		// You fill this in.
		for (int i = 1; i < 6; i++){
			for (int j = i+1; j < 7; j++){
				pointCombinations.add(new Connector(i,j));
			}
		}
		//System.out.println(pointCombinations);
	}
	
	// Add the given connector with the given color to the board.
	// Unchecked precondition: the given connector is not already chosen 
	// as RED or BLUE.
	public void add (Connector cnctr, Color c) {
		// You fill this in.
		boolean isOk = true;
		if (donePoints.contains(cnctr)){
			isOk = false;
			System.out.println("Illegal Connecting");
		}
		if (isOk){
			donePoints.add(cnctr);
			pointCombinations.remove(cnctr);
			//System.out.println(donePoints);
			//System.out.println(pointCombinations);
			if (c.equals(Color.RED)){
				donePoints1.add(cnctr);
			}else if (c.equals(Color.BLUE)){
				donePoints2.add(cnctr);
			}else{
				System.out.println("Wrong color");
			}
		}
	}
	
	// Set up an iterator through the connectors of the given color, 
	// which is either RED, BLUE, or WHITE. 
	// If the color is WHITE, iterate through the uncolored connectors.
	// No connector should appear twice in the iteration.  
	public Iterator<Connector> connectors (Color c) {
		// You fill this in.
		if  (c.equals(Color.RED)){
			Iterator itr = donePoints1.iterator();
			return itr;
		}else if (c.equals(Color.BLUE)){
			Iterator itr = donePoints2.iterator();
			return itr;
		}else if (c.equals(Color.WHITE)){
			Iterator itr = pointCombinations.iterator();
			return itr;
		}
		return null;
	}
	
	// Set up an iterator through all the 15 connectors.
	// No connector should appear twice in the iteration.  
	public Iterator<Connector> connectors ( ) {
		// You fill this in.
		Iterator itr = donePoints.iterator();
		return itr;
	}
	
	// Return the color of the given connector.
	// If the connector is colored, its color will be RED or BLUE;
	// otherwise, its color is WHITE.
	public Color colorOf (Connector e) {
		// You fill this in.
		if (donePoints1.contains(e)){
			return Color.RED;
		}else if (donePoints2.contains(e)){
			return Color.BLUE;
		}else{
			return Color.WHITE;
		}
	}
	
	// Unchecked prerequisite: cnctr is an initialized uncolored connector.
	// Let its endpoints be p1 and p2.
	// Return true exactly when there is a point p3 such that p1 is adjacent
	// to p3 and p2 is adjacent to p3 and those connectors have color c.
	public boolean formsTriangle (Connector cnctr, Color c) {
		// You fill this in.
		int p1 = cnctr.endPt1();
		int p2 = cnctr.endPt2();
		if (c.equals(Color.RED)){
			assert !donePoints1.contains(cnctr);
			for (Object i:donePoints1){
				Connector p = (Connector)i;
				if (p1 == p.endPt1()){
					Connector pToFind = new Connector(p2,p.endPt2());
					if (donePoints1.contains(pToFind)){
						return true;
					}
				}else if (p1 == p.endPt2()){
					Connector pToFind = new Connector(p2,p.endPt1());
					if (donePoints1.contains(pToFind)){
						return true;
					}
				}else if (p2 == p.endPt1()){
					Connector pToFind = new Connector(p1,p.endPt2());
					if (donePoints1.contains(pToFind)){
						return true;
					}
				}else if (p2 == p.endPt2()){
					Connector pToFind = new Connector(p1,p.endPt1());
					if (donePoints1.contains(pToFind)){
						return true;
					}
				}
			}
		}else if  (c.equals(Color.BLUE)){
			assert !donePoints2.contains(cnctr);
			for (Object i:donePoints2){
				Connector p = (Connector)i;
				if (p1 == p.endPt1()){
					Connector pToFind = new Connector(p2,p.endPt2());
					if (donePoints2.contains(pToFind)){
						computerDefeated =true;
						return true;
					}
				}else if (p1 == p.endPt2()){
					Connector pToFind = new Connector(p2,p.endPt1());
					if (donePoints2.contains(pToFind)){
						computerDefeated = true;
						return true;
					}
				}else if (p2 == p.endPt1()){
					Connector pToFind = new Connector(p1,p.endPt2());
					if (donePoints2.contains(pToFind)){
						computerDefeated = true;
						return true;
					}
				}else if (p2 == p.endPt2()){
					Connector pToFind = new Connector(p1,p.endPt1());
					if (donePoints2.contains(pToFind)){
						computerDefeated = true;
						return true;
					}
				}
			}
		}else{
			return false;
		}
		
		
		return false;
	}
	
	// The computer (playing BLUE) wants a move to make.
	// The board is assumed to contain an uncolored connector, with no 
	// monochromatic triangles.
	// There must be an uncolored connector, since if all 15 connectors are colored,
	// there must be a monochromatic triangle.
	// Pick the first uncolored connector that doesn't form a BLUE triangle.
	// If each uncolored connector, colored BLUE, would form a BLUE triangle,
	// return any uncolored connector.
	public Connector choice ( ) {
		// You fill this in.
		int kMin = 999;
		Connector bestP = new Connector(1,2);//OK?
		for (Object i:pointCombinations) {
			Connector p = (Connector)i;
			int k = 0;
			int p1 = p.endPt1();
			//System.out.println(p1);
			int p2 = p.endPt2();
			//System.out.println(p2);
			
			//Point uncallable
			if (!pointCombinations.contains(p)){
				k += 9999;
			}
			
			for (Object j: donePoints1){
				Connector s = (Connector)j;
				//System.out.println(s);
				if (s.endPt1() == p1||s.endPt2() == p1){
					k += 3;
					//System.out.println("wore");
				}
				if (s.endPt1() == p2||s.endPt2() == p2){
					k += 3;
				}
			}
			for (Object j: donePoints2){
				Connector s = (Connector)j;
				if (s.endPt1() == p1||s.endPt2() == p1){
					k += 4;
				}
				if (s.endPt1() == p2||s.endPt2() == p2){
					k += 4;
				}
			}
			
			//Can made triangle - survived triangle
			int m = 0;
			for (Object j: donePoints2){
				Connector s = (Connector)j;
				if (s.endPt1() == p1){
					m += 1;
					Connector pToFind = new Connector(p2,p.endPt2());
					if (donePoints1.contains(pToFind)){
						m = m - 1;
					}
				}
				if (s.endPt2() == p1){
					m += 1;
					Connector pToFind = new Connector(p2,p.endPt1());
					if (donePoints1.contains(pToFind)){
						m = m - 1;
					}
				}
				if (s.endPt1() == p2){
					m += 1;
					Connector pToFind = new Connector(p1,p.endPt2());
					if (donePoints1.contains(pToFind)){
						m = m - 1;
					}
				}
				if (s.endPt2() == p2){
					m += 1;
					Connector pToFind = new Connector(p1,p.endPt1());
					if (donePoints1.contains(pToFind)){
						m = m - 1;
					}
				}
			}
			
			if (donePoints2.size() > 2){
				m = m * 10;
			}else{
				m = m * 3;
			}
			k += m;
			
			//Triangle
			for (Object j: donePoints1){
				Connector s = (Connector)j;
				int left = 0;
				int right = 0;
				for (Object z: donePoints1){
					Connector q = (Connector)z;
					if (s.endPt1() == q.endPt1()&&(q.endPt2()==p1||q.endPt2()==p2)){
						left = q.endPt2();
					}else
					if (s.endPt1() == q.endPt2()&&(q.endPt1()==p1||q.endPt1()==p2)){
						left = q.endPt1();
					}else
					if (s.endPt2() == q.endPt1()&&(q.endPt2()==p1||q.endPt2()==p2)){
						right = q.endPt2();
					}else
					if (s.endPt2() == q.endPt2()&&(q.endPt1()==p1||q.endPt1()==p2)){
						right = q.endPt1();
					}
				}
				if (left > 0 && right > 0 && left != right){
					Connector px = new Connector(left,right);
					if (!this.formsTriangle(px,Color.BLUE)&&pointCombinations.contains(px)){
						return px;
					}
				}
			}
			
			//Kill itself
			if (this.formsTriangle(p,Color.BLUE)){
				k += 999;
			}
			//Help human
			if (this.formsTriangle(p,Color.RED)){
				k += 30;
			}
			//change the kMin
			if (k < kMin) {
				kMin = k;
				//System.out.println(kMin);
				bestP = p;
			}
		}
		if (kMin < 999) {
			return bestP;
		}
		for (Object i:pointCombinations) {
			Connector p = (Connector)i;
			if (pointCombinations.contains(p)){
				return p;
			}
		}
		return null;
	}

	// Return true if the instance variables have correct and internally
	// consistent values.  Return false otherwise.
	// Unchecked prerequisites:
	//	Each connector in the board is properly initialized so that 
	// 	1 <= myPoint1 < myPoint2 <= 6.
	public boolean isOK ( ) {
		// You fill this in.
		for (Object p:pointCombinations){
			Connector i = (Connector)p;
			if (i.endPt1()>i.endPt2()||i.endPt1()<1||i.endPt1()>5||i.endPt2()<2||i.endPt2()>6){
				return false;
			}
		}
		for (Object p:donePoints){
			Connector i = (Connector)p;
			if (i.endPt1()>i.endPt2()||i.endPt1()<1||i.endPt1()>5||i.endPt2()<2||i.endPt2()>6){
				return false;
			}
		}
		for (Object p:donePoints1){
			Connector i = (Connector)p;
			if (i.endPt1()>i.endPt2()||i.endPt1()<1||i.endPt1()>5||i.endPt2()<2||i.endPt2()>6){
				return false;
			}
		}
		for (Object p:donePoints2){
			Connector i = (Connector)p;
			if (i.endPt1()>i.endPt2()||i.endPt1()<1||i.endPt1()>5||i.endPt2()<2||i.endPt2()>6){
				return false;
			}
		}
		if (pointCombinations.size()+ donePoints1.size()+ donePoints2.size() != 15){
			return false;
		}
		if (donePoints1.size()<donePoints2.size()||donePoints1.size()>donePoints2.size()+1){
			return false;
		}
		if (computerDefeated){
			return false;
		}
		return true;
	}
}
