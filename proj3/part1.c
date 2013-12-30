#include <emmintrin.h>
#define KERNX 5 //this is the x-size of the kernel. It will always be odd.
#define KERNY 3 //this is the y-size of the kernel. It will always be odd.
int conv2D(float* in, float* out, int data_size_X, int data_size_Y,
                    float* kernel)
{
    // the x coordinate of the kernel's center
    int kern_cent_X = (KERNX - 1)/2;
    // the y coordinate of the kernel's center
    int kern_cent_Y = (KERNY - 1)/2;

    //make a local kernal object to speed up memory access
    float local_kernel[KERNX*KERNY];
    for (int i = 0; i < KERNX * KERNY; i++) {
    	local_kernel[i] = kernel[i];
    }

    //printf("Start!\n");

    // main convolution loop
	for(int y = kern_cent_Y; y < data_size_Y-1; y++){ // the y coordinate of theoutput location we're focusing on
		for(int x = kern_cent_X; x < data_size_X-1; x++){ // the x coordinate of the output location we're focusing on
			float out_temp = 0;
		for(int i = -kern_cent_X; i <= kern_cent_X; i++){ // kernel unflipped x coordinate
			for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate
					// only do the operation if not out of bounds
					//if(x+i>-1 && x+i<data_size_X && y+j>-1 && y+j<data_size_Y){
						//Note that the kernel is flipped
						out_temp += 
								local_kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
					//}
				}
			}

			out[x+y*data_size_X] = out_temp;
		}
	}

	
	//printf("After main loop!\n");
	/*Have a loop for each edge case (top/left/right/bottom) along with corners; modify i and j  */

	//top-left corner (top row and left-most column of kernal excluded)
    for (int x =0; x<kern_cent_X; x++){
    	for (int y =0; y<kern_cent_Y; y++){
	for(int i = -kern_cent_X+1; i <= kern_cent_X; i++){ // kernel unflipped x coordinate
		//x and y stays constant as you are getting the top-left corner
		for(int j = -kern_cent_Y+1; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate
			out[x+y*data_size_X] += 
					local_kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
		}
	}
}
}
 
	//printf("After top-left corner loop!\n");
	//top-right corner (top row and right-most column of kernal excluded)
    for (int x =data_size_X-kern_cent_X; x<=data_size_X-1; x++){
    	for (int y =0; y<kern_cent_Y; y++){
	for(int i = -kern_cent_X; i <= kern_cent_X-1; i++){ // kernel unflipped x coordinate
		 //x and y stays constant as you are getting the top-right corner
		for(int j = -kern_cent_Y+1; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate
			out[x+y*data_size_X] += 
					local_kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
		}
	}
}
}

	//bottom-left corner (first i and j excluded)
for (int x =0; x<kern_cent_X; x++){
    	for (int y =data_size_Y-kern_cent_Y; y<=data_size_Y-1; y++){
	for(int i = -kern_cent_X+1; i <= kern_cent_X; i++){ // kernel unflipped x coordinate
		//x and y stays constant as you are getting the bottom-left corner
		for(int j = -kern_cent_Y; j <= kern_cent_Y-1; j++){ // kernel unflipped y coordinate
			out[x+y*data_size_X] += 
					local_kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
		}
	}
}
}

	//bottom-right corner (first i and j excluded)
for (int x =data_size_X-kern_cent_X; x<=data_size_X-1; x++){
    	for (int y =data_size_Y-kern_cent_Y; y<=data_size_Y-1; y++){
	for(int i = -kern_cent_X; i <= kern_cent_X-1; i++){ // kernel unflipped x coordinate
		 //x and y stays constant as you are getting the bottom-right corner
		for(int j = -kern_cent_Y; j <= kern_cent_Y-1; j++){ // kernel unflipped y coordinate
			out[x+y*data_size_X] += 
					local_kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
		}
	}
}
}

	//printf("Passed corner loops!\n");
	//top row (top row of kernal is excluded)

	for (int x = kern_cent_X; x < data_size_X-1; x++) {
		for (int y =0; y<kern_cent_Y; y++){ //y stays constant as you only need to access the top row of y's
		for(int i = -kern_cent_X; i <= kern_cent_X; i++){ // kernel unflipped x coordinate
			for(int j = -kern_cent_Y+1; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate skips first row
				out[x+y*data_size_X] += 
						local_kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
			}
		}
	}
}

	//printf("Passed top row loops!\n");
	//bottom row (last row of kernal is excluded)

	for (int x = kern_cent_X; x < data_size_X-1; x++) {
		for (int y =data_size_Y-kern_cent_Y; y<=data_size_Y-1; y++){  //y stays constant as you only need to access the bottom row of y's
		for(int i = -kern_cent_X; i <= kern_cent_X; i++){ // kernel unflipped x coordinate
			for(int j = -kern_cent_Y; j <= kern_cent_Y-1; j++){ // kernel unflipped y coordinate stops before last row
				out[x+y*data_size_X] += 
							local_kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
			}
		}
	}
}

	//printf("Passed bottom row loops!\n");

	//left column (first column of kernal is excluded)

	for (int y = kern_cent_Y; y < data_size_Y-1; y++) {
		 for (int x =0; x<kern_cent_X; x++){//x stays constant as you only need to access the left-most column of x's
		for(int i = -kern_cent_X+1; i <= kern_cent_X; i++){ // kernel unflipped x coordinate stops before last column
			for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate
				out[x+y*data_size_X] += 
						local_kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
			}
		}
	}
}

	//printf("Passed left column loops!\n");

	//right column (last column of kernal is excluded)
	
	for (int y = kern_cent_Y; y < data_size_Y-1; y++) {
		for (int x =data_size_X-kern_cent_X; x<=data_size_X-1; x++){ //x stays constant as you only need to access the right-most column of x's
		for(int i = -kern_cent_X; i <= kern_cent_X-1; i++){ // kernel unflipped x coordinate stops before last column
			for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate
				out[x+y*data_size_X] += 
						local_kernel[(kern_cent_X-i)+(kern_cent_Y-j)*KERNX] * in[(x+i) + (y+j)*data_size_X];
			}
		}
	}
}
	

	//printf("Passed all loops!\n");

	return 1;
}

