public class Connector {
	
	// Implement an immutable connector that connects two points on the game board.
	// Invariant: 1 <= myPoint1 < myPoint2 <= 6.
	
	private int myPoint1, myPoint2;
	
	public Connector (int p1, int p2) {
		if (p1 < p2) {
			myPoint1 = p1;
			myPoint2 = p2;
		} else {
			myPoint1 = p2;
			myPoint2 = p1;
		}
	}
	
	public int endPt1 ( ) {
		return myPoint1;
	}
	
	public int endPt2 ( ) {
		return myPoint2;
	}
	
	public boolean equals (Object obj) {
		Connector e = (Connector) obj;
		return (e.myPoint1 == myPoint1 && e.myPoint2 == myPoint2);
	}
	
	public String toString ( ) {
		return "" + myPoint1 + myPoint2;
	}
	
	// Format of a connector is endPoint1 + endPoint2 (+ means string concatenation),
	// possibly surrounded by white space. Each endpoint is a digit between
	// 1 and 6, inclusive; moreover, the endpoints aren't identical.
	// If the contents of the given string is correctly formatted,
	// return the corresponding connector.  Otherwise, throw IllegalFormatException.
	public static Connector toConnector (String s) throws IllegalFormatException {
		// You fill this in.
	      
        if (s ==null ){
                throw new IllegalFormatException("Input is null.");
        }
       
        String trimmed = s.trim();
       
        
        if (trimmed == ""){
                throw new IllegalFormatException("Input is empty");
        }
       
        int endPoints = Integer.parseInt(trimmed);
        int [] ptArray = new int[2];
       
        if (endPoints >= 66 || endPoints <= 11){
                throw new IllegalFormatException("Endpoint must be between 1 and 6");
        }
       
        for (int i=1;i>=0;i--){
                int temp = endPoints%10;//13
                ptArray[i] = temp;     
                endPoints = endPoints/10;
        }
       
    
        if (ptArray[0] == ptArray[1]){
                throw new IllegalFormatException("Endpoints cannot be the same.");
        }
        return new Connector(ptArray[0],ptArray[1]);
}
}
